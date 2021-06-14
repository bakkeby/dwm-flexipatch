#if !BAR_DWMBLOCKS_PATCH
static const char statusexport[] = "export BUTTON=-;";
static int statuscmdn;
static char lastbutton[] = "-";
#endif // BAR_DWMBLOCKS_PATCH

int
click_statuscmd(Bar *bar, Arg *arg, BarArg *a)
{
	return click_statuscmd_text(arg, a->x, rawstext);
}

#if BAR_EXTRASTATUS_PATCH
int
click_statuscmd_es(Bar *bar, Arg *arg, BarArg *a)
{
	return click_statuscmd_text(arg, a->x, rawestext);
}
#endif // BAR_EXTRASTATUS_PATCH

int
click_statuscmd_text(Arg *arg, int rel_x, char *text)
{
	int i = -1;
	int x = 0;
	char ch;
	#if BAR_DWMBLOCKS_PATCH
	statussig = -1;
	#else
	statuscmdn = 0;
	#endif // BAR_DWMBLOCKS_PATCH
	while (text[++i]) {
		if ((unsigned char)text[i] < ' ') {
			ch = text[i];
			text[i] = '\0';
			#if BAR_STATUS2D_PATCH && !BAR_BAR_STATUSCOLORS_PATCH
			x += status2dtextlength(text);
			#else
			x += TEXTWM(text) - lrpad;
			#endif // BAR_STATUS2D_PATCH
			text[i] = ch;
			text += i+1;
			i = -1;
			#if BAR_DWMBLOCKS_PATCH
			if (x >= rel_x && statussig != -1)
				break;
			statussig = ch;
			#else
			if (x >= rel_x)
				break;
			if (ch <= LENGTH(statuscmds))
				statuscmdn = ch;
			#endif // BAR_DWMBLOCKS_PATCH
		}
	}
	#if BAR_DWMBLOCKS_PATCH
	if (statussig == -1)
		statussig = 0;
	#endif // BAR_DWMBLOCKS_PATCH
	return ClkStatusText;
}

void
copyvalidchars(char *text, char *rawtext)
{
	int i = -1, j = 0;

	while (rawtext[++i]) {
		if ((unsigned char)rawtext[i] >= ' ') {
			text[j++] = rawtext[i];
		}
	}
	text[j] = '\0';
}

