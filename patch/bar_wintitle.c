int
width_wintitle(Monitor *m, int max_width)
{
	return max_width;
}

int
draw_wintitle(Monitor *m, int x, int w)
{
	#if !BAR_ACTIVETAGINDICATORBAR_PATCH && !BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	#if BAR_PANGO_PATCH
	int boxs = drw->font->h / 9;
	#else
	int boxs = drw->fonts->h / 9;
	#endif // BAR_PANGO_PATCH
	#endif // BAR_ACTIVETAGINDICATORBAR_PATCH | BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	#if BAR_PANGO_PATCH
	int boxw = drw->font->h / 6 + 2;
	#else
	int boxw = drw->fonts->h / 6 + 2;
	#endif // BAR_PANGO_PATCH

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
		#if BAR_PADDING_PATCH && BAR_PANGO_PATCH
		drw_text(drw, x, 0, w - 2*sp, bh, mid, m->sel->name, 0, False);
		#elif BAR_PADDING_PATCH
		drw_text(drw, x, 0, w - 2*sp, bh, mid, m->sel->name, 0);
		#elif BAR_PANGO_PATCH
		drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0, False);
		#else
		drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0);
		#endif // BAR_PADDING_PATCH
		#else
		#if BAR_PADDING_PATCH && BAR_PANGO_PATCH
		drw_text(drw, x, 0, w - 2*sp, bh, lrpad / 2, m->sel->name, 0, False);
		#elif BAR_PADDING_PATCH
		drw_text(drw, x, 0, w - 2*sp, bh, lrpad / 2, m->sel->name, 0);
		#elif BAR_PANGO_PATCH
		drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0, False);
		#else
		drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0);
		#endif // BAR_PADDING_PATCH
		#endif // BAR_CENTEREDWINDOWNAME_PATCH
		#if BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		#endif // BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
		if (m->sel->isfloating)
			#if BAR_ACTIVETAGINDICATORBAR_PATCH
			drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw, m->sel->isfixed, 0);
			#elif BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
			drw_rect(drw, x + boxw, bh - boxw/2, w - ( 2 * boxw + 1), boxw/2,
			#else
			drw_rect(drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0);
			#endif // BAR_ACTIVETAGINDICATORBAR_PATCH
	} else {
		#if BAR_VTCOLORS_PATCH
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		#else
		drw_setscheme(drw, scheme[SchemeNorm]);
		#endif // BAR_VTCOLORS_PATCH
		#if BAR_PADDING_PATCH
		drw_rect(drw, x, 0, w - 2 * sp, bh, 1, 1);
		#else
		drw_rect(drw, x, 0, w, bh, 1, 1);
		#endif // BAR_PADDING_PATCH
	}
	return x + w;
}

int
click_wintitle(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h)
{
	return ClkWinTitle;
}


