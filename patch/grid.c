#if VANITYGAPS_PATCH
static void
grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, cols, rows;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	/* grid dimensions */
	for (rows = 0; rows <= n/2; rows++)
		if (rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (m->wh - 2*oh - ih * (rows - 1)) / (rows ? rows : 1);
	cw = (m->ww - 2*ov - iv * (cols - 1)) / (cols ? cols : 1);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		cx = m->wx + (i / rows) * (cw + iv) + ov;
		cy = m->wy + (i % rows) * (ch + ih) + oh;
		resize(c, cx, cy, cw - 2*c->bw, ch - 2*c->bw, False);
	}
}
#else
static void
grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, cols, rows;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);

	/* grid dimensions */
	for (rows = 0; rows <= n/2; rows++)
		if (rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		resize(c, cx, cy, cw - 2*c->bw, ch - 2*c->bw, False);
	}
}
#endif