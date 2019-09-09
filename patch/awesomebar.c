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
	if (c == selmon->sel)
		hide(c);
	else {
		if (HIDDEN(c))
			show(c);
		focus(c);
		restack(selmon);
	}
}