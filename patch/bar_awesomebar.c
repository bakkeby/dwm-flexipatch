int
width_awesomebar(Monitor *m, int max_width)
{
	return max_width;
}

int
draw_awesomebar(Monitor *m, int x_orig, int w)
{
	int n = 0, scm, remainder = 0, tabw;
	unsigned int i, x = x_orig;
	Client *c;
	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;

	if (n > 0) {
		remainder = w % n;
		tabw = w / n;
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
	return x_orig + w;
}

int
click_awesomebar(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h)
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
			x += (1.0 / (double)n) * rel_w;
	} while (c && rel_x > x && (c = c->next));

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