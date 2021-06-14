#include "ipc.h"

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <yajl/yajl_gen.h>
#include <yajl/yajl_tree.h>

#include "util.h"
#include "yajl_dumps.h"

static struct sockaddr_un sockaddr;
static struct epoll_event sock_epoll_event;
static IPCClientList ipc_clients = NULL;
static int epoll_fd = -1;
static int sock_fd = -1;
static IPCCommand *ipc_commands;
static unsigned int ipc_commands_len;
// Max size is 1 MB
static const uint32_t MAX_MESSAGE_SIZE = 1000000;
static const int IPC_SOCKET_BACKLOG = 5;

/**
 * Create IPC socket at specified path and return file descriptor to socket.
 * This initializes the static variable sockaddr.
 */
static int
ipc_create_socket(const char *filename)
{
  char *normal_filename;
  char *parent;
  const size_t addr_size = sizeof(struct sockaddr_un);
  const int sock_type = SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC;

  normalizepath(filename, &normal_filename);

  // In case socket file exists
  unlink(normal_filename);

  // For portability clear the addr structure, since some implementations have
  // nonstandard fields in the structure
  memset(&sockaddr, 0, addr_size);

  parentdir(normal_filename, &parent);
  // Create parent directories
  mkdirp(parent);
  free(parent);

  sockaddr.sun_family = AF_LOCAL;
  strcpy(sockaddr.sun_path, normal_filename);
  free(normal_filename);

  sock_fd = socket(AF_LOCAL, sock_type, 0);
  if (sock_fd == -1) {
    fputs("Failed to create socket\n", stderr);
    return -1;
  }

  DEBUG("Created socket at %s\n", sockaddr.sun_path);

  if (bind(sock_fd, (const struct sockaddr *)&sockaddr, addr_size) == -1) {
    fputs("Failed to bind socket\n", stderr);
    return -1;
  }

  DEBUG("Socket binded\n");

  if (listen(sock_fd, IPC_SOCKET_BACKLOG) < 0) {
    fputs("Failed to listen for connections on socket\n", stderr);
    return -1;
  }

  DEBUG("Now listening for connections on socket\n");

  return sock_fd;
}

/**
 * Internal function used to receive IPC messages from a given file descriptor.
 *
 * Returns -1 on error reading (could be EAGAIN or EINTR)
 * Returns -2 if EOF before header could be read
 * Returns -3 if invalid IPC header
 * Returns -4 if message length exceeds MAX_MESSAGE_SIZE
 */
static int
ipc_recv_message(int fd, uint8_t *msg_type, uint32_t *reply_size,
                 uint8_t **reply)
{
  uint32_t read_bytes = 0;
  const int32_t to_read = sizeof(dwm_ipc_header_t);
  char header[to_read];
  char *walk = header;

  // Try to read header
  while (read_bytes < to_read) {
    const ssize_t n = read(fd, header + read_bytes, to_read - read_bytes);

    if (n == 0) {
      if (read_bytes == 0) {
        fprintf(stderr, "Unexpectedly reached EOF while reading header.");
        fprintf(stderr,
                "Read %" PRIu32 " bytes, expected %" PRIu32 " total bytes.\n",
                read_bytes, to_read);
        return -2;
      } else {
        fprintf(stderr, "Unexpectedly reached EOF while reading header.");
        fprintf(stderr,
                "Read %" PRIu32 " bytes, expected %" PRIu32 " total bytes.\n",
                read_bytes, to_read);
        return -3;
      }
    } else if (n == -1) {
      // errno will still be set
      return -1;
    }

    read_bytes += n;
  }

  // Check if magic string in header matches
  if (memcmp(walk, IPC_MAGIC, IPC_MAGIC_LEN) != 0) {
    fprintf(stderr, "Invalid magic string. Got '%.*s', expected '%s'\n",
            IPC_MAGIC_LEN, walk, IPC_MAGIC);
    return -3;
  }

  walk += IPC_MAGIC_LEN;

  // Extract reply size
  memcpy(reply_size, walk, sizeof(uint32_t));
  walk += sizeof(uint32_t);

  if (*reply_size > MAX_MESSAGE_SIZE) {
    fprintf(stderr, "Message too long: %" PRIu32 " bytes. ", *reply_size);
    fprintf(stderr, "Maximum message size is: %d\n", MAX_MESSAGE_SIZE);
    return -4;
  }

  // Extract message type
  memcpy(msg_type, walk, sizeof(uint8_t));
  walk += sizeof(uint8_t);

  if (*reply_size > 0)
    (*reply) = malloc(*reply_size);
  else
    return 0;

  read_bytes = 0;
  while (read_bytes < *reply_size) {
    const ssize_t n = read(fd, *reply + read_bytes, *reply_size - read_bytes);

    if (n == 0) {
      fprintf(stderr, "Unexpectedly reached EOF while reading payload.");
      fprintf(stderr, "Read %" PRIu32 " bytes, expected %" PRIu32 " bytes.\n",
              read_bytes, *reply_size);
      free(*reply);
      return -2;
    } else if (n == -1) {
      // TODO: Should we return and wait for another epoll event?
      // This would require saving the partial read in some way.
      if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) continue;

      free(*reply);
      return -1;
    }

    read_bytes += n;
  }

  return 0;
}

