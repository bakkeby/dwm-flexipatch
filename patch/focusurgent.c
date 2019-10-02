void
focusurgent(const Arg *arg)
{
	Client *c;
	int i;
	for (c=selmon->clients; c && !c->isurgent; c=c->next);
	if (c) {
		for (i=0; i < LENGTH(tags) && !((1 << i) & c->tags); i++);
		if (i < LENGTH(tags)) {
			const Arg a = {.ui = 1 << i};
			view(&a);
			focus(c);
		}
	}
}