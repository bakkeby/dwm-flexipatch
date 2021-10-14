static Clr **statusscheme;

int
width_pwrl_status(Bar *bar, BarArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return widthpowerlinestatus(rawstext);
	#else
	return widthpowerlinestatus(stext);
	#endif // BAR_STATUSCMD_PATCH
}

#if BAR_EXTRASTATUS_PATCH
int
width_pwrl_status_es(Bar *bar, BarArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return widthpowerlinestatus(rawestext);
	#else
	return widthpowerlinestatus(estext);
	#endif // BAR_STATUSCMD_PATCH
}
#endif // BAR_EXTRASTATUS_PATCH

int
draw_pwrl_status(Bar *bar, BarArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return drawpowerlinestatus(a->x + a->w, rawstext, a);
	#else
	return drawpowerlinestatus(a->x + a->w, stext, a);
	#endif // BAR_STATUSCMD_PATCH
}

#if BAR_EXTRASTATUS_PATCH
int
draw_pwrl_status_es(Bar *bar, BarArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return drawpowerlinestatus(a->x + a->w, rawestext, a);
	#else
	return drawpowerlinestatus(a->x + a->w, estext, a);
	#endif // BAR_STATUSCMD_PATCH
}
#endif // BAR_EXTRASTATUS_PATCH

#if BAR_STATUSCMD_PATCH
int
click_pwrl_status_text(Arg *arg, int rel_x, char *text)
{
	int delimPad = drw->fonts->h;
	int i = -1;
	int x = delimPad / 2 - (TEXTW("_") - lrpad);
	char ch;
	#if BAR_DWMBLOCKS_PATCH
	statussig = -1;
	#else
	statuscmdn = 0;
	#endif // BAR_DWMBLOCKS_PATCH
	while (text[++i]) {
		if ((unsigned char)text[i] < ' ') {
			if (text[i] <= LENGTH(statuscolors))
				continue;
			ch = text[i];
			text[i] = '\0';
			x += TEXTW(text) - lrpad + delimPad;
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
#endif // BAR_STATUSCMD_PATCH

int
click_pwrl_status(Bar *bar, Arg *arg, BarArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return click_pwrl_status_text(arg, a->x, rawstext);
	#else
	return ClkStatusText;
	#endif
}

int
widthpowerlinestatus(char *stext)
{
	char status[512];
	int w = 0, i, n = strlen(stext);
	int plw = drw->fonts->h / 2 + 1;
	char *bs, bp = '|';
	strcpy(status, stext);

	for (i = n, bs = &status[n-1]; i >= 0; i--, bs--) {
		if (*bs == '<' || *bs == '/' || *bs == '\\' || *bs == '>' || *bs == '|') { /* block start */
			if (bp != '|')
				w += plw;
			w += TEXTW(bs+2);
			bp = *bs;
			*bs = 0;
		}
	}
	if (bp != '|')
		w += plw * 2;

	return w;
}

int
drawpowerlinestatus(int xpos, char *stext, BarArg *barg)
{
	char status[512];
	int i, n = strlen(stext), cn = 0;
	int x = xpos, w = 0;
	int plw = drw->fonts->h / 2 + 1;
	char *bs, bp = '|';
	Clr *prevscheme = statusscheme[0], *nxtscheme;
	strcpy(status, stext);

	for (i = n, bs = &status[n-1]; i >= 0; i--, bs--) {
		if (*bs == '<' || *bs == '/' || *bs == '\\' || *bs == '>' || *bs == '|') { /* block start */
			#if BAR_STATUSCMD_PATCH
			int textOffset = 3;
			#else
			int textOffset = 2;
			#endif

			cn = *(bs+textOffset - 1) - 1;
			if (cn < LENGTH(statuscolors)) {
				drw_settrans(drw, prevscheme, (nxtscheme = statusscheme[cn]));
			} else {
				textOffset--;
				drw_settrans(drw, prevscheme, (nxtscheme = statusscheme[0]));
			}

			if (bp != '|') {
				drw_arrow(drw, x - plw, barg->y, plw, barg->h, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
				x -= plw;
			}

			drw_setscheme(drw, nxtscheme);
			w = TEXTW(bs+textOffset);
			drw_text(drw, x - w, barg->y, w, barg->h, lrpad / 2, bs+textOffset, 0, False);
			x -= w;

			bp = *bs;
			*bs = 0;
			prevscheme = nxtscheme;
		}
	}
	if (bp != '|') {
		drw_settrans(drw, prevscheme, scheme[SchemeNorm]);
		drw_arrow(drw, x - plw, barg->y, plw, barg->h, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
		drw_rect(drw, x - 2 * plw, barg->y, plw, barg->h, 1, 1);
		x -= plw * 2;
	}

	return xpos - x;
}