/**
 * Internal function used to write a buffer to a file descriptor
 *
 * Returns number of bytes written if successful write
 * Returns 0 if no bytes were written due to EAGAIN or EWOULDBLOCK
 * Returns -1 on unknown error trying to write, errno will carry over from
 *   write() call
 */
static ssize_t
ipc_write_message(int fd, const void *buf, size_t count)
{
  size_t written = 0;

  while (written < count) {
    const ssize_t n = write(fd, (uint8_t *)buf + written, count - written);

    if (n == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        return written;
      else if (errno == EINTR)
        continue;
      else
        return n;
    }

    written += n;
    DEBUG("Wrote %zu/%zu to client at fd %d\n", written, count, fd);
  }

  return written;
}

/**
 * Initialization for generic event message. This is used to allocate the yajl
 * handle, set yajl options, and in the future any other initialization that
 * should occur for event messages.
 */
static void
ipc_event_init_message(yajl_gen *gen)
{
  *gen = yajl_gen_alloc(NULL);
  yajl_gen_config(*gen, yajl_gen_beautify, 1);
}

/**
 * Prepares buffers of IPC subscribers of specified event using buffer from yajl
 * handle.
 */
static void
ipc_event_prepare_send_message(yajl_gen gen, IPCEvent event)
{
  const unsigned char *buffer;
  size_t len = 0;

  yajl_gen_get_buf(gen, &buffer, &len);
  len++;  // For null char

  for (IPCClient *c = ipc_clients; c; c = c->next) {
    if (c->subscriptions & event) {
      DEBUG("Sending selected client change event to fd %d\n", c->fd);
      ipc_prepare_send_message(c, IPC_TYPE_EVENT, len, (char *)buffer);
    }
  }

  // Not documented, but this frees temp_buffer
  yajl_gen_free(gen);
}

/**
 * Initialization for generic reply message. This is used to allocate the yajl
 * handle, set yajl options, and in the future any other initialization that
 * should occur for reply messages.
 */
static void
ipc_reply_init_message(yajl_gen *gen)
{
  *gen = yajl_gen_alloc(NULL);
  yajl_gen_config(*gen, yajl_gen_beautify, 1);
}

/**
 * Prepares the IPC client's buffer with a message using the buffer of the yajl
 * handle.
 */
static void
ipc_reply_prepare_send_message(yajl_gen gen, IPCClient *c,
                               IPCMessageType msg_type)
{
  const unsigned char *buffer;
  size_t len = 0;

  yajl_gen_get_buf(gen, &buffer, &len);
  len++;  // For null char

  ipc_prepare_send_message(c, msg_type, len, (const char *)buffer);

  // Not documented, but this frees temp_buffer
  yajl_gen_free(gen);
}

/**
 * Find the IPCCommand with the specified name
 *
 * Returns 0 if a command with the specified name was found
 * Returns -1 if a command with the specified name could not be found
 */
static int
ipc_get_ipc_command(const char *name, IPCCommand *ipc_command)
{
  for (int i = 0; i < ipc_commands_len; i++) {
    if (strcmp(ipc_commands[i].name, name) == 0) {
      *ipc_command = ipc_commands[i];
      return 0;
    }
  }

  return -1;
}

/**
 * Parse a IPC_TYPE_RUN_COMMAND message from a client. This function extracts
 * the arguments, argument count, argument types, and command name and returns
 * the parsed information as an IPCParsedCommand. If this function returns
 * successfully, the parsed_command must be freed using
 * ipc_free_parsed_command_members.
 *
 * Returns 0 if the message was successfully parsed
 * Returns -1 otherwise
 */
