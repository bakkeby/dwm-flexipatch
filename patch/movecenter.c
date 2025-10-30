void
movecenter(const Arg *arg)
{
	Client *c = selmon->sel;
	XEvent ev;

	if (!c || !c->isfloating)
		return;

	c->x = c->mon->mx + (c->mon->mw - WIDTH(c)) / 2;
	c->y = c->mon->my + (c->mon->mh - HEIGHT(c)) / 2;
	XMoveWindow(dpy, c->win, c->x, c->y);
	XSync(dpy, False);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
