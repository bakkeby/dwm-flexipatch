static int width_bartabgroups(Bar *bar, BarArg *a);
static int draw_bartabgroups(Bar *bar, BarArg *a);
static int click_bartabgroups(Bar *bar, Arg *arg, BarArg *a);

static void bartabdraw(Monitor *m, Client *c, int unused, int x, int w, int groupactive, Arg *arg, BarArg *barg);
static void bartabclick(Monitor *m, Client *c, int passx, int x, int w, int unused, Arg *arg, BarArg *barg);
static int bartabcalculate(Monitor *m, int offx, int w, int passx, void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg, BarArg *barg), Arg *arg, BarArg *barg);