static int
ipc_parse_run_command(char *msg, IPCParsedCommand *parsed_command)
{
  char error_buffer[1000];
  yajl_val parent = yajl_tree_parse(msg, error_buffer, 1000);

  if (parent == NULL) {
    fputs("Failed to parse command from client\n", stderr);
    fprintf(stderr, "%s\n", error_buffer);
    fprintf(stderr, "Tried to parse: %s\n", msg);
    return -1;
  }

  // Format:
  // {
  //   "command": "<command name>"
  //   "args": [ "arg1", "arg2", ... ]
  // }
  const char *command_path[] = {"command", 0};
  yajl_val command_val = yajl_tree_get(parent, command_path, yajl_t_string);

  if (command_val == NULL) {
    fputs("No command key found in client message\n", stderr);
    yajl_tree_free(parent);
    return -1;
  }

  const char *command_name = YAJL_GET_STRING(command_val);
  size_t command_name_len = strlen(command_name);
  parsed_command->name = (char *)malloc((command_name_len + 1) * sizeof(char));
  strcpy(parsed_command->name, command_name);

  DEBUG("Received command: %s\n", parsed_command->name);

  const char *args_path[] = {"args", 0};
  yajl_val args_val = yajl_tree_get(parent, args_path, yajl_t_array);

  if (args_val == NULL) {
    fputs("No args key found in client message\n", stderr);
    yajl_tree_free(parent);
    return -1;
  }

  unsigned int *argc = &parsed_command->argc;
  Arg **args = &parsed_command->args;
  ArgType **arg_types = &parsed_command->arg_types;

  *argc = args_val->u.array.len;

  // If no arguments are specified, make a dummy argument to pass to the
  // function. This is just the way dwm's void(Arg*) functions are setup.
  if (*argc == 0) {
    *args = (Arg *)malloc(sizeof(Arg));
    *arg_types = (ArgType *)malloc(sizeof(ArgType));
    (*arg_types)[0] = ARG_TYPE_NONE;
    (*args)[0].f = 0;
    (*argc)++;
  } else if (*argc > 0) {
    *args = (Arg *)calloc(*argc, sizeof(Arg));
    *arg_types = (ArgType *)malloc(*argc * sizeof(ArgType));

    for (int i = 0; i < *argc; i++) {
      yajl_val arg_val = args_val->u.array.values[i];

      if (YAJL_IS_NUMBER(arg_val)) {
        if (YAJL_IS_INTEGER(arg_val)) {
          // Any values below 0 must be a signed int
          if (YAJL_GET_INTEGER(arg_val) < 0) {
            (*args)[i].i = YAJL_GET_INTEGER(arg_val);
            (*arg_types)[i] = ARG_TYPE_SINT;
            DEBUG("i=%ld\n", (*args)[i].i);
            // Any values above 0 should be an unsigned int
          } else if (YAJL_GET_INTEGER(arg_val) >= 0) {
            (*args)[i].ui = YAJL_GET_INTEGER(arg_val);
            (*arg_types)[i] = ARG_TYPE_UINT;
            DEBUG("ui=%ld\n", (*args)[i].i);
          }
          // If the number is not an integer, it must be a float
        } else {
          (*args)[i].f = (float)YAJL_GET_DOUBLE(arg_val);
          (*arg_types)[i] = ARG_TYPE_FLOAT;
          DEBUG("f=%f\n", (*args)[i].f);
          // If argument is not a number, it must be a string
        }
      } else if (YAJL_IS_STRING(arg_val)) {
        char *arg_s = YAJL_GET_STRING(arg_val);
        size_t arg_s_size = (strlen(arg_s) + 1) * sizeof(char);
        (*args)[i].v = (char *)malloc(arg_s_size);
        (*arg_types)[i] = ARG_TYPE_STR;
        strcpy((char *)(*args)[i].v, arg_s);
      }
    }
  }

  yajl_tree_free(parent);

  return 0;
}

/**
 * Free the members of a IPCParsedCommand struct
 */
static void
ipc_free_parsed_command_members(IPCParsedCommand *command)
{
  for (int i = 0; i < command->argc; i++) {
    if (command->arg_types[i] == ARG_TYPE_STR) free((void *)command->args[i].v);
  }
  free(command->args);
  free(command->arg_types);
  free(command->name);
}

/**
 * Check if the given arguments are the correct length and type. Also do any
 * casting to correct the types.
 *
 * Returns 0 if the arguments were the correct length and types
 * Returns -1 if the argument count doesn't match
 * Returns -2 if the argument types don't match
 */
static int
ipc_validate_run_command(IPCParsedCommand *parsed, const IPCCommand actual)
{
  if (actual.argc != parsed->argc) return -1;

  for (int i = 0; i < parsed->argc; i++) {
    ArgType ptype = parsed->arg_types[i];
    ArgType atype = actual.arg_types[i];

    if (ptype != atype) {
      if (ptype == ARG_TYPE_UINT && atype == ARG_TYPE_PTR)
        // If this argument is supposed to be a void pointer, cast it
        parsed->args[i].v = (void *)parsed->args[i].ui;
      else if (ptype == ARG_TYPE_UINT && atype == ARG_TYPE_SINT)
        // If this argument is supposed to be a signed int, cast it
        parsed->args[i].i = parsed->args[i].ui;
      else
        return -2;
    }
  }

  return 0;
}

/**
 * Convert event name to their IPCEvent equivalent enum value
 *
 * Returns 0 if a valid event name was given
 * Returns -1 otherwise
 */
