static int click_statuscmd(Bar *bar, Arg *arg, BarArg *a);
#if BAR_EXTRASTATUS_PATCH
static int click_statuscmd_es(Bar *bar, Arg *arg, BarArg *a);
#endif // BAR_EXTRASTATUS_PATCH
static int click_statuscmd_text(Arg *arg, int rel_x, char *text);
static void copyvalidchars(char *text, char *rawtext);

typedef struct {
	const char *cmd;
	int id;
} StatusCmd;

