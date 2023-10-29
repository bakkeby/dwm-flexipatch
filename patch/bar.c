void
barhover(XEvent *e, Bar *bar)
{
	const BarRule *br;
	Monitor *m = bar->mon;
	XMotionEvent *ev = &e->xmotion;
	BarArg barg = { 0, 0, 0, 0 };
	int r;

	for (r = 0; r < LENGTH(barrules); r++) {
		br = &barrules[r];
		if (br->bar != bar->idx || (br->monitor == 'A' && m != selmon) || br->hoverfunc == NULL)
			continue;
		if (br->monitor != 'A' && br->monitor != -1 && br->monitor != bar->mon->num)
			continue;
		if (bar->x[r] > ev->x || ev->x > bar->x[r] + bar->w[r])
			continue;

		barg.x = ev->x - bar->x[r];
		barg.y = ev->y - bar->borderpx;
		barg.w = bar->w[r];
		barg.h = bar->bh - 2 * bar->borderpx;

		br->hoverfunc(bar, &barg, ev);
		break;
	}
}

Bar *
wintobar(Window win)
{
	Monitor *m;
	Bar *bar;
	for (m = mons; m; m = m->next)
		for (bar = m->bar; bar; bar = bar->next)
			if (bar->win == win)
				return bar;
	return NULL;
}
