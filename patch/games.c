void
minimize(Client *c)
{
	if (!c || HIDDEN(c))
		return;

	Window w = c->win;
	static XWindowAttributes ra, ca;

	/* More or less taken directly from blackbox's hide() function */
	XGrabServer(dpy);
	XGetWindowAttributes(dpy, root, &ra);
	XGetWindowAttributes(dpy, w, &ca);
	/* Prevent UnmapNotify events */
	XSelectInput(dpy, root, ra.your_event_mask & ~SubstructureNotifyMask);
	XSelectInput(dpy, w, ca.your_event_mask & ~StructureNotifyMask);
	XUnmapWindow(dpy, w);
	setclientstate(c, IconicState);
	XSelectInput(dpy, root, ra.your_event_mask);
	XSelectInput(dpy, w, ca.your_event_mask);
	XUngrabServer(dpy);
}

void
unminimize(Client *c)
{
	if (!c || !HIDDEN(c))
		return;

	XMapWindow(dpy, c->win);
	setclientstate(c, NormalState);
}
