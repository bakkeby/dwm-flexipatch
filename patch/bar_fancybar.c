int
width_fancybar(Monitor *m, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_fancybar(Monitor *m, BarDrawArg *a)
{
	int ftw, mw, ew = 0, n = 0, x = a->x, w = a->w;
	unsigned int i;
	Client *c;
	#if !BAR_HIDEVACANTTAGS_PATCH
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
	#endif // BAR_HIDEVACANTTAGS_PATCH

	for (c = m->clients; c; c = c->next) {
		if (ISVISIBLE(c))
			n++;
	}

	if (n > 0) {
		ftw = TEXTW(m->sel->name) + lrpad;
		mw = (ftw >= w || n == 1) ? 0 : (w - ftw) / (n - 1);

		i = 0;
		for (c = m->clients; c; c = c->next) {
			if (!ISVISIBLE(c) || c == m->sel)
				continue;
			ftw = TEXTW(c->name);
			if (ftw < mw)
				ew += (mw - ftw);
			else
				i++;
		}
		if (i > 0)
			mw += ew / i;

		for (c = m->clients; c; c = c->next) {
			if (!ISVISIBLE(c))
				continue;
			ftw = MIN(m->sel == c ? w : mw, TEXTW(c->name));

			#if BAR_VTCOLORS_PATCH
			drw_setscheme(drw, scheme[m->sel == c ? SchemeTitleSel : SchemeTitleNorm]);
			#elif BAR_TITLECOLOR_PATCH
			drw_setscheme(drw, scheme[m->sel == c ? SchemeTitle : SchemeNorm]);
			#else
			drw_setscheme(drw, scheme[m->sel == c ? SchemeSel : SchemeNorm]);
			#endif // BAR_VTCOLORS_PATCH / BAR_TITLECOLOR_PATCH
			if (ftw > 0) /* trap special handling of 0 in drw_text */
				#if BAR_PANGO_PATCH
				drw_text(drw, x, 0, ftw, bh, lrpad / 2, c->name, 0, False);
				#else
				drw_text(drw, x, 0, ftw, bh, lrpad / 2, c->name, 0);
				#endif // BAR_PANGO_PATCH
			#if !BAR_HIDEVACANTTAGS_PATCH
			if (c->isfloating)
				#if BAR_ACTIVETAGINDICATORBAR_PATCH
				drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw, c->isfixed, 0);
				#elif BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
				drw_rect(drw, x + boxw, bh - boxw/2, w - ( 2 * boxw + 1), boxw/2,
				#else
				drw_rect(drw, x + boxs, boxs, boxw, boxw, c->isfixed, 0);
				#endif // BAR_ACTIVETAGINDICATORBAR_PATCH
			#endif // BAR_HIDEVACANTTAGS_PATCH
			x += ftw;
			w -= ftw;
		}
	}
	#if BAR_VTCOLORS_PATCH
	drw_setscheme(drw, scheme[SchemeTitleNorm]);
	#else
	drw_setscheme(drw, scheme[SchemeNorm]);
	#endif // BAR_VTCOLORS_PATCH
	drw_rect(drw, x, 0, w, bh, 1, 1);
	return x + w;
}

int
click_fancybar(Monitor *m, Arg *arg, BarClickArg *a)
{
	return ClkWinTitle;
}


