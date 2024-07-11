typedef struct {
	char* name;
	const Arg command;
} Launcher;

static int width_launcher(Bar *bar, BarArg *a);
static int draw_launcher(Bar *bar, BarArg *a);
static int click_launcher(Bar *bar, Arg *arg, BarArg *a);
