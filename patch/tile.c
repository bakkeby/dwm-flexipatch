#if VANITYGAPS_PATCH && CFACTS_PATCH
static void
tile(Monitor *m)
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
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (c->cfact / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
}
#elif VANITYGAPS_PATCH
static void
tile(Monitor *m)
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
			resize(c, mx, my, mw - (2*c->bw), mh / MIN(n, m->nmaster) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh / (n - MIN(n, m->nmaster)) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
}
#elif CFACTS_PATCH
static void
tile(Monitor *m)
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
		sw = mw * (1 - m->mfact);
		mw = mw * m->mfact;
		sx = mx + mw;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c);
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (c->cfact / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c);
		}
}
#else
void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww;
	for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
			my += HEIGHT(c);
		} else {
			h = (m->wh - ty) / (n - i);
			resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
			ty += HEIGHT(c);
		}
}
#endif