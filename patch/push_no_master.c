Client *
prevt(Client *c)
{
	Client *p, *r;

	for (p = selmon->clients, r = NULL; p && p != c; p = p->next)
		if (!p->isfloating && ISVISIBLE(p))
			r = p;
	return r;
}

void
pushup(const Arg *arg)
{
	Client *sel = selmon->sel, *c;

	if (!sel || sel->isfloating)
		return;
	if ((c = prevt(sel)) && c != nexttiled(selmon->clients)) {
		detach(sel);
		sel->next = c;
		for (c = selmon->clients; c->next != sel->next; c = c->next);
		c->next = sel;
	}
	focus(sel);
	arrange(selmon);
}

void
pushdown(const Arg *arg)
{
	Client *sel = selmon->sel, *c;

	if (!sel || sel->isfloating || sel == nexttiled(selmon->clients))
		return;
	if ((c = nexttiled(sel->next))) {
		detach(sel);
		sel->next = c->next;
		c->next = sel;
	}
	focus(sel);
	arrange(selmon);
}

