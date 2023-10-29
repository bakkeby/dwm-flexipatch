static int width_statuscolors(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int width_statuscolors_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int draw_statuscolors(Bar *bar, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int draw_statuscolors_es(Bar *bar, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
#if !BAR_STATUSCMD_PATCH
static int click_statuscolors(Bar *bar, Arg *arg, BarArg *a);
#endif // BAR_STATUSCMD_PATCH
static int textw_wosc(char *s);
static int draw_wosc(Bar *bar, BarArg *a, char *s);