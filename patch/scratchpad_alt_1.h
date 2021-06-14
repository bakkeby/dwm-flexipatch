#define SCRATCHPAD_MASK (1u << NUMTAGS)

static void scratchpad_hide();
static _Bool scratchpad_last_showed_is_killed(void);
static void scratchpad_remove();
static void scratchpad_show();
static void scratchpad_show_client(Client *c);
static void scratchpad_show_first(void);

