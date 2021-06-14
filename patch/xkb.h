static XkbInfo *createxkb(Window w);
static XkbInfo *findxkb(Window w);
static void xkbeventnotify(XEvent *e);

static int width_xkb(Bar *bar, BarArg *a);
static int draw_xkb(Bar *bar, BarArg *a);
static int click_xkb(Bar *bar, Arg *arg, BarArg *a);

