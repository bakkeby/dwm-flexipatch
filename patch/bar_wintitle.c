int
width_wintitle(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_wintitle(Bar *bar, BarArg *a)
{
	#if BAR_TITLE_LEFT_PAD_PATCH && BAR_TITLE_RIGHT_PAD_PATCH
	int x = a->x + lrpad / 2, w = a->w - lrpad;
	#elif BAR_TITLE_LEFT_PAD_PATCH
	int x = a->x + lrpad / 2, w = a->w - lrpad / 2;
	#elif BAR_TITLE_RIGHT_PAD_PATCH
	int x = a->x, w = a->w - lrpad / 2;
	#else
	int x = a->x, w = a->w;
	#endif // BAR_TITLE_LEFT_PAD_PATCH | BAR_TITLE_RIGHT_PAD_PATCH
	Monitor *m = bar->mon;
	Client *c = m->sel;

	if (!c) {
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		drw_rect(drw, x, a->y, w, a->h, 1, 1);
		return 0;
	}

	int tpad = lrpad / 2;
	#if BAR_WINICON_PATCH
	int ipad = c->icon ? c->icw + ICONSPACING : 0;
	#endif // BAR_WINICON_PATCH
	#if BAR_CENTEREDWINDOWNAME_PATCH
	int cpad = 0;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH
	int tx = x;
	int tw = w;

	drw_setscheme(drw, scheme[m == selmon ? SchemeTitleSel : SchemeTitleNorm]);
	#if BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	XSetErrorHandler(xerrordummy);
	#endif // BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH

	if (w <= TEXTW("A") - lrpad + tpad) // reduce text padding if wintitle is too small
		tpad = (w - TEXTW("A") + lrpad < 0 ? 0 : (w - TEXTW("A") + lrpad) / 2);
	#if BAR_WINICON_PATCH && BAR_CENTEREDWINDOWNAME_PATCH
	else if (TEXTW(c->name) + ipad < w)
		cpad = (w - TEXTW(c->name) - ipad) / 2;
	#elif BAR_CENTEREDWINDOWNAME_PATCH
	else if (TEXTW(c->name) < w)
		cpad = (w - TEXTW(c->name)) / 2;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH

	XSetForeground(drw->dpy, drw->gc, drw->scheme[ColBg].pixel);
	XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, a->y, w, a->h);

	#if BAR_CENTEREDWINDOWNAME_PATCH
	/* Apply center padding, if any */
	tx += cpad;
	tw -= cpad;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH

	tx += tpad;
	tw -= lrpad;

	#if BAR_WINICON_PATCH
	if (ipad) {
		drw_pic(drw, tx, a->y + (a->h - c->ich) / 2, c->icw, c->ich, c->icon);
		tx += ipad;
		tw -= ipad;
	}
	#endif // BAR_WINICON_PATCH

	drw_text(drw, tx, a->y, tw, a->h, 0, c->name, 0, False);

	#if BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	XSync(dpy, False);
	XSetErrorHandler(xerror);
	#endif // BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	drawstateindicator(m, c, 1, x, a->y, w, a->h, 0, 0, c->isfixed);
	return 1;
}

int
click_wintitle(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkWinTitle;
}

