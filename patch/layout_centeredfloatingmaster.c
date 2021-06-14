void
centeredfloatingmaster(Monitor *m)
{
	unsigned int i, n;
	float mfacts, sfacts;
	int mrest, srest;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	Client *c;

	#if VANITYGAPS_PATCH
	float mivf = 1.0; // master inner vertical gap factor
	int oh, ov, ih, iv;
	getgaps(m, &oh, &ov, &ih, &iv, &n);
	#else
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	#endif // VANITYGAPS_PATCH

	if (n == 0)
		return;

	#if VANITYGAPS_PATCH
	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	mw = m->ww - 2*ov - iv*(n - 1);
	sw = m->ww - 2*ov - iv*(n - m->nmaster - 1);

	if (m->nmaster && n > m->nmaster) {
		mivf = 0.8;
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->ww * m->mfact - iv*mivf*(MIN(n, m->nmaster) - 1);
			mh = m->wh * 0.9;
		} else {
			mw = m->ww * 0.9 - iv*mivf*(MIN(n, m->nmaster) - 1);
			mh = m->wh * m->mfact;
		}
		mx = m->wx + (m->ww - mw) / 2;
		my = m->wy + (m->wh - mh - 2*oh) / 2;

		sx = m->wx + ov;
		sy = m->wy + oh;
		sh = m->wh - 2*oh;
	}
	#else
	sx = mx = m->wx;
	sy = my = m->wy;
	sh = mh = m->wh;
	sw = mw = m->ww;

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->ww * m->mfact;
			mh = m->wh * 0.9;
		} else {
			mw = m->ww * 0.9;
			mh = m->wh * m->mfact;
		}
		mx = m->wx + (m->ww - mw) / 2;
		my = m->wy + (m->wh - mh) / 2;
	}
	#endif // VANITYGAPS_PATCH

	getfacts(m, mw, sw, &mfacts, &sfacts, &mrest, &srest);

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			/* nmaster clients are stacked horizontally, in the center of the screen */
			#if CFACTS_PATCH
			resize(c, mx, my, (mw / mfacts) * c->cfact + (i < mrest ? 1 : 0) - (2*c->bw), mh - (2*c->bw), 0);
			#else
			resize(c, mx, my, (mw / mfacts) + (i < mrest ? 1 : 0) - (2*c->bw), mh - (2*c->bw), 0);
			#endif // CFACTS_PATCH
			#if VANITYGAPS_PATCH
			mx += WIDTH(c) + iv*mivf;
			#else
			mx += WIDTH(c);
			#endif
		} else {
			/* stack clients are stacked horizontally */
			#if CFACTS_PATCH
			resize(c, sx, sy, (sw / sfacts) * c->cfact + ((i - m->nmaster) < srest ? 1 : 0) - (2*c->bw), sh - (2*c->bw), 0);
			#else
			resize(c, sx, sy, (sw / sfacts) + ((i - m->nmaster) < srest ? 1 : 0) - (2*c->bw), sh - (2*c->bw), 0);
			#endif // CFACTS_PATCH
			#if VANITYGAPS_PATCH
			sx += WIDTH(c) + iv;
			#else
			sx += WIDTH(c);
			#endif
		}
}

