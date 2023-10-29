static int width_pwrl_status(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_pwrl_status_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_pwrl_status(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_pwrl_status_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int click_pwrl_status(Bar *bar, Arg *arg, BarArg *a);
static int drawpowerlinestatus(int x, char *stext, BarArg *a);
static int widthpowerlinestatus(char *stext);

