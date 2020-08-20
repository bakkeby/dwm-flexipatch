int
width_wintitle_hidden(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_wintitle_hidden(Bar *bar, BarDrawArg *a)
{
	drw_rect(drw, a->x, 0, a->w, bh, 1, 1);
	calc_wintitle_hidden(bar->mon, a->x, a->w, -1, flextitledraw, NULL);
	return a->x + a->w;
}

int
click_wintitle_hidden(Bar *bar, Arg *arg, BarClickArg *a)
{
	calc_wintitle_hidden(bar->mon, 0, a->rel_w, a->rel_x, flextitleclick, arg);
	return ClkWinTitle;
}

void
calc_wintitle_hidden(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg), Arg *arg
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
		return;

	w = tabw / clientsnhidden;
	r = tabw % clientsnhidden;
	c = flextitledrawarea(m, m->clients, offx, r, w, clientsnhidden, SCHEMEFOR(GRP_HIDDEN), 0, 1, 0, passx, tabfn, arg);
}