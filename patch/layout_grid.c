#if VANITYGAPS_PATCH
void
grid(Monitor *m)
{
	unsigned int i, n;
	int cx, cy, cw, ch, cc, cr, chrest, cwrest, cols, rows;
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
	chrest = (m->wh - 2*oh - ih * (rows - 1)) - ch * rows;
	cwrest = (m->ww - 2*ov - iv * (cols - 1)) - cw * cols;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		cc = i / rows;
		cr = i % rows;
		cx = m->wx + ov + cc * (cw + iv) + MIN(cc, cwrest);
		cy = m->wy + oh + cr * (ch + ih) + MIN(cr, chrest);
		resize(c, cx, cy, cw + (cc < cwrest ? 1 : 0) - 2*c->bw, ch + (cr < chrest ? 1 : 0) - 2*c->bw, False);
	}
}
#else
void
grid(Monitor *m)
{
	unsigned int i, n;
	int cx, cy, cw, ch, cc, cr, chrest, cwrest, cols, rows;
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
	chrest = m->wh - ch * rows;
	cwrest = m->ww - cw * cols;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		cc = i / rows;
		cr = i % rows;
		cx = m->wx + cc * cw + MIN(cc, cwrest);
		cy = m->wy + cr * ch + MIN(cr, chrest);
		resize(c, cx, cy, cw + (cc < cwrest ? 1 : 0) - 2*c->bw, ch + (cr < chrest ? 1 : 0) - 2*c->bw, False);
	}
}
#endif

