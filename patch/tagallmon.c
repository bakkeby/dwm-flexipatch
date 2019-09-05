void
tagallmon(const Arg *arg)
{
	Monitor *m;
	Client *c;
	Client *next;

	if (!mons->next)
		return;

	m = dirtomon(arg->i);
	for (c = selmon->clients; c; c = next) {
		next = c->next;
		if (!ISVISIBLE(c))
			continue;
		unfocus(c, 1);
		detach(c);
		detachstack(c);
		c->mon = m;
		c->tags = m->tagset[m->seltags]; /* assign tags of target monitor */
		attach(c);
		attachstack(c);
		if (c->isfullscreen) {
			setfullscreen(c, 0);
			setfullscreen(c, 1);
		}
	}

	focus(NULL);
	arrange(NULL);
}