void
movecenter(const Arg *arg)
{
	Monitor *m = selmon;
	Client *c = m->sel;
	XEvent ev;

	if (!c)
		return;

	if (m->lt[m->sellt]->arrange != NULL && !c->isfloating)
		return;

	c->x = m->mx + (m->mw - WIDTH(c)) / 2;
	c->y = m->my + (m->mh - HEIGHT(c)) / 2;
	XMoveWindow(dpy, c->win, c->x, c->y);
	XSync(dpy, False);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
