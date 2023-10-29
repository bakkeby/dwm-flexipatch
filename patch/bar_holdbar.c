void
holdbar(const Arg *arg)
{
	if (selmon->showbar)
		return;
	Bar *bar;
	selmon->showbar = 2;
	updatebarpos(selmon);
	for (bar = selmon->bar; bar; bar = bar->next)
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	drawbar(selmon);
}

void
keyrelease(XEvent *e)
{
	Bar *bar;
	if (XEventsQueued(dpy, QueuedAfterReading)) {
		XEvent ne;
		XPeekEvent(dpy, &ne);

		if (ne.type == KeyPress && ne.xkey.time == e->xkey.time &&
				ne.xkey.keycode == e->xkey.keycode) {
			XNextEvent(dpy, &ne);
			return;
		}
	}
	if (e->xkey.keycode == XKeysymToKeycode(dpy, HOLDKEY) && selmon->showbar == 2) {
		selmon->showbar = 0;
		updatebarpos(selmon);
		for (bar = selmon->bar; bar; bar = bar->next)
			XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		#if BAR_SYSTRAY_PATCH
		if (!selmon->showbar && systray)
			XMoveWindow(dpy, systray->win, -32000, -32000);
		#endif // BAR_SYSTRAY_PATCH
		arrange(selmon);
	}
	#if COMBO_PATCH
	combo = 0;
	#endif // COMBO_PATCH
}

