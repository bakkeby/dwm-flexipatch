#include "IPCClient.h"

#include <string.h>
#include <sys/epoll.h>

#include "util.h"

IPCClient *
ipc_client_new(int fd)
{
  IPCClient *c = (IPCClient *)malloc(sizeof(IPCClient));

  if (c == NULL) return NULL;

  // Initialize struct
  memset(&c->event, 0, sizeof(struct epoll_event));

  c->buffer_size = 0;
  c->buffer = NULL;
  c->fd = fd;
  c->event.data.fd = fd;
  c->next = NULL;
  c->prev = NULL;
  c->subscriptions = 0;

  return c;
}

void
ipc_list_add_client(IPCClientList *list, IPCClient *nc)
{
  DEBUG("Adding client with fd %d to list\n", nc->fd);

  if (*list == NULL) {
    // List is empty, point list at first client
    *list = nc;
  } else {
    IPCClient *c;
    // Go to last client in list
    for (c = *list; c && c->next; c = c->next)
      ;
    c->next = nc;
    nc->prev = c;
  }
}

void
ipc_list_remove_client(IPCClientList *list, IPCClient *c)
{
  IPCClient *cprev = c->prev;
  IPCClient *cnext = c->next;

  if (cprev != NULL) cprev->next = c->next;
  if (cnext != NULL) cnext->prev = c->prev;
  if (c == *list) *list = c->next;
}

IPCClient *
ipc_list_get_client(IPCClientList list, int fd)
{
  for (IPCClient *c = list; c; c = c->next) {
    if (c->fd == fd) return c;
  }

  return NULL;
}

