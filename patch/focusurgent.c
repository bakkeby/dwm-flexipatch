void
focusurgent(const Arg *arg)
{
	Client *c;
	int i;
	for (c=selmon->clients; c && !c->isurgent; c=c->next);
	if (c) {
		for (i=0; i < NUMTAGS && !((1 << i) & c->tags); i++);
		if (i < NUMTAGS) {
			const Arg a = {.ui = 1 << i};
			view(&a);
			focus(c);
		}
	}
}