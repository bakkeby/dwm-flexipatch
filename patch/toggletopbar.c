void
toggletopbar(const Arg *arg)
{
	Bar *bar;
	Monitor *m = selmon;

	for (bar = m->bar; bar; bar = bar->next)
		bar->topbar = !bar->topbar;

	if (!m->showbar) {
		togglebar(NULL);
		return;
	}

	updatebarpos(m);

	for (bar = m->bar; bar; bar = bar->next)
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	arrange(m);
}
