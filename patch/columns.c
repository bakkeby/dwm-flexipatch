#if VANITYGAPS_PATCH && CFACTS_PATCH
static void
col(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);
	getfacts(m, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sh = m->wh - 2*oh - ih * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (c->cfact / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
}
#elif VANITYGAPS_PATCH
static void
col(Monitor *m)
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
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sh = m->wh - 2*oh - ih * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw / MIN(n, m->nmaster) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh / (n - MIN(n, m->nmaster)) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
}
#elif CFACTS_PATCH
static void
col(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	float mfacts, sfacts;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	getfacts(m, &mfacts, &sfacts);

	sx = mx = m->wx;
	sy = my = m->wy;
	sh = mh = m->wh;
	sw = mw = m->ww;

	if (m->nmaster && n > m->nmaster) {
		sw = mw * (1 - m->mfact);
		mw = mw * m->mfact;
		sx = mx + mw;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c);
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (c->cfact / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c);
		}
}
#else
void
col(Monitor *m) {
	unsigned int i, n, h, w, x, y,mw;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
        if (n > m->nmaster)
                mw = m->nmaster ? m->ww * m->mfact : 0;
        else
                mw = m->ww;
	for (i = x = y = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			 w = (mw - x) / (MIN(n, m->nmaster)-i);
                         resize(c, x + m->wx, m->wy, w - (2*c->bw), m->wh - (2*c->bw), False);
			x += WIDTH(c);
		}
		else {
			h = (m->wh - y) / (n - i);
			resize(c, x + m->wx, m->wy + y, m->ww - x  - (2*c->bw), h - (2*c->bw), False);
			y += HEIGHT(c);
		}
	}
}
#endif