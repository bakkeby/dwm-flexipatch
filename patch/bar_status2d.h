static int width_status2d(Bar *bar, BarWidthArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_status2d_es(Bar *bar, BarWidthArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_status2d(Bar *bar, BarDrawArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_status2d_es(Bar *bar, BarDrawArg *a);
#endif // BAR_EXTRASTATUS_PATCH
#if !BAR_STATUSCMD_PATCH
static int click_status2d(Bar *bar, Arg *arg, BarClickArg *a);
#endif // BAR_STATUSCMD_PATCH
static int drawstatusbar(int x, char* text);
static int status2dtextlength(char* stext);