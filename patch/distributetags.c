void
distributetags(const Arg *arg)
{
	unsigned int ui = 1;
	int i = 0;
	for (Client *c = selmon->clients; c; c = c->next) {
		if (HIDDEN(c))
			continue;
		if (!(c->tags & TAGMASK))
			continue;
		c->tags = (ui << i) & TAGMASK;
		i = (i + 1) % NUMTAGS;
	}
	focus(NULL);
	arrange(selmon);
}

