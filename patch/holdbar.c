void
holdbar(const Arg *arg)
{
	selmon->showbar = 1;
	updateholdbarpos(selmon);
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
}

void
keyrelease(XEvent *e)
{
	if (e->xkey.keycode == XKeysymToKeycode(dpy, HOLDKEY)) {
		selmon->showbar = 0;
		updateholdbarpos(selmon);
		XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
		arrange(selmon);
	}
	#if COMBO_PATCH
	combo = 0;
	#endif // COMBO_PATCH
}

void
updateholdbarpos(Monitor *m)
{
	m->wy = m->my;
	m->wh = m->mh;
	if (m->showbar) {
		m->by = m->topbar ? m->wy : m->wy + m->wh - bh;
		m->wy = m->topbar ? m->wy - bh + bh : m->wy;
	} else {
		m->by = -bh;
	}
}