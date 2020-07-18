static int width_status2d(Bar *bar, BarWidthArg *a);
static int draw_status2d(Bar *bar, BarDrawArg *a);
#if !BAR_STATUSCMD_PATCH
static int click_status2d(Bar *bar, Arg *arg, BarClickArg *a);
#endif // BAR_STATUSCMD_PATCH
static int drawstatusbar(int x, char* text);
static int status2dtextlength(char* stext);