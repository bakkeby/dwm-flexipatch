void
swapfocus(const Arg *arg)
{
	Client *c = NULL;
	Monitor *m = selmon;
	Pertag *p = m->pertag;

	if (!m->sel)
		return;

	if (p->prevclient[p->curtag] != NULL && ISVISIBLE(p->prevclient[p->curtag]))
		c = p->prevclient[p->curtag];

	if (!c)
		for (c = m->sel->next; c && !ISVISIBLE(c); c = c->next);
	if (!c)
		for (c = m->clients; c && !ISVISIBLE(c); c = c->next);

	if (c) {
		focus(c);
		restack(c->mon);
	}
}

void
removeswapfocusclient(Client *c)
{
	Monitor *m;
	Pertag *p;
	int i;

	for (m = mons; m; m = m->next) {
		p = m->pertag;
		for (i = 0; i < NUMTAGS + 1; i++) {
			if (p->prevclient[i] == c) {
				p->prevclient[i] = NULL;
			}
		}
	}
}
