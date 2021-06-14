int
width_wintitle_hidden(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_wintitle_hidden(Bar *bar, BarArg *a)
{
	drw_rect(drw, a->x, a->y, a->w, a->h, 1, 1);
	return calc_wintitle_hidden(bar->mon, a->x, a->w, -1, flextitledraw, NULL, a);
}

int
click_wintitle_hidden(Bar *bar, Arg *arg, BarArg *a)
{
	calc_wintitle_hidden(bar->mon, 0, a->w, a->x, flextitleclick, arg, a);
	return ClkWinTitle;
}

int
calc_wintitle_hidden(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg, BarArg *barg),
	Arg *arg, BarArg *barg
) {
	Client *c;
	int clientsnhidden = 0, w, r;
	int groupactive = GRP_HIDDEN;

	for (c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c))
			continue;
		if (HIDDEN(c))
			clientsnhidden++;
	}

	if (!clientsnhidden)
		return 0;

	w = tabw / clientsnhidden;
	r = tabw % clientsnhidden;
	c = flextitledrawarea(m, m->clients, offx, r, w, clientsnhidden, SCHEMEFOR(GRP_HIDDEN), 0, 1, 0, passx, tabfn, arg, barg);
	return 1;
}

