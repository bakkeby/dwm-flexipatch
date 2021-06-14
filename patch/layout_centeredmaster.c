void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	float mfacts = 0, lfacts = 0, rfacts = 0;
	int mtotal = 0, ltotal = 0, rtotal = 0;
	int mrest = 0, lrest = 0, rrest = 0;
	Client *c;

	#if VANITYGAPS_PATCH
	int oh, ov, ih, iv;
	getgaps(m, &oh, &ov, &ih, &iv, &n);
	#else
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	#endif // VANITYGAPS_PATCH

	if (n == 0)
		return;

	/* initialize areas */
	#if VANITYGAPS_PATCH
	mx = m->wx + ov;
	my = m->wy + oh;
	mh = m->wh - 2*oh - ih * ((!m->nmaster ? n : MIN(n, m->nmaster)) - 1);
	mw = m->ww - 2*ov;
	lh = m->wh - 2*oh - ih * (((n - m->nmaster) / 2) - 1);
	rh = m->wh - 2*oh - ih * (((n - m->nmaster) / 2) - ((n - m->nmaster) % 2 ? 0 : 1));

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (n - m->nmaster > 1) {
			/* ||<-S->|<---M--->|<-S->|| */
			mw = (m->ww - 2*ov - 2*iv) * m->mfact;
			lw = (m->ww - mw - 2*ov - 2*iv) / 2;
			mx += lw + iv;
		} else {
			/* ||<---M--->|<-S->|| */
			mw = (mw - iv) * m->mfact;
			lw = m->ww - mw - iv - 2*ov;
		}
		rw = lw;
		lx = m->wx + ov;
		ly = m->wy + oh;
		rx = mx + mw + iv;
		ry = m->wy + oh;
	}
	#else
	mx = m->wx;
	my = m->wy;
	mh = m->wh;
	mw = m->ww;
	lh = m->wh;
	rh = m->wh;

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (n - m->nmaster > 1) {
			/* ||<-S->|<---M--->|<-S->|| */
			mw = m->ww * m->mfact;
			lw = (m->ww - mw) / 2;
			mx += lw;
		} else {
			/* ||<---M--->|<-S->|| */
			mw = mw * m->mfact;
			lw = m->ww - mw;
		}
		rw = lw;
		lx = m->wx;
		ly = m->wy;
		rx = mx + mw;
		ry = m->wy;
	}
	#endif // VANITYGAPS_PATCH

	/* calculate facts */
	#if CFACTS_PATCH
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mfacts += c->cfact; // total factor of master area
		else if ((n - m->nmaster) % 2)
			lfacts += c->cfact; // total factor of left hand stack area
		else
			rfacts += c->cfact; // total factor of right hand stack area
	}

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		if (!m->nmaster || n < m->nmaster)
			mtotal += mh / mfacts;
		else if ((n - m->nmaster) % 2)
			ltotal += lh * (c->cfact / lfacts);
		else
			rtotal += rh * (c->cfact / rfacts);
	#else
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mfacts += 1;
		else if ((n - m->nmaster) % 2)
			lfacts += 1; // total factor of left hand stack area
		else
			rfacts += 1; // total factor of right hand stack area
	}

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
		if (!m->nmaster || n < m->nmaster)
			mtotal += mh / mfacts;
		else if ((n - m->nmaster) % 2)
			ltotal += lh / lfacts;
		else
			rtotal += rh / rfacts;
	#endif // CFACTS_PATCH

	mrest = mh - mtotal;
	lrest = lh - ltotal;
	rrest = rh - rtotal;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			#if CFACTS_PATCH
			resize(c, mx, my, mw - (2*c->bw), (mh / mfacts) * c->cfact + (i < mrest ? 1 : 0) - (2*c->bw), 0);
			#else
			resize(c, mx, my, mw - (2*c->bw), (mh / mfacts) + (i < mrest ? 1 : 0) - (2*c->bw), 0);
			#endif // CFACTS_PATCH
			#if VANITYGAPS_PATCH
			my += HEIGHT(c) + ih;
			#else
			my += HEIGHT(c);
			#endif
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				#if CFACTS_PATCH
				resize(c, lx, ly, lw - (2*c->bw), (lh / lfacts) * c->cfact + ((i - 2*m->nmaster) < 2*lrest ? 1 : 0) - (2*c->bw), 0);
				#else
				resize(c, lx, ly, lw - (2*c->bw), (lh / lfacts) + ((i - 2*m->nmaster) < 2*lrest ? 1 : 0) - (2*c->bw), 0);
				#endif // CFACTS_PATCH
				#if VANITYGAPS_PATCH
				ly += HEIGHT(c) + ih;
				#else
				ly += HEIGHT(c);
				#endif
			} else {
				#if CFACTS_PATCH
				resize(c, rx, ry, rw - (2*c->bw), (rh / rfacts) * c->cfact + ((i - 2*m->nmaster) < 2*rrest ? 1 : 0) - (2*c->bw), 0);
				#else
				resize(c, rx, ry, rw - (2*c->bw), (rh / rfacts) + ((i - 2*m->nmaster) < 2*rrest ? 1 : 0) - (2*c->bw), 0);
				#endif // CFACTS_PATCH
				#if VANITYGAPS_PATCH
				ry += HEIGHT(c) + ih;
				#else
				ry += HEIGHT(c);
				#endif
			}
		}
	}
}