static int
ipc_event_stoi(const char *subscription, IPCEvent *event)
{
  if (strcmp(subscription, "tag_change_event") == 0)
    *event = IPC_EVENT_TAG_CHANGE;
  else if (strcmp(subscription, "client_focus_change_event") == 0)
    *event = IPC_EVENT_CLIENT_FOCUS_CHANGE;
  else if (strcmp(subscription, "layout_change_event") == 0)
    *event = IPC_EVENT_LAYOUT_CHANGE;
  else if (strcmp(subscription, "monitor_focus_change_event") == 0)
    *event = IPC_EVENT_MONITOR_FOCUS_CHANGE;
  else if (strcmp(subscription, "focused_title_change_event") == 0)
    *event = IPC_EVENT_FOCUSED_TITLE_CHANGE;
  else if (strcmp(subscription, "focused_state_change_event") == 0)
    *event = IPC_EVENT_FOCUSED_STATE_CHANGE;
  else
    return -1;
  return 0;
}

/**
 * Parse a IPC_TYPE_SUBSCRIBE message from a client. This function extracts the
 * event name and the subscription action from the message.
 *
 * Returns 0 if message was successfully parsed
 * Returns -1 otherwise
 */
static int
ipc_parse_subscribe(const char *msg, IPCSubscriptionAction *subscribe,
                    IPCEvent *event)
{
  char error_buffer[100];
  yajl_val parent = yajl_tree_parse((char *)msg, error_buffer, 100);

  if (parent == NULL) {
    fputs("Failed to parse command from client\n", stderr);
    fprintf(stderr, "%s\n", error_buffer);
    return -1;
  }

  // Format:
  // {
  //   "event": "<event name>"
  //   "action": "<subscribe|unsubscribe>"
  // }
  const char *event_path[] = {"event", 0};
  yajl_val event_val = yajl_tree_get(parent, event_path, yajl_t_string);

  if (event_val == NULL) {
    fputs("No 'event' key found in client message\n", stderr);
    return -1;
  }

  const char *event_str = YAJL_GET_STRING(event_val);
  DEBUG("Received event: %s\n", event_str);

  if (ipc_event_stoi(event_str, event) < 0) return -1;

  const char *action_path[] = {"action", 0};
  yajl_val action_val = yajl_tree_get(parent, action_path, yajl_t_string);

  if (action_val == NULL) {
    fputs("No 'action' key found in client message\n", stderr);
    return -1;
  }

  const char *action = YAJL_GET_STRING(action_val);

  if (strcmp(action, "subscribe") == 0)
    *subscribe = IPC_ACTION_SUBSCRIBE;
  else if (strcmp(action, "unsubscribe") == 0)
    *subscribe = IPC_ACTION_UNSUBSCRIBE;
  else {
    fputs("Invalid action specified for subscription\n", stderr);
    return -1;
  }

  yajl_tree_free(parent);

  return 0;
}

/**
 * Parse an IPC_TYPE_GET_DWM_CLIENT message from a client. This function
 * extracts the window id from the message.
 *
 * Returns 0 if message was successfully parsed
 * Returns -1 otherwise
 */
static int
ipc_parse_get_dwm_client(const char *msg, Window *win)
{
  char error_buffer[100];

  yajl_val parent = yajl_tree_parse(msg, error_buffer, 100);

  if (parent == NULL) {
    fputs("Failed to parse message from client\n", stderr);
    fprintf(stderr, "%s\n", error_buffer);
    return -1;
  }

  // Format:
  // {
  //   "client_window_id": <client window id>
  // }
  const char *win_path[] = {"client_window_id", 0};
  yajl_val win_val = yajl_tree_get(parent, win_path, yajl_t_number);

  if (win_val == NULL) {
    fputs("No client window id found in client message\n", stderr);
    return -1;
  }

  *win = YAJL_GET_INTEGER(win_val);

  yajl_tree_free(parent);

  return 0;
}

/**
 * Called when an IPC_TYPE_RUN_COMMAND message is received from a client. This
 * function parses, executes the given command, and prepares a reply message to
 * the client indicating success/failure.
 *
 * NOTE: There is currently no check for argument validity beyond the number of
 * arguments given and types of arguments. There is also no way to check if the
 * function succeeded based on dwm's void(const Arg*) function types. Pointer
 * arguments can cause crashes if they are not validated in the function itself.
 *
 * Returns 0 if message was successfully parsed
 * Returns -1 on failure parsing message
 */
