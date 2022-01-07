static Client *
nextc(Client *c, float f)
{
	if (!f)
		return nexttiled(c);

	for (; c && !ISVISIBLE(c); c = c->next);
	return c;
}

static Client *
prevc(Client *c, float f)
{
	Client *p, *r;

	for (p = selmon->clients, r = NULL; c && p && p != c; p = p->next)
		if ((f || !p->isfloating) && ISVISIBLE(p))
			r = p;
	return r;
}

static void
pushup(const Arg *arg)
{
	Client *sel = selmon->sel;
	Client *c;

	if (!sel || (sel->isfloating && !arg->f))
		return;
	if ((c = prevc(sel, arg->f))) {
		/* attach before c */
		detach(sel);
		sel->next = c;
		if (selmon->clients == c)
			selmon->clients = sel;
		else {
			for (c = selmon->clients; c->next != sel->next; c = c->next);
			c->next = sel;
		}
	} else {
		/* move to the end */
		for (c = sel; c->next; c = c->next);
		if (sel != c) {
			detach(sel);
			sel->next = NULL;
			c->next = sel;
		}
	}
	focus(sel);
	arrange(selmon);
}

static void
pushdown(const Arg *arg)
{
	Client *sel = selmon->sel;
	Client *c;

	if (!sel || (sel->isfloating && !arg->f))
		return;
	if ((c = nextc(sel->next, arg->f))) {
		/* attach after c */
		detach(sel);
		sel->next = c->next;
		c->next = sel;
	} else {
		/* move to the front */
		detach(sel);
		attach(sel);
	}
	focus(sel);
	arrange(selmon);
}

