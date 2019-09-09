#if VANITYGAPS_PATCH && CFACTS_PATCH
void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	int oh, ov, ih, iv;
	float mfacts = 0, lfacts = 0, rfacts = 0;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);
	if (n == 0)
		return;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mfacts += c->cfact; // total factor of master area
		else if ((n - m->nmaster) % 2)
			lfacts += c->cfact; // total factor of left hand stacke area
		else
			rfacts += c->cfact; // total factor of right hand stack area
	}

	/* initialize areas */
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

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				resize(c, lx, ly, lw - (2*c->bw), lh * (c->cfact / lfacts) - (2*c->bw), 0);
				ly += HEIGHT(c) + ih;
			} else {
				resize(c, rx, ry, rw - (2*c->bw), rh * (c->cfact / rfacts) - (2*c->bw), 0);
				ry += HEIGHT(c) + ih;
			}
		}
	}
}
#elif VANITYGAPS_PATCH
void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	int oh, ov, ih, iv;
	int mn = 0, ln = 0, rn = 0; // number of clients in master, left and right area
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mn += 1;
		else if ((n - m->nmaster) % 2)
			ln += 1; // total factor of left hand stacke area
		else
			rn += 1; // total factor of right hand stack area
	}

	/* initialize areas */
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

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			resize(c, mx, my, mw - (2*c->bw), mh / mn - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				resize(c, lx, ly, lw - (2*c->bw), lh / ln - (2*c->bw), 0);
				ly += HEIGHT(c) + ih;
			} else {
				resize(c, rx, ry, rw - (2*c->bw), rh / rn - (2*c->bw), 0);
				ry += HEIGHT(c) + ih;
			}
		}
	}
}
#elif CFACTS_PATCH
void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	float mfacts = 0, lfacts = 0, rfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mfacts += c->cfact; // total factor of master area
		else if ((n - m->nmaster) % 2)
			lfacts += c->cfact; // total factor of left hand stacke area
		else
			rfacts += c->cfact; // total factor of right hand stack area
	}

	if (n == 0)
		return;

	/* initialize areas */
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

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c);
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				resize(c, lx, ly, lw - (2*c->bw), lh * (c->cfact / lfacts) - (2*c->bw), 0);
				ly += HEIGHT(c);
			} else {
				resize(c, rx, ry, rw - (2*c->bw), rh * (c->cfact / rfacts) - (2*c->bw), 0);
				ry += HEIGHT(c);
			}
		}
	}
}
#else
void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	int mn = 0, ln = 0, rn = 0; // number of clients in master, left and right area
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mn += 1;
		else if ((n - m->nmaster) % 2)
			ln += 1; // total factor of left hand stacke area
		else
			rn += 1; // total factor of right hand stack area
	}

	if (n == 0)
		return;

	/* initialize areas */
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

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			resize(c, mx, my, mw - (2*c->bw), mh / mn - (2*c->bw), 0);
			my += HEIGHT(c);
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				resize(c, lx, ly, lw - (2*c->bw), lh / ln - (2*c->bw), 0);
				ly += HEIGHT(c);
			} else {
				resize(c, rx, ry, rw - (2*c->bw), rh / rn - (2*c->bw), 0);
				ry += HEIGHT(c);
			}
		}
	}
}
#endif