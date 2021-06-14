int
width_wintitle_floating(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_wintitle_floating(Bar *bar, BarArg *a)
{
	drw_rect(drw, a->x, a->y, a->w, a->h, 1, 1);
	return calc_wintitle_floating(bar->mon, a->x, a->w, -1, flextitledraw, NULL, a);
}

int
click_wintitle_floating(Bar *bar, Arg *arg, BarArg *a)
{
	calc_wintitle_floating(bar->mon, 0, a->w, a->x, flextitleclick, arg, a);
	return ClkWinTitle;
}

int
calc_wintitle_floating(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg, BarArg *barg),
	Arg *arg, BarArg *barg
) {
	Client *c;
	int clientsnfloating = 0, w, r;
	int groupactive = GRP_FLOAT;

	for (c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c) || HIDDEN(c))
			continue;
		if (c->isfloating)
			clientsnfloating++;
	}

	if (!clientsnfloating)
		return 0;

	w = tabw / clientsnfloating;
	r = tabw % clientsnfloating;
	c = flextitledrawarea(m, m->clients, offx, r, w, clientsnfloating, SCHEMEFOR(GRP_FLOAT), 0, 0, 1, passx, tabfn, arg, barg);
	return 1;
}

