static int width_awesomebar(Bar *bar, BarWidthArg *a);
static int draw_awesomebar(Bar *bar, BarDrawArg *a);
static int click_awesomebar(Bar *bar, Arg *arg, BarClickArg *a);

static void hide(Client *c);
static void show(Client *c);
static void togglewin(const Arg *arg);
static Client *prevtiled(Client *c);
static void showhideclient(const Arg *arg);