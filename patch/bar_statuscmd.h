static int click_statuscmd(Bar *bar, Arg *arg, BarClickArg *a);
#if BAR_EXTRABAR_PATCH
static int click_statuscmd_eb(Bar *bar, Arg *arg, BarClickArg *a);
#endif // BAR_EXTRABAR_PATCH
static int click_statuscmd_text(Arg *arg, int rel_x, char *text);
static void copyvalidchars(char *text, char *rawtext);