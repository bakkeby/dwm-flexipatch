int
width_fancybar(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_fancybar(Bar *bar, BarArg *a)
{
	int ftw, mw, ew = 0, n = 0;
	unsigned int i;
	Client *c;
	Monitor *m = bar->mon;

	#if BAR_TITLE_LEFT_PAD_PATCH && BAR_TITLE_RIGHT_PAD_PATCH
	int x = a->x + lrpad / 2, w = a->w - lrpad;
	#elif BAR_TITLE_LEFT_PAD_PATCH
	int x = a->x + lrpad / 2, w = a->w - lrpad / 2;
	#elif BAR_TITLE_RIGHT_PAD_PATCH
	int x = a->x, w = a->w - lrpad / 2;
	#else
	int x = a->x, w = a->w;
	#endif // BAR_TITLE_LEFT_PAD_PATCH | BAR_TITLE_RIGHT_PAD_PATCH

	for (c = m->clients; c; c = c->next) {
		if (ISVISIBLE(c))
			n++;
	}

	if (n > 0) {
		ftw = TEXTW(m->sel->name);
		#if BAR_WINICON_PATCH
		if (m->sel->icon)
			ftw += m->sel->icon->width + ICONSPACING;
		#endif // BAR_WINICON_PATCH
		mw = (ftw >= w || n == 1) ? 0 : (w - ftw) / (n - 1);

		i = 0;

		for (c = m->clients; c; c = c->next) {
			if (!ISVISIBLE(c) || c == m->sel)
				continue;
			ftw = TEXTW(c->name);
			#if BAR_WINICON_PATCH
			if (c->icon)
				ftw += c->icon->width + ICONSPACING;
			#endif // BAR_WINICON_PATCH
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
			if (ftw > 0) { /* trap special handling of 0 in drw_text */

				drw_text(drw, x, a->y, ftw, a->h, lrpad / 2, c->name, 0, False);

				#if BAR_WINICON_PATCH
				drw_text(drw, x, a->y, ftw, a->h, lrpad / 2 + (c->icon ? c->icon->width + ICONSPACING : 0), c->name, 0, False);
				if (c->icon)
					drw_img(drw, x + lrpad / 2, a->y + (a->h - c->icon->height) / 2, c->icon, tmpicon);
				#else
				drw_text(drw, x, a->y, ftw, a->h, lrpad / 2, c->name, 0, False);
				#endif // BAR_WINICON_PATCH

			}
			drawstateindicator(c->mon, c, 1, x, a->y, ftw, a->h, 0, 0, c->isfixed);
			x += ftw;
			w -= ftw;
		}
	}
	return n;
}

int
click_fancybar(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkWinTitle;
}

