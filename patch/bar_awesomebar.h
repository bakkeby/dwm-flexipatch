static int width_awesomebar(Monitor *m, BarWidthArg *a);
static int draw_awesomebar(Monitor *m, BarDrawArg *a);
static int click_awesomebar(Monitor *m, Arg *arg, BarClickArg *a);

static void hide(Client *c);
static void show(Client *c);
static void togglewin(const Arg *arg);
static void showhideclient(const Arg *arg);