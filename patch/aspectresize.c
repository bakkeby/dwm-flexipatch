void
aspectresize(const Arg *arg)
{
	/* only floating windows can be moved */
	Client *c;
	c = selmon->sel;
	float ratio;
	int w, h,nw, nh;

	if (!c || !arg)
		return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		return;

	ratio = (float)c->w / (float)c->h;
	h = arg->i;
	w = (int)(ratio * h);

	nw = c->w + w;
	nh = c->h + h;

	XRaiseWindow(dpy, c->win);
	resize(c, c->x, c->y, nw, nh, True);
}

