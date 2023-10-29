static int width_status2d(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_status2d_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_status2d(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_status2d_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
#if !BAR_STATUSCMD_PATCH
static int click_status2d(Bar *bar, Arg *arg, BarArg *a);
#endif // BAR_STATUSCMD_PATCH
static int drawstatusbar(BarArg *a, char *text);
static int status2dtextlength(char *stext);