static int
ipc_run_command(IPCClient *ipc_client, char *msg)
{
  IPCParsedCommand parsed_command;
  IPCCommand ipc_command;

  // Initialize struct
  memset(&parsed_command, 0, sizeof(IPCParsedCommand));

  if (ipc_parse_run_command(msg, &parsed_command) < 0) {
    ipc_prepare_reply_failure(ipc_client, IPC_TYPE_RUN_COMMAND,
                              "Failed to parse run command");
    return -1;
  }

  if (ipc_get_ipc_command(parsed_command.name, &ipc_command) < 0) {
    ipc_prepare_reply_failure(ipc_client, IPC_TYPE_RUN_COMMAND,
                              "Command %s not found", parsed_command.name);
    ipc_free_parsed_command_members(&parsed_command);
    return -1;
  }

  int res = ipc_validate_run_command(&parsed_command, ipc_command);
  if (res < 0) {
    if (res == -1)
      ipc_prepare_reply_failure(ipc_client, IPC_TYPE_RUN_COMMAND,
                                "%u arguments provided, %u expected",
                                parsed_command.argc, ipc_command.argc);
    else if (res == -2)
      ipc_prepare_reply_failure(ipc_client, IPC_TYPE_RUN_COMMAND,
                                "Type mismatch");
    ipc_free_parsed_command_members(&parsed_command);
    return -1;
  }

  if (parsed_command.argc == 1)
    ipc_command.func.single_param(parsed_command.args);
  else if (parsed_command.argc > 1)
    ipc_command.func.array_param(parsed_command.args, parsed_command.argc);

  DEBUG("Called function for command %s\n", parsed_command.name);

  ipc_free_parsed_command_members(&parsed_command);

  ipc_prepare_reply_success(ipc_client, IPC_TYPE_RUN_COMMAND);
  return 0;
}

/**
 * Called when an IPC_TYPE_GET_MONITORS message is received from a client. It
 * prepares a reply with the properties of all of the monitors in JSON.
 */
static void
ipc_get_monitors(IPCClient *c, Monitor *mons, Monitor *selmon)
{
  yajl_gen gen;
  ipc_reply_init_message(&gen);
  dump_monitors(gen, mons, selmon);

  ipc_reply_prepare_send_message(gen, c, IPC_TYPE_GET_MONITORS);
}

/**
 * Called when an IPC_TYPE_GET_TAGS message is received from a client. It
 * prepares a reply with info about all the tags in JSON.
 */
static void
ipc_get_tags(IPCClient *c, const int tags_len)
{
  yajl_gen gen;
  ipc_reply_init_message(&gen);

  dump_tags(gen, tags_len);

  ipc_reply_prepare_send_message(gen, c, IPC_TYPE_GET_TAGS);
}

/**
 * Called when an IPC_TYPE_GET_LAYOUTS message is received from a client. It
 * prepares a reply with a JSON array of available layouts
 */
static void
ipc_get_layouts(IPCClient *c, const Layout layouts[], const int layouts_len)
{
  yajl_gen gen;
  ipc_reply_init_message(&gen);

  dump_layouts(gen, layouts, layouts_len);

  ipc_reply_prepare_send_message(gen, c, IPC_TYPE_GET_LAYOUTS);
}

/**
 * Called when an IPC_TYPE_GET_DWM_CLIENT message is received from a client. It
 * prepares a JSON reply with the properties of the client with the specified
 * window XID.
 *
 * Returns 0 if the message was successfully parsed and if the client with the
 *   specified window XID was found
 * Returns -1 if the message could not be parsed
 */
static int
ipc_get_dwm_client(IPCClient *ipc_client, const char *msg, const Monitor *mons)
{
  Window win;

  if (ipc_parse_get_dwm_client(msg, &win) < 0) return -1;

  // Find client with specified window XID
  for (const Monitor *m = mons; m; m = m->next)
    for (Client *c = m->clients; c; c = c->next)
      if (c->win == win) {
        yajl_gen gen;
        ipc_reply_init_message(&gen);

        dump_client(gen, c);

        ipc_reply_prepare_send_message(gen, ipc_client,
                                       IPC_TYPE_GET_DWM_CLIENT);

        return 0;
      }

  ipc_prepare_reply_failure(ipc_client, IPC_TYPE_GET_DWM_CLIENT,
                            "Client with window id %d not found", win);
  return -1;
}

/**
 * Called when an IPC_TYPE_SUBSCRIBE message is received from a client. It
 * subscribes/unsubscribes the client from the specified event and replies with
 * the result.
 *
 * Returns 0 if the message was successfully parsed.
 * Returns -1 if the message could not be parsed
 */
static int
ipc_subscribe(IPCClient *c, const char *msg)
{
  IPCSubscriptionAction action = IPC_ACTION_SUBSCRIBE;
  IPCEvent event = 0;

  if (ipc_parse_subscribe(msg, &action, &event)) {
    ipc_prepare_reply_failure(c, IPC_TYPE_SUBSCRIBE, "Event does not exist");
    return -1;
  }

  if (action == IPC_ACTION_SUBSCRIBE) {
    DEBUG("Subscribing client on fd %d to %d\n", c->fd, event);
    c->subscriptions |= event;
  } else if (action == IPC_ACTION_UNSUBSCRIBE) {
    DEBUG("Unsubscribing client on fd %d to %d\n", c->fd, event);
    c->subscriptions ^= event;
  } else {
    ipc_prepare_reply_failure(c, IPC_TYPE_SUBSCRIBE,
                              "Invalid subscription action");
    return -1;
  }

  ipc_prepare_reply_success(c, IPC_TYPE_SUBSCRIBE);
  return 0;
}

int
ipc_init(const char *socket_path, const int p_epoll_fd, IPCCommand commands[],
         const int commands_len)
{
  // Initialize struct to 0
  memset(&sock_epoll_event, 0, sizeof(sock_epoll_event));

  int socket_fd = ipc_create_socket(socket_path);
  if (socket_fd < 0) return -1;

  ipc_commands = commands;
  ipc_commands_len = commands_len;

  epoll_fd = p_epoll_fd;

  // Wake up to incoming connection requests
  sock_epoll_event.data.fd = socket_fd;
  sock_epoll_event.events = EPOLLIN;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &sock_epoll_event)) {
    fputs("Failed to add sock file descriptor to epoll", stderr);
    return -1;
  }

  return socket_fd;
}

void
ipc_cleanup()
{
  IPCClient *c = ipc_clients;
  // Free clients and their buffers
  while (c) {
    ipc_drop_client(c);
    c = ipc_clients;
  }

  // Stop waking up for socket events
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock_fd, &sock_epoll_event);

  // Uninitialize all static variables
  epoll_fd = -1;
  sock_fd = -1;
  ipc_commands = NULL;
  ipc_commands_len = 0;
  memset(&sock_epoll_event, 0, sizeof(struct epoll_event));
  memset(&sockaddr, 0, sizeof(struct sockaddr_un));

  // Delete socket
  unlink(sockaddr.sun_path);

  shutdown(sock_fd, SHUT_RDWR);
  close(sock_fd);
}

int
ipc_get_sock_fd()
{
  return sock_fd;
}

IPCClient *
ipc_get_client(int fd)
{
  return ipc_list_get_client(ipc_clients, fd);
}

int
ipc_is_client_registered(int fd)
{
  return (ipc_get_client(fd) != NULL);
}

int
ipc_accept_client()
{
  int fd = -1;

  struct sockaddr_un client_addr;
  socklen_t len = 0;

  // For portability clear the addr structure, since some implementations
  // have nonstandard fields in the structure
  memset(&client_addr, 0, sizeof(struct sockaddr_un));

  fd = accept(sock_fd, (struct sockaddr *)&client_addr, &len);
  if (fd < 0 && errno != EINTR) {
    fputs("Failed to accept IPC connection from client", stderr);
    return -1;
  }

  if (fcntl(fd, F_SETFD, FD_CLOEXEC) < 0) {
    shutdown(fd, SHUT_RDWR);
    close(fd);
    fputs("Failed to set flags on new client fd", stderr);
  }

  IPCClient *nc = ipc_client_new(fd);
  if (nc == NULL) return -1;

  // Wake up to messages from this client
  nc->event.data.fd = fd;
  nc->event.events = EPOLLIN | EPOLLHUP;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &nc->event);

  ipc_list_add_client(&ipc_clients, nc);

  DEBUG("%s%d\n", "New client at fd: ", fd);

  return fd;
}

int
ipc_drop_client(IPCClient *c)
{
  int fd = c->fd;
  shutdown(fd, SHUT_RDWR);
  int res = close(fd);

  if (res == 0) {
    struct epoll_event ev;

    // Stop waking up to messages from this client
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
    ipc_list_remove_client(&ipc_clients, c);

    free(c->buffer);
    free(c);

    DEBUG("Successfully removed client on fd %d\n", fd);
  } else if (res < 0 && res != EINTR) {
    fprintf(stderr, "Failed to close fd %d\n", fd);
  }

  return res;
}

int
ipc_read_client(IPCClient *c, IPCMessageType *msg_type, uint32_t *msg_size,
                char **msg)
{
  int fd = c->fd;
  int ret =
      ipc_recv_message(fd, (uint8_t *)msg_type, msg_size, (uint8_t **)msg);

  if (ret < 0) {
    // This will happen if these errors occur while reading header
    if (ret == -1 &&
        (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
      return -2;

    fprintf(stderr, "Error reading message: dropping client at fd %d\n", fd);
    ipc_drop_client(c);

    return -1;
  }

  // Make sure receive message is null terminated to avoid parsing issues
  if (*msg_size > 0) {
    size_t len = *msg_size;
    nullterminate(msg, &len);
    *msg_size = len;
  }

  DEBUG("[fd %d] ", fd);
  if (*msg_size > 0)
    DEBUG("Received message: '%.*s' ", *msg_size, *msg);
  else
    DEBUG("Received empty message ");
  DEBUG("Message type: %" PRIu8 " ", (uint8_t)*msg_type);
  DEBUG("Message size: %" PRIu32 "\n", *msg_size);

  return 0;
}

ssize_t
ipc_write_client(IPCClient *c)
{
  const ssize_t n = ipc_write_message(c->fd, c->buffer, c->buffer_size);

  if (n < 0) return n;

  // TODO: Deal with client timeouts

  if (n == c->buffer_size) {
    c->buffer_size = 0;
    free(c->buffer);
    // No dangling pointers!
    c->buffer = NULL;
    // Stop waking up when client is ready to receive messages
    if (c->event.events & EPOLLOUT) {
      c->event.events -= EPOLLOUT;
      epoll_ctl(epoll_fd, EPOLL_CTL_MOD, c->fd, &c->event);
    }
    return n;
  }

  // Shift unwritten buffer to beginning of buffer and reallocate
  c->buffer_size -= n;
  memmove(c->buffer, c->buffer + n, c->buffer_size);
  c->buffer = (char *)realloc(c->buffer, c->buffer_size);

  return n;
}

void
ipc_prepare_send_message(IPCClient *c, const IPCMessageType msg_type,
                         const uint32_t msg_size, const char *msg)
{
  dwm_ipc_header_t header = {
      .magic = IPC_MAGIC_ARR, .type = msg_type, .size = msg_size};

  uint32_t header_size = sizeof(dwm_ipc_header_t);
  uint32_t packet_size = header_size + msg_size;

  if (c->buffer == NULL)
    c->buffer = (char *)malloc(c->buffer_size + packet_size);
  else
    c->buffer = (char *)realloc(c->buffer, c->buffer_size + packet_size);

  // Copy header to end of client buffer
  memcpy(c->buffer + c->buffer_size, &header, header_size);
  c->buffer_size += header_size;

  // Copy message to end of client buffer
  memcpy(c->buffer + c->buffer_size, msg, msg_size);
  c->buffer_size += msg_size;

  // Wake up when client is ready to receive messages
  c->event.events |= EPOLLOUT;
  epoll_ctl(epoll_fd, EPOLL_CTL_MOD, c->fd, &c->event);
}

void
ipc_prepare_reply_failure(IPCClient *c, IPCMessageType msg_type,
                          const char *format, ...)
{
  yajl_gen gen;
  va_list args;

  // Get output size
  va_start(args, format);
  size_t len = vsnprintf(NULL, 0, format, args);
  va_end(args);
  char *buffer = (char *)malloc((len + 1) * sizeof(char));

  ipc_reply_init_message(&gen);

  va_start(args, format);
  vsnprintf(buffer, len + 1, format, args);
  va_end(args);
  dump_error_message(gen, buffer);

  ipc_reply_prepare_send_message(gen, c, msg_type);
  fprintf(stderr, "[fd %d] Error: %s\n", c->fd, buffer);

  free(buffer);
}

void
ipc_prepare_reply_success(IPCClient *c, IPCMessageType msg_type)
{
  const char *success_msg = "{\"result\":\"success\"}";
  const size_t msg_len = strlen(success_msg) + 1;  // +1 for null char

  ipc_prepare_send_message(c, msg_type, msg_len, success_msg);
}

void
ipc_tag_change_event(int mon_num, TagState old_state, TagState new_state)
{
  yajl_gen gen;
  ipc_event_init_message(&gen);
  dump_tag_event(gen, mon_num, old_state, new_state);
  ipc_event_prepare_send_message(gen, IPC_EVENT_TAG_CHANGE);
}

void
ipc_client_focus_change_event(int mon_num, Client *old_client,
                              Client *new_client)
{
  yajl_gen gen;
  ipc_event_init_message(&gen);
  dump_client_focus_change_event(gen, old_client, new_client, mon_num);
  ipc_event_prepare_send_message(gen, IPC_EVENT_CLIENT_FOCUS_CHANGE);
}

void
ipc_layout_change_event(const int mon_num, const char *old_symbol,
                        const Layout *old_layout, const char *new_symbol,
                        const Layout *new_layout)
{
  yajl_gen gen;
  ipc_event_init_message(&gen);
  dump_layout_change_event(gen, mon_num, old_symbol, old_layout, new_symbol,
                           new_layout);
  ipc_event_prepare_send_message(gen, IPC_EVENT_LAYOUT_CHANGE);
}

void
ipc_monitor_focus_change_event(const int last_mon_num, const int new_mon_num)
{
  yajl_gen gen;
  ipc_event_init_message(&gen);
  dump_monitor_focus_change_event(gen, last_mon_num, new_mon_num);
  ipc_event_prepare_send_message(gen, IPC_EVENT_MONITOR_FOCUS_CHANGE);
}

void
ipc_focused_title_change_event(const int mon_num, const Window client_id,
                               const char *old_name, const char *new_name)
{
  yajl_gen gen;
  ipc_event_init_message(&gen);
  dump_focused_title_change_event(gen, mon_num, client_id, old_name, new_name);
  ipc_event_prepare_send_message(gen, IPC_EVENT_FOCUSED_TITLE_CHANGE);
}

void
ipc_focused_state_change_event(const int mon_num, const Window client_id,
                               const ClientState *old_state,
                               const ClientState *new_state)
{
  yajl_gen gen;
  ipc_event_init_message(&gen);
  dump_focused_state_change_event(gen, mon_num, client_id, old_state,
                                  new_state);
  ipc_event_prepare_send_message(gen, IPC_EVENT_FOCUSED_STATE_CHANGE);
}

void
ipc_send_events(Monitor *mons, Monitor **lastselmon, Monitor *selmon)
{
  for (Monitor *m = mons; m; m = m->next) {
    unsigned int urg = 0, occ = 0, tagset = 0;

    for (Client *c = m->clients; c; c = c->next) {
      occ |= c->tags;

      if (c->isurgent) urg |= c->tags;
    }
    tagset = m->tagset[m->seltags];

    TagState new_state = {.selected = tagset, .occupied = occ, .urgent = urg};

    if (memcmp(&m->tagstate, &new_state, sizeof(TagState)) != 0) {
      ipc_tag_change_event(m->num, m->tagstate, new_state);
      m->tagstate = new_state;
    }

    if (m->lastsel != m->sel) {
      ipc_client_focus_change_event(m->num, m->lastsel, m->sel);
      m->lastsel = m->sel;
    }

    if (strcmp(m->ltsymbol, m->lastltsymbol) != 0 ||
        m->lastlt != m->lt[m->sellt]) {
      ipc_layout_change_event(m->num, m->lastltsymbol, m->lastlt, m->ltsymbol,
                              m->lt[m->sellt]);
      strcpy(m->lastltsymbol, m->ltsymbol);
      m->lastlt = m->lt[m->sellt];
    }

    if (*lastselmon != selmon) {
      if (*lastselmon != NULL)
        ipc_monitor_focus_change_event((*lastselmon)->num, selmon->num);
      *lastselmon = selmon;
    }

    Client *sel = m->sel;
    if (!sel) continue;
    ClientState *o = &m->sel->prevstate;
    ClientState n = {.oldstate = sel->oldstate,
                     .isfixed = sel->isfixed,
                     .isfloating = sel->isfloating,
                     .isfullscreen = sel->isfullscreen,
                     .isurgent = sel->isurgent,
                     .neverfocus = sel->neverfocus};
    if (memcmp(o, &n, sizeof(ClientState)) != 0) {
      ipc_focused_state_change_event(m->num, m->sel->win, o, &n);
      *o = n;
    }
  }
}

int
ipc_handle_client_epoll_event(struct epoll_event *ev, Monitor *mons,
                              Monitor **lastselmon, Monitor *selmon, const int tags_len,
                              const Layout *layouts, const int layouts_len)
{
  int fd = ev->data.fd;
  IPCClient *c = ipc_get_client(fd);

  if (ev->events & EPOLLHUP) {
    DEBUG("EPOLLHUP received from client at fd %d\n", fd);
    ipc_drop_client(c);
  } else if (ev->events & EPOLLOUT) {
    DEBUG("Sending message to client at fd %d...\n", fd);
    if (c->buffer_size) ipc_write_client(c);
  } else if (ev->events & EPOLLIN) {
    IPCMessageType msg_type = 0;
    uint32_t msg_size = 0;
    char *msg = NULL;

    DEBUG("Received message from fd %d\n", fd);
    if (ipc_read_client(c, &msg_type, &msg_size, &msg) < 0) return -1;

    if (msg_type == IPC_TYPE_GET_MONITORS)
      ipc_get_monitors(c, mons, selmon);
    else if (msg_type == IPC_TYPE_GET_TAGS)
      ipc_get_tags(c, tags_len);
    else if (msg_type == IPC_TYPE_GET_LAYOUTS)
      ipc_get_layouts(c, layouts, layouts_len);
    else if (msg_type == IPC_TYPE_RUN_COMMAND) {
      if (ipc_run_command(c, msg) < 0) return -1;
      ipc_send_events(mons, lastselmon, selmon);
    } else if (msg_type == IPC_TYPE_GET_DWM_CLIENT) {
      if (ipc_get_dwm_client(c, msg, mons) < 0) return -1;
    } else if (msg_type == IPC_TYPE_SUBSCRIBE) {
      if (ipc_subscribe(c, msg) < 0) return -1;
    } else {
      fprintf(stderr, "Invalid message type received from fd %d", fd);
      ipc_prepare_reply_failure(c, msg_type, "Invalid message type: %d",
                                msg_type);
    }
    free(msg);
  } else {
    fprintf(stderr, "Epoll event returned %d from fd %d\n", ev->events, fd);
    return -1;
  }

  return 0;
}

int
ipc_handle_socket_epoll_event(struct epoll_event *ev)
{
  if (!(ev->events & EPOLLIN)) return -1;

  // EPOLLIN means incoming client connection request
  fputs("Received EPOLLIN event on socket\n", stderr);
  int new_fd = ipc_accept_client();

  return new_fd;
}

