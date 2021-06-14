void
setinset(Monitor *m, Inset inset)
{
	Bar *bar;
	m->inset = inset;
	updatebarpos(m);
	for (bar = m->bar; bar; bar = bar->next)
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	arrange(m);
}

void
updateinset(const Arg *arg)
{
	Inset *inset = (Inset *)arg->v;
	for (Monitor *m = mons; m; m = m->next)
		setinset(m, *inset);
}

