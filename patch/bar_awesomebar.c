int
width_awesomebar(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_awesomebar(Bar *bar, BarArg *a)
{
	int n = 0, scm, remainder = 0, tabw, tpad, tx, tw;
	unsigned int i;
	#if BAR_CENTEREDWINDOWNAME_PATCH
	int cpad;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH
	#if BAR_WINICON_PATCH
	int ipad;
	#endif // BAR_WINICON_PATCH
	#if BAR_TITLE_LEFT_PAD_PATCH && BAR_TITLE_RIGHT_PAD_PATCH
	int x = a->x + lrpad / 2, w = a->w - lrpad;
	#elif BAR_TITLE_LEFT_PAD_PATCH
	int x = a->x + lrpad / 2, w = a->w - lrpad / 2;
	#elif BAR_TITLE_RIGHT_PAD_PATCH
	int x = a->x, w = a->w - lrpad / 2;
	#else
	int x = a->x, w = a->w;
	#endif // BAR_TITLE_LEFT_PAD_PATCH | BAR_TITLE_RIGHT_PAD_PATCH

	Client *c;
	for (c = bar->mon->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	if (n > 0) {
		remainder = w % n;
		tabw = w / n;
		for (i = 0, c = bar->mon->clients; c; c = c->next, i++) {
			if (!ISVISIBLE(c))
				continue;
			if (bar->mon->sel == c && HIDDEN(c))
				scm = SchemeHidSel;
			else if (HIDDEN(c))
				scm = SchemeHidNorm;
			else if (bar->mon->sel == c)
				scm = SchemeTitleSel;
			else
				scm = SchemeTitleNorm;

			tpad = lrpad / 2;
			#if BAR_CENTEREDWINDOWNAME_PATCH
			cpad = 0;
			#endif // BAR_CENTEREDWINDOWNAME_PATCH
			#if BAR_WINICON_PATCH
			ipad = c->icon ? c->icw + ICONSPACING : 0;
			#endif // BAR_WINICON_PATCH

			tx = x;
			tw = tabw;

			#if BAR_WINICON_PATCH && BAR_CENTEREDWINDOWNAME_PATCH
			if (TEXTW(c->name) + ipad < tabw)
				cpad = (tabw - TEXTW(c->name) - ipad) / 2;
			#elif BAR_CENTEREDWINDOWNAME_PATCH
			if (TEXTW(c->name) < tabw)
				cpad = (tabw - TEXTW(c->name)) / 2;
			#endif // BAR_CENTEREDWINDOWNAME_PATCH

			drw_setscheme(drw, scheme[scm]);

			XSetForeground(drw->dpy, drw->gc, drw->scheme[ColBg].pixel);
			XFillRectangle(drw->dpy, drw->drawable, drw->gc, tx, a->y, tw, a->h);

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

			drawstateindicator(c->mon, c, 1, x, a->y, tabw + (i < remainder ? 1 : 0), a->h, 0, 0, c->isfixed);
			x += tabw + (i < remainder ? 1 : 0);
		}
	}
	return n;
}

int
click_awesomebar(Bar *bar, Arg *arg, BarArg *a)
{
	int x = 0, n = 0;
	Client *c;

	for (c = bar->mon->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	c = bar->mon->clients;

	do {
		if (!c || !ISVISIBLE(c))
			continue;
		else
			x += (1.0 / (double)n) * a->w;
	} while (c && a->x > x && (c = c->next));

	if (c) {
		arg->v = c;
		return ClkWinTitle;
	}
	return -1;
}

