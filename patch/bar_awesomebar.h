static int width_awesomebar(Monitor *m, int max_width);
static int draw_awesomebar(Monitor *m, int x, int w);
static int click_awesomebar(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h);

static void hide(Client *c);
static void show(Client *c);
static void togglewin(const Arg *arg);
static void showhideclient(const Arg *arg);