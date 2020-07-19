static Clr **statusscheme;

int
width_pwrl_status(Bar *bar, BarWidthArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return widthpowerlinestatus(rawstext);
	#else
	return widthpowerlinestatus(stext);
	#endif // BAR_STATUSCMD_PATCH
}

#if BAR_EXTRASTATUS_PATCH
int
width_pwrl_status_es(Bar *bar, BarWidthArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return widthpowerlinestatus(rawestext);
	#else
	return widthpowerlinestatus(estext);
	#endif // BAR_STATUSCMD_PATCH
}
#endif // BAR_EXTRASTATUS_PATCH

int
draw_pwrl_status(Bar *bar, BarDrawArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return drawpowerlinestatus(a->x + a->w, rawstext);
	#else
	return drawpowerlinestatus(a->x + a->w, stext);
	#endif // BAR_STATUSCMD_PATCH
}

#if BAR_EXTRASTATUS_PATCH
int
draw_pwrl_status_es(Bar *bar, BarDrawArg *a)
{
	#if BAR_STATUSCMD_PATCH
	return drawpowerlinestatus(a->x + a->w, rawestext);
	#else
	return drawpowerlinestatus(a->x + a->w, estext);
	#endif // BAR_STATUSCMD_PATCH
}
#endif // BAR_EXTRASTATUS_PATCH

int
click_pwrl_status(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkStatusText;
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
drawpowerlinestatus(int xpos, char *stext)
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
			cn = ((int) *(bs+1)) - 1;

			if (cn < LENGTH(statuscolors)) {
				drw_settrans(drw, prevscheme, (nxtscheme = statusscheme[cn]));
			} else {
				drw_settrans(drw, prevscheme, (nxtscheme = statusscheme[0]));
			}

			if (bp != '|') {
				drw_arrow(drw, x - plw, 0, plw, bh, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
				x -= plw;
			}

			drw_setscheme(drw, nxtscheme);
			w = TEXTW(bs+2);
			drw_text(drw, x - w, 0, w, bh, lrpad / 2, bs+2, 0);
			x -= w;

			bp = *bs;
			*bs = 0;
			prevscheme = nxtscheme;
		}
	}
	if (bp != '|') {
		drw_settrans(drw, prevscheme, scheme[SchemeNorm]);
		drw_arrow(drw, x - plw, 0, plw, bh, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
		drw_rect(drw, x - 2 * plw, 0, plw, bh, 1, 1);
		x -= plw * 2;
	}

	return xpos - x;
}