static int width_status2d(Monitor *m, BarWidthArg *a);
static int draw_status2d(Monitor *m, BarDrawArg *a);
#if !BAR_DWMBLOCKS_PATCH
static int click_status2d(Monitor *m, Arg *arg, BarClickArg *a);
#endif // BAR_DWMBLOCKS_PATCH
static int drawstatusbar(Monitor *m, int x, int w, char* text);
static int status2dtextlength(char* stext);