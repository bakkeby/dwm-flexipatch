#if VANITYGAPS_PATCH
static void
fibonacci(Monitor *m, int s)
{
	unsigned int i, n;
	int nx, ny, nw, nh;
	int oh, ov, ih, iv;
	#if CFACTS_PATCH
	float mfacts, sfacts;
	#endif // CFACTS_PATCH
	Client *c;

	#if CFACTS_PATCH
	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);
	#else
	getgaps(m, &oh, &ov, &ih, &iv, &n);
	#endif // CFACTS_PATCH

	if (n == 0)
		return;

	nx = m->wx + ov;
	ny = oh;
	nw = m->ww - 2*ov;
	nh = m->wh - 2*oh;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2*c->bw)
		   || (!(i % 2) && nw / 2 > 2*c->bw)) {
			if (i < n - 1) {
				if (i % 2)
					nh = (nh - ih) / 2;
				else
					nw = (nw - iv) / 2;

				if ((i % 4) == 2 && !s)
					nx += nw + iv;
				else if ((i % 4) == 3 && !s)
					ny += nh + ih;
			}
			if ((i % 4) == 0) {
				if (s)
					ny += nh + ih;
				else
					ny -= nh + ih;
			}
			else if ((i % 4) == 1)
				nx += nw + iv;
			else if ((i % 4) == 2)
				ny += nh + ih;
			else if ((i % 4) == 3) {
				if (s)
					nx += nw + iv;
				else
					nx -= nw + iv;
			}
			if (i == 0)	{
				if (n != 1)
					nw = (m->ww - 2*ov - iv) * m->mfact;
				ny = m->wy + oh;
			}
			else if (i == 1)
				nw = m->ww - nw - iv - 2*ov;
			i++;
		}

		resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), False);
	}
}
#else
void
fibonacci(Monitor *mon, int s)
{
	unsigned int i, n, nx, ny, nw, nh;
	Client *c;

	for (n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	nx = mon->wx;
	ny = 0;
	nw = mon->ww;
	nh = mon->wh;

	for (i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if (i < n - 1) {
				if (i % 2)
					nh /= 2;
				else
					nw /= 2;
				if ((i % 4) == 2 && !s)
					nx += nw;
				else if ((i % 4) == 3 && !s)
					ny += nh;
			}
			if ((i % 4) == 0) {
				if(s)
					ny += nh;
				else
					ny -= nh;
			}
			else if ((i % 4) == 1)
				nx += nw;
			else if ((i % 4) == 2)
				ny += nh;
			else if ((i % 4) == 3) {
				if (s)
					nx += nw;
				else
					nx -= nw;
			}
			if (i == 0)
			{
				if (n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy;
			}
			else if (i == 1)
				nw = mon->ww - nw;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}
#endif

#if FIBONACCI_DWINDLE_LAYOUT
static void
dwindle(Monitor *m)
{
	fibonacci(m, 1);
}
#endif

#if FIBONACCI_SPIRAL_LAYOUT
static void
spiral(Monitor *m)
{
	fibonacci(m, 0);
}
#endif