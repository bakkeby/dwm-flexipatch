static int width_bartabgroups(Bar *bar, BarWidthArg *a);
static int draw_bartabgroups(Bar *bar, BarDrawArg *a);
static int click_bartabgroups(Bar *bar, Arg *arg, BarClickArg *a);

static void bartabdraw(Monitor *m, Client *c, int unused, int x, int w, int groupactive, Arg *arg);
static void bartabclick(Monitor *m, Client *c, int passx, int x, int w, int unused, Arg *arg);
static void bartabcalculate(Monitor *m, int offx, int w, int passx, void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg), Arg *arg);