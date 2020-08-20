static int width_wintitle_hidden(Bar *bar, BarWidthArg *a);
static int draw_wintitle_hidden(Bar *bar, BarDrawArg *a);
static int click_wintitle_hidden(Bar *bar, Arg *arg, BarClickArg *a);
static void calc_wintitle_hidden(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg), Arg *arg
);