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
			drw_setscheme(drw, scheme[m->sel == c ? SchemeTitleSel : SchemeTitleNorm]);
			if (ftw > 0) /* trap special handling of 0 in drw_text */
				#if BAR_PANGO_PATCH
				drw_text(drw, x, 0, ftw, bh, lrpad / 2, c->name, 0, False);
				#else
				drw_text(drw, x, 0, ftw, bh, lrpad / 2, c->name, 0);
				#endif // BAR_PANGO_PATCH
			if (c->isfloating)
				drawindicator(c->mon, c, 1, x, w, 0, 0, c->isfixed, floatindicatortype);
			x += ftw;
			w -= ftw;
		}
	}
	return n;
}

int
click_fancybar(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkWinTitle;
}


