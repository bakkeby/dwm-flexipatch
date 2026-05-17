void
dragfact(const Arg *arg)
{
	unsigned int n;
	int px, py; // pointer coordinates
	int dist_x, dist_y;
	int horizontal = 0; // layout configuration
	float mfact, cfact, cf, cw, ch, mw, mh;
	Client *c;
	Monitor *m = selmon;
	XEvent ev;
	Time lasttime = 0;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (!(c = m->sel) || !n || !m->lt[m->sellt]->arrange)
		return;

	#if BSTACK_LAYOUT
	else if (m->lt[m->sellt]->arrange == &bstack)
		horizontal = 1;
	#endif // BSTACK_LAYOUT
	#if BSTACKHORIZ_LAYOUT
	else if (m->lt[m->sellt]->arrange == &bstackhoriz)
		horizontal = 1;
	#endif // BSTACK_LAYOUT
	#if FLEXTILE_DELUXE_LAYOUT
	else if (m->lt[m->sellt]->arrange == &flextile) {
		switch (m->ltaxis[LAYOUT]) {
		case SPLIT_HORIZONTAL:
		case SPLIT_CENTERED_HORIZONTAL:
		case SPLIT_HORIZONTAL_DUAL_STACK:
		case SPLIT_HORIZONTAL_FIXED:
		case SPLIT_CENTERED_HORIZONTAL_FIXED:
		case SPLIT_HORIZONTAL_DUAL_STACK_FIXED:
			horizontal = 1;
			break;
		default:
			horizontal = 0;
			break;
		}
	}
	#endif // FLEXTILE_DELUXE_LAYOUT

	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurResize]->cursor, CurrentTime) != GrabSuccess)
		return;

	if (!getrootptr(&px, &py))
		return;

	cf = c->cfact;
	ch = c->h;
	cw = c->w;
	mw = m->ww * m->mfact;
	mh = m->wh * m->mfact;

	#if WARP_PATCH
	ignore_warp = 1;
	#endif // WARP_PATCH

	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch (ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 40))
				continue;
			lasttime = ev.xmotion.time;

			dist_x = ev.xmotion.x - px;
			dist_y = ev.xmotion.y - py;

			if (horizontal) {
				cfact = (float) cf * (cw + dist_x) / cw;
				mfact = (float) (mh + dist_y) / m->wh;
			} else {
				cfact = (float) cf * (ch - dist_y) / ch;
				mfact = (float) (mw + dist_x) / m->ww;
			}

			c->cfact = MAX(0.25, MIN(4.0, cfact));
			m->mfact = MAX(0.05, MIN(0.95, mfact));
			arrangemon(m);
			break;
		}
	} while (ev.type != ButtonRelease);

	#if WARP_PATCH
	ignore_warp = 0;
	#endif // WARP_PATCH

	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

void
resizeorfacts(const Arg *arg)
{
	Monitor *m = selmon;

	if (!m->sel)
		return;

	if (!m->lt[m->sellt]->arrange || m->sel->isfloating)
		resizemouse(arg);
	else
		dragfact(arg);
}
