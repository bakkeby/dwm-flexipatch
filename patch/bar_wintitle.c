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
	int pad = lrpad / 2;

	if (!c) {
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		drw_rect(drw, x, a->y, w, a->h, 1, 1);
		return 0;
	}

	drw_setscheme(drw, scheme[m == selmon ? SchemeTitleSel : SchemeTitleNorm]);
	#if BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	XSetErrorHandler(xerrordummy);
	#endif // BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	#if BAR_CENTEREDWINDOWNAME_PATCH
	if (TEXTW(c->name) < w)
		pad = (w - TEXTW(c->name) + lrpad) / 2;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH

	#if BAR_WINICON_PATCH
	drw_text(drw, x, a->y, w, a->h, pad + (c->icon ? c->icon->width + ICONSPACING : 0), c->name, 0, False);
	if (c->icon)
		drw_img(drw, x + pad, a->y + (a->h - c->icon->height) / 2, c->icon, tmpicon);
	#else
	drw_text(drw, x, a->y, w, a->h, pad, c->name, 0, False);
	#endif // BAR_WINICON_PATCH

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

