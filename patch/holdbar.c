void
holdbar(const Arg *arg)
{
	selmon->showbar = 1;
	updateholdbarpos(selmon);
	#if BARPADDING_PATCH
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx + sp, selmon->by + vp, selmon->ww -  2 * sp, bh);
	#else
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
	#endif // BARPADDING_PATCH
	#if EXTRABAR_PATCH
	#if BARPADDING_PATCH
	XMoveResizeWindow(dpy, selmon->extrabarwin, selmon->wx + sp, selmon->eby - vp, selmon->ww - 2 * sp, bh);
	#else
	XMoveResizeWindow(dpy, selmon->extrabarwin, selmon->wx, selmon->eby, selmon->ww, bh);
	#endif // BARPADDING_PATCH
	#endif // EXTRABAR_PATCH
}

void
keyrelease(XEvent *e)
{
	if (e->xkey.keycode == XKeysymToKeycode(dpy, HOLDKEY)) {
		selmon->showbar = 0;
		updateholdbarpos(selmon);
		#if BARPADDING_PATCH
		XMoveResizeWindow(dpy, selmon->barwin, selmon->wx + sp, selmon->by + vp, selmon->ww -  2 * sp, bh);
		#else
		XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
		#endif // BARPADDING_PATCH
		#if EXTRABAR_PATCH
		#if BARPADDING_PATCH
		XMoveResizeWindow(dpy, selmon->extrabarwin, selmon->wx + sp, selmon->eby - vp, selmon->ww - 2 * sp, bh);
		#else
		XMoveResizeWindow(dpy, selmon->extrabarwin, selmon->wx, selmon->eby, selmon->ww, bh);
		#endif // BARPADDING_PATCH
		#endif // EXTRABAR_PATCH
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
	#if EXTRABAR_PATCH
	#if BARPADDING_PATCH
	m->wh = m->wh - vertpad * m->showbar * 2 - bh * m->showbar * 2;
	m->wy = m->showbar ? m->wy + bh + vertpad: m->wy;
	if (m->showbar) {
		m->by = m->topbar ? m->wy - bh - vertpad: m->wy + m->wh + vertpad;
		m->eby = m->topbar ? m->wy + m->wh + vertpad: m->wy - bh - vertpad;
	} else {
		m->by = -bh - vertpad;
		m->eby = -bh - vertpad;
	}
	#else
	m->wh = m->wh - bh * m->showbar * 2;
	m->wy = m->showbar ? m->wy + bh : m->wy;
	if (m->showbar) {
		m->by = m->topbar ? m->wy - bh : m->wy + m->wh;
		m->eby = m->topbar ? m->wy + m->wh : m->wy - bh;
	} else {
		m->by = -bh;
		m->eby = -bh;
	}
	#endif // BARPADDING_PATCH
	#elif BARPADDING_PATCH
	if (m->showbar) {
		m->wh = m->wh - vertpad - bh;
		m->by = m->topbar ? m->wy : m->wy + m->wh + vertpad;
		m->wy = m->topbar ? m->wy + bh + vp : m->wy;
	} else
		m->by = -bh - vp;
	#else
	if (m->showbar) {
		m->wh -= bh;
		m->by = m->topbar ? m->wy : m->wy + m->wh;
		m->wy = m->topbar ? m->wy + bh : m->wy;
	} else
		m->by = -bh;
	#endif // EXTRABAR_PATCH
}