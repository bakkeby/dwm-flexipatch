int
width_fancybar(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_fancybar(Bar *bar, BarDrawArg *a)
{
	int ftw, mw, ew = 0, n = 0;
	unsigned int i;
	Client *c;
	Monitor *m = bar->mon;

	#if BAR_PANGO_PATCH
	int boxs = drw->font->h / 9;
	int boxw = drw->font->h / 6 + 2;
	#else
	int boxs = drw->fonts->h / 9;
	int boxw = drw->fonts->h / 6 + 2;
	#endif // BAR_PANGO_PATCH
	#if BAR_TITLE_LEFT_PAD && BAR_TITLE_RIGHT_PAD
	int x = a->x + lrpad / 2, w = a->w - lrpad;
	#elif BAR_TITLE_LEFT_PAD
	int x = a->x + lrpad / 2, w = a->w - lrpad / 2;
	#elif BAR_TITLE_RIGHT_PAD
	int x = a->x, w = a->w - lrpad / 2;
	#else
	int x = a->x, w = a->w;
	#endif // BAR_TITLE_LEFT_PAD | BAR_TITLE_RIGHT_PAD

	for (c = m->clients; c; c = c->next) {
		if (ISVISIBLE(c))
			n++;
	}

	if (n > 0) {
		ftw = TEXTW(m->sel->name);
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
			if (c->isfloating)
				drw_rect(drw, x + boxs, boxs, boxw, boxw, c->isfixed, 0);
			x += ftw;
			w -= ftw;
		}
	}
	return x + w;
}

int
click_fancybar(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkWinTitle;
}


