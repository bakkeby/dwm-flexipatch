void
warp(const Client *c)
{
	Monitor *m;
	Bar *bar;
	int x, y;

	if (ignore_warp)
		return;

	if (!c) {
		XWarpPointer(dpy, None, root, 0, 0, 0, 0, selmon->wx + selmon->ww / 2, selmon->wy + selmon->wh / 2);
		return;
	}

	if (!force_warp) {
		if (!getrootptr(&x, &y) ||
			(x > c->x - c->bw &&
			 y > c->y - c->bw &&
			 x < c->x + c->w + c->bw*2 &&
			 y < c->y + c->h + c->bw*2)
		)
			return;

		for (m = mons; m; m = m->next)
			for (bar = m->bar; bar; bar = bar->next)
				if (x > bar->bx &&
					x < bar->bx + bar->bw &&
					y > bar->by &&
					y < bar->by + bar->bh)
					return;
	} else
		force_warp = 0;

	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w / 2, c->h / 2);
}
