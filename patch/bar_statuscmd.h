static int click_statuscmd(Monitor *m, Arg *arg, BarClickArg *a);
#if BAR_EXTRABAR_PATCH
static int click_statuscmd_eb(Monitor *m, Arg *arg, BarClickArg *a);
#endif // BAR_EXTRABAR_PATCH
static int click_statuscmd_text(Monitor *m, Arg *arg, int rel_x, char *text);
static void copyvalidchars(char *text, char *rawtext);