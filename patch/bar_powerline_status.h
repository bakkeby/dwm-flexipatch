static int width_pwrl_status(Bar *bar, BarWidthArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_pwrl_status_es(Bar *bar, BarWidthArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_pwrl_status(Bar *bar, BarDrawArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_pwrl_status_es(Bar *bar, BarDrawArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int click_pwrl_status(Bar *bar, Arg *arg, BarClickArg *a);
static int drawpowerlinestatus(int x, char *stext);
static int widthpowerlinestatus(char *stext);