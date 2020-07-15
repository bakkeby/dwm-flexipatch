int
width_awesomebar(Monitor *m, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_awesomebar(Monitor *m, BarDrawArg *a)
{
	int n = 0, scm, remainder = 0, tabw;
	unsigned int i, x = a->x;
	Client *c;
	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	if (n > 0) {
		remainder = a->w % n;
		tabw = a->w / n;
		for (i = 0, c = m->clients; c; c = c->next, i++) {
			if (!ISVISIBLE(c))
				continue;
			if (m->sel == c)
				#if BAR_VTCOLORS_PATCH
				scm = SchemeTitleSel;
				#elif BAR_TITLECOLOR_PATCH
				scm = SchemeTitle;
				#else
				scm = SchemeSel;
				#endif // BAR_VTCOLORS_PATCH / BAR_TITLECOLOR_PATCH
			else if (HIDDEN(c))
				scm = SchemeHid;
			else
				#if BAR_VTCOLORS_PATCH
				scm = SchemeTitleNorm;
				#else
				scm = SchemeNorm;
				#endif // BAR_VTCOLORS_PATCH

			drw_setscheme(drw, scheme[scm]);
			#if BAR_PANGO_PATCH
			drw_text(drw, x, 0, tabw + (i < remainder ? 1 : 0), bh, lrpad / 2, c->name, 0, False);
			#else
			drw_text(drw, x, 0, tabw + (i < remainder ? 1 : 0), bh, lrpad / 2, c->name, 0);
			#endif // BAR_PANGO_PATCH
			x += tabw;
		}
	}
	return a->x + a->w;
}

int
click_awesomebar(Monitor *m, Arg *arg, BarClickArg *a)
{
	int x = 0, n = 0;
	Client *c;

	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	c = m->clients;

	do {
		if (!c || !ISVISIBLE(c))
			continue;
		else
			x += (1.0 / (double)n) * a->rel_w;
	} while (c && a->rel_x > x && (c = c->next));

	if (c) {
		arg->v = c;
		return ClkWinTitle;
	}
	return -1;
}

void
hide(Client *c) {

	if (!c || HIDDEN(c))
		return;

	Window w = c->win;
	static XWindowAttributes ra, ca;

	// more or less taken directly from blackbox's hide() function
	XGrabServer(dpy);
	XGetWindowAttributes(dpy, root, &ra);
	XGetWindowAttributes(dpy, w, &ca);
	// prevent UnmapNotify events
	XSelectInput(dpy, root, ra.your_event_mask & ~SubstructureNotifyMask);
	XSelectInput(dpy, w, ca.your_event_mask & ~StructureNotifyMask);
	XUnmapWindow(dpy, w);
	setclientstate(c, IconicState);
	XSelectInput(dpy, root, ra.your_event_mask);
	XSelectInput(dpy, w, ca.your_event_mask);
	XUngrabServer(dpy);

	focus(c->snext);
	arrange(c->mon);
}

void
show(Client *c)
{
	if (!c || !HIDDEN(c))
		return;

	XMapWindow(dpy, c->win);
	setclientstate(c, NormalState);
	arrange(c->mon);
}

void
togglewin(const Arg *arg)
{
	Client *c = (Client*)arg->v;
	if (!c)
		return;
	if (c == selmon->sel)
		hide(c);
	else {
		if (HIDDEN(c))
			show(c);
		focus(c);
		restack(selmon);
	}
}

void
showhideclient(const Arg *arg)
{
	Client *c = (Client*)arg->v;
	if (!c)
		c = selmon->sel;
	if (!c)
		return;

	if (HIDDEN(c)) {
		show(c);
		restack(selmon);
	} else {
		hide(c);
	}
}