static int width_status(Bar *bar, BarWidthArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_status_es(Bar *bar, BarWidthArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_status(Bar *bar, BarDrawArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_status_es(Bar *bar, BarDrawArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int click_status(Bar *bar, Arg *arg, BarClickArg *a);