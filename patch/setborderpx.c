void
setborderpx(const Arg *arg)
{
	Client *c;
	Monitor *m = selmon;
	#if BAR_BORDER_PATCH
	Bar *bar;
	#endif // BAR_BORDER_PATCH
	int prev_borderpx = m->borderpx;
	m->borderpx = arg->i
		? MAX(m->borderpx + arg->i, 0)
		: m->borderpx == borderpx
		? 0
		: borderpx;
	int delta = 2 * (m->borderpx - prev_borderpx);

	#if BAR_BORDER_PATCH
	if (!barborderpx) {
		for (bar = m->bar; bar; bar = bar->next) {
			bar->bh = bar->bh - 2 * bar->borderpx + 2 * m->borderpx;
			bar->borderpx = m->borderpx;
		}
		updatebarpos(m);
		for (bar = m->bar; bar; bar = bar->next)
			XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	}
	#endif // BAR_BORDER_PATCH

	for (c = m->clients; c; c = c->next) {
		c->bw = m->borderpx;
		#if !FAKEFULLSCREEN_PATCH
		#if FAKEFULLSCREEN_CLIENT_PATCH
		if (c->isfullscreen && !c->fakefullscreen)
			continue;
		#else
		if (c->isfullscreen)
			continue;
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		#endif // FAKEFULLSCREEN_PATCH
		#if BAR_WINTITLEACTIONS_PATCH
		if (HIDDEN(c))
			continue;
		#endif // BAR_WINTITLEACTIONS_PATCH
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if ((c->tags & SPTAGMASK) && !ISVISIBLE(c))
			continue;
		#endif // SCRATCHPADS_PATCH
		#if SCRATCHPAD_ALT_1_PATCH
        if ((c->tags & SCRATCHPAD_MASK))
            continue;
		#endif // SCRATCHPAD_ALT_1_PATCH
		if (!c->isfloating && m->lt[m->sellt]->arrange)
			continue;

		resizeclient(c, c->x, c->y, c->w - delta, c->h - delta);
	}
	arrange(m);
}
