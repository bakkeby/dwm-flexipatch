int
width_wintitle(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_wintitle(Bar *bar, BarDrawArg *a)
{
	#if !BAR_ACTIVETAGINDICATORBAR_PATCH && !BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	int boxs = drw->fonts->h / 9;
	#endif // BAR_ACTIVETAGINDICATORBAR_PATCH | BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	int boxw = drw->fonts->h / 6 + 2;

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

	if (m->sel) {
		#if BAR_VTCOLORS_PATCH
		drw_setscheme(drw, scheme[m == selmon ? SchemeTitleSel : SchemeTitleNorm]);
		#elif BAR_TITLECOLOR_PATCH
		drw_setscheme(drw, scheme[m == selmon ? SchemeTitle : SchemeNorm]);
		#else
		drw_setscheme(drw, scheme[m == selmon ? SchemeSel : SchemeNorm]);
		#endif // BAR_VTCOLORS_PATCH / BAR_TITLECOLOR_PATCH
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
			#if BAR_ACTIVETAGINDICATORBAR_PATCH
			drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw, m->sel->isfixed, 0);
			#elif BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
			drw_rect(drw, x + boxw, bh - boxw/2, w - ( 2 * boxw + 1), boxw/2, 0);
			#else
			drw_rect(drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0);
			#endif // BAR_ACTIVETAGINDICATORBAR_PATCH
	} else {
		#if BAR_VTCOLORS_PATCH
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		#else
		drw_setscheme(drw, scheme[SchemeNorm]);
		#endif // BAR_VTCOLORS_PATCH
		drw_rect(drw, x, 0, w, bh, 1, 1);
	}
	return x + w;
}

int
click_wintitle(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkWinTitle;
}


