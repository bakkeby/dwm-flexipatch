void
dragmfact(const Arg *arg)
{
	unsigned int n;
	int py, px; // pointer coordinates
	int ax, ay, aw, ah; // area position, width and height
	int center = 0, horizontal = 0, mirror = 0, fixed = 0; // layout configuration
	double fact;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	m = selmon;

	#if VANITYGAPS_PATCH
	int oh, ov, ih, iv;
	getgaps(m, &oh, &ov, &ih, &iv, &n);
	#else
	Client *c;
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	#endif // VANITYGAPS_PATCH

	ax = m->wx;
	ay = m->wy;
	ah = m->wh;
	aw = m->ww;

	if (!n)
		return;
	#if FLEXTILE_DELUXE_LAYOUT
	else if (m->lt[m->sellt]->arrange == &flextile) {
		int layout = m->ltaxis[LAYOUT];
		if (layout < 0) {
			mirror = 1;
			layout *= -1;
		}
		if (layout > FLOATING_MASTER) {
			layout -= FLOATING_MASTER;
			fixed = 1;
		}

		if (layout == SPLIT_HORIZONTAL || layout == SPLIT_HORIZONTAL_DUAL_STACK)
			horizontal = 1;
		else if (layout == SPLIT_CENTERED_VERTICAL && (fixed || n - m->nmaster > 1))
			center = 1;
		else if (layout == FLOATING_MASTER) {
			center = 1;
			if (aw < ah)
				horizontal = 1;
		}
		else if (layout == SPLIT_CENTERED_HORIZONTAL) {
			if (fixed || n - m->nmaster > 1)
				center = 1;
			horizontal = 1;
		}
	}
	#endif // FLEXTILE_DELUXE_LAYOUT
	#if CENTEREDMASTER_LAYOUT
	else if (m->lt[m->sellt]->arrange == &centeredmaster && (fixed || n - m->nmaster > 1))
		center = 1;
	#endif // CENTEREDMASTER_LAYOUT
	#if CENTEREDFLOATINGMASTER_LAYOUT
	else if (m->lt[m->sellt]->arrange == &centeredfloatingmaster)
		center = 1;
	#endif // CENTEREDFLOATINGMASTER_LAYOUT
	#if BSTACK_LAYOUT
	else if (m->lt[m->sellt]->arrange == &bstack)
		horizontal = 1;
	#endif // BSTACK_LAYOUT
	#if BSTACKHORIZ_LAYOUT
	else if (m->lt[m->sellt]->arrange == &bstackhoriz)
		horizontal = 1;
	#endif // BSTACKHORIZ_LAYOUT

	/* do not allow mfact to be modified under certain conditions */
	if (!m->lt[m->sellt]->arrange                            // floating layout
		|| (!fixed && m->nmaster && n <= m->nmaster) // no master
		#if MONOCLE_LAYOUT
		|| m->lt[m->sellt]->arrange == &monocle
		#endif // MONOCLE_LAYOUT
		#if GRIDMODE_LAYOUT
		|| m->lt[m->sellt]->arrange == &grid
		#endif // GRIDMODE_LAYOUT
		#if HORIZGRID_LAYOUT
		|| m->lt[m->sellt]->arrange == &horizgrid
		#endif // HORIZGRID_LAYOUT
		#if GAPPLESSGRID_LAYOUT
		|| m->lt[m->sellt]->arrange == &gaplessgrid
		#endif // GAPPLESSGRID_LAYOUT
		#if NROWGRID_LAYOUT
		|| m->lt[m->sellt]->arrange == &nrowgrid
		#endif // NROWGRID_LAYOUT
		#if FLEXTILE_DELUXE_LAYOUT
		|| (m->lt[m->sellt]->arrange == &flextile && m->ltaxis[LAYOUT] == NO_SPLIT)
		#endif // FLEXTILE_DELUXE_LAYOUT
	)
		return;

	#if VANITYGAPS_PATCH
	ay += oh;
	ax += ov;
	aw -= 2*ov;
	ah -= 2*oh;
	#endif // VANITYGAPS_PATCH

	if (center) {
		if (horizontal) {
			px = ax + aw / 2;
			#if VANITYGAPS_PATCH
			py = ay + ah / 2 + (ah - 2*ih) * (m->mfact / 2.0) + ih / 2;
			#else
			py = ay + ah / 2 + ah * m->mfact / 2.0;
			#endif // VANITYGAPS_PATCH
		} else { // vertical split
			#if VANITYGAPS_PATCH
			px = ax + aw / 2 + (aw - 2*iv) * m->mfact / 2.0 + iv / 2;
			#else
			px = ax + aw / 2 + aw * m->mfact / 2.0;
			#endif // VANITYGAPS_PATCH
			py = ay + ah / 2;
		}
	} else if (horizontal) {
		px = ax + aw / 2;
		if (mirror)
			#if VANITYGAPS_PATCH
			py = ay + (ah - ih) * (1.0 - m->mfact) + ih / 2;
			#else
			py = ay + (ah * (1.0 - m->mfact));
			#endif // VANITYGAPS_PATCH
		else
			#if VANITYGAPS_PATCH
			py = ay + ((ah - ih) * m->mfact) + ih / 2;
			#else
			py = ay + (ah * m->mfact);
			#endif // VANITYGAPS_PATCH
	} else { // vertical split
		if (mirror)
			#if VANITYGAPS_PATCH
			px = ax + (aw - iv) * (1.0 - m->mfact) + iv / 2;
			#else
			px = ax + (aw * m->mfact);
			#endif // VANITYGAPS_PATCH
		else
			#if VANITYGAPS_PATCH
			px = ax + ((aw - iv) * m->mfact) + iv / 2;
			#else
			px = ax + (aw * m->mfact);
			#endif // VANITYGAPS_PATCH
		py = ay + ah / 2;
	}

	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[horizontal ? CurResizeVertArrow : CurResizeHorzArrow]->cursor, CurrentTime) != GrabSuccess)
		return;

	#if WARP_PATCH
	ignore_warp = 1;
	#endif // WARP_PATCH

	XWarpPointer(dpy, None, root, 0, 0, 0, 0, px, py);

	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 40))
				continue;
			if (lasttime != 0) {
				px = ev.xmotion.x;
				py = ev.xmotion.y;
			}
			lasttime = ev.xmotion.time;

			#if VANITYGAPS_PATCH
			if (center)
				if (horizontal)
					if (py - ay > ah / 2)
						fact = (double) 1.0 - (ay + ah - py - ih / 2) * 2 / (double) (ah - 2*ih);
					else
						fact = (double) 1.0 - (py - ay - ih / 2) * 2 / (double) (ah - 2*ih);
				else
					if (px - ax > aw / 2)
						fact = (double) 1.0 - (ax + aw - px - iv / 2) * 2 / (double) (aw - 2*iv);
					else
						fact = (double) 1.0 - (px - ax - iv / 2) * 2 / (double) (aw - 2*iv);
			else
				if (horizontal)
					fact = (double) (py - ay - ih / 2) / (double) (ah - ih);
				else
					fact = (double) (px - ax - iv / 2) / (double) (aw - iv);
			#else
			if (center)
				if (horizontal)
					if (py - ay > ah / 2)
						fact = (double) 1.0 - (ay + ah - py) * 2 / (double) ah;
					else
						fact = (double) 1.0 - (py - ay) * 2 / (double) ah;
				else
					if (px - ax > aw / 2)
						fact = (double) 1.0 - (ax + aw - px) * 2 / (double) aw;
					else
						fact = (double) 1.0 - (px - ax) * 2 / (double) aw;
			else
				if (horizontal)
					fact = (double) (py - ay) / (double) ah;
				else
					fact = (double) (px - ax) / (double) aw;
			#endif // VANITYGAPS_PATCH

			if (!center && mirror)
				fact = 1.0 - fact;

			setmfact(&((Arg) { .f = 1.0 + fact }));
			px = ev.xmotion.x;
			py = ev.xmotion.y;
			break;
		}
	} while (ev.type != ButtonRelease);

	#if WARP_PATCH
	ignore_warp = 0;
	#endif // WARP_PATCH

	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

