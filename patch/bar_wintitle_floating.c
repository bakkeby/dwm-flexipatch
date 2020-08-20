int
width_wintitle_floating(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_wintitle_floating(Bar *bar, BarDrawArg *a)
{
	drw_rect(drw, a->x, 0, a->w, bh, 1, 1);
	calc_wintitle_floating(bar->mon, a->x, a->w, -1, flextitledraw, NULL);
	return a->x + a->w;
}

int
click_wintitle_floating(Bar *bar, Arg *arg, BarClickArg *a)
{
	calc_wintitle_floating(bar->mon, 0, a->rel_w, a->rel_x, flextitleclick, arg);
	return ClkWinTitle;
}

void
calc_wintitle_floating(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg), Arg *arg
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
		return;

	w = tabw / clientsnfloating;
	r = tabw % clientsnfloating;
	c = flextitledrawarea(m, m->clients, offx, r, w, clientsnfloating, SCHEMEFOR(GRP_FLOAT), 0, 0, 1, passx, tabfn, arg);
}