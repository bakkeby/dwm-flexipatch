int
width_wintitle(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_wintitle(Bar *bar, BarDrawArg *a)
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

	if (!m->sel) {
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		drw_rect(drw, x, 0, w, bh, 1, 1);
		return 0;
	}

	drw_setscheme(drw, scheme[m == selmon ? SchemeTitleSel : SchemeTitleNorm]);
	#if BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	XSetErrorHandler(xerrordummy);
	#endif // BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	#if BAR_CENTEREDWINDOWNAME_PATCH
	int mid = (m->ww - TEXTW(m->sel->name)) / 2 - x;
	#if BAR_PANGO_PATCH
	drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0, False);
	#else
	drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0);
	#endif // BAR_PANGO_PATCH
	#else
	#if BAR_PANGO_PATCH
	drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0, False);
	#else
	drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0);
	#endif // BAR_PANGO_PATCH
	#endif // BAR_CENTEREDWINDOWNAME_PATCH
	#if BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	XSync(dpy, False);
	XSetErrorHandler(xerror);
	#endif // BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
	if (m->sel->isfloating)
		drawindicator(m, m->sel, 1, x, w, 0, 0, c->isfixed, floatindicatortype);
	return 1;
}

int
click_wintitle(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkWinTitle;
}


