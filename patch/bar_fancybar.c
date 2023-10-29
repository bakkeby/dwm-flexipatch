int
width_fancybar(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_fancybar(Bar *bar, BarArg *a)
{
	int tabw, mw, ew = 0, n = 0, tx, tw;
	#if BAR_WINICON_PATCH
	int ipad;
	#endif // BAR_WINICON_PATCH
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
		tabw = TEXTW(m->sel->name);
		#if BAR_WINICON_PATCH
		if (m->sel->icon)
			tabw += m->sel->icw + ICONSPACING;
		#endif // BAR_WINICON_PATCH
		mw = (tabw >= w || n == 1) ? 0 : (w - tabw) / (n - 1);

		i = 0;

		for (c = m->clients; c; c = c->next) {
			if (!ISVISIBLE(c) || c == m->sel)
				continue;
			tabw = TEXTW(c->name);
			#if BAR_WINICON_PATCH
			if (c->icon)
				tabw += c->icw + ICONSPACING;
			#endif // BAR_WINICON_PATCH
			if (tabw < mw)
				ew += (mw - tabw);
			else
				i++;
		}

		if (i > 0)
			mw += ew / i;

		for (c = m->clients; c; c = c->next) {
			if (!ISVISIBLE(c))
				continue;
			tabw = MIN(m->sel == c ? w : mw, TEXTW(c->name));
			#if BAR_WINICON_PATCH
			ipad = c->icon ? c->icw + ICONSPACING : 0;
			tabw += ipad;
			#endif // BAR_WINICON_PATCH
			tx = x;
			tw = tabw;
			drw_setscheme(drw, scheme[m->sel == c ? SchemeTitleSel : SchemeTitleNorm]);

			XSetForeground(drw->dpy, drw->gc, drw->scheme[ColBg].pixel);
			XFillRectangle(drw->dpy, drw->drawable, drw->gc, tx, a->y, tw, a->h);

			if (tabw <= 0) /* trap special handling of 0 in drw_text */
				continue;

			tx += lrpad / 2;
			tw -= lrpad;

			#if BAR_WINICON_PATCH
			if (ipad) {
				drw_pic(drw, tx, a->y + (a->h - c->ich) / 2, c->icw, c->ich, c->icon);
				tx += ipad;
				tw -= ipad;
			}
			#endif // BAR_WINICON_PATCH

			drw_text(drw, tx, a->y, tw, a->h, 0, c->name, 0, False);
			drawstateindicator(c->mon, c, 1, x, a->y, tabw, a->h, 0, 0, c->isfixed);
			x += tabw;
			w -= tabw;
		}
	}
	return n;
}

int
click_fancybar(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkWinTitle;
}

