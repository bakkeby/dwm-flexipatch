static int width_status(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_status_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_status(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_status_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int click_status(Bar *bar, Arg *arg, BarArg *a);

