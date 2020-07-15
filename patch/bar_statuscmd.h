static int click_statuscmd(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h);
#if BAR_EXTRABAR_PATCH
static int click_statuscmd_eb(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h);
#endif // BAR_EXTRABAR_PATCH
static int click_statuscmd_text(Monitor *m, Arg *arg, int rel_x, int rel_y, char *text);
static void copyvalidchars(char *text, char *rawtext);