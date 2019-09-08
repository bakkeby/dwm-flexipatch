#if VANITYGAPS_PATCH && CFACTS_PATCH
static void
bstack(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv * (MIN(n, m->nmaster) - 1);

	if (m->nmaster && n > m->nmaster) {
		sh = (mh - ih) * (1 - m->mfact);
		mh = (mh - ih) * m->mfact;
		sx = mx;
		sy = my + mh + ih;
		sw = m->ww - 2*ov - iv * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw * (c->cfact / sfacts) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
	}
}
#elif VANITYGAPS_PATCH
static void
bstack(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv * (MIN(n, m->nmaster) - 1);

	if (m->nmaster && n > m->nmaster) {
		sh = (mh - ih) * (1 - m->mfact);
		mh = (mh - ih) * m->mfact;
		sy = my + mh + ih;
		sw = m->ww - 2*ov - iv * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw / MIN(n, m->nmaster) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw / (n - MIN(n, m->nmaster)) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
	}
}
#elif CFACTS_PATCH
static void
bstack(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	float mfacts = 1, sfacts = 1;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}

	if (n == 0)
		return;

	sx = mx = m->wx;
	sy = my = m->wy;
	sh = mh = m->wh;
	sw = mw = m->ww;

	if (m->nmaster && n > m->nmaster) {
		sh = mh * (1 - m->mfact);
		mh = mh * m->mfact;
		sy = my + mh;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c);
		} else {
			resize(c, sx, sy, sw * (c->cfact / sfacts) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c);
		}
	}
}
#else
static void
bstack(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	sx = mx = m->wx;
	sy = my = m->wy;
	sh = mh = m->wh;
	sw = mw = m->ww;

	if (m->nmaster && n > m->nmaster) {
		sh = mh * (1 - m->mfact);
		mh = mh * m->mfact;
		sy = my + mh;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw / MIN(n, m->nmaster) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c);
		} else {
			resize(c, sx, sy, sw / (n - MIN(n, m->nmaster)) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c);
		}
	}
}
#endif