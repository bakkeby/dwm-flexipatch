void
togglescratch(const Arg *arg)
{
	Client *c = NULL;
	Monitor *mon;
	unsigned int found = 0;
	unsigned int scratchtag = SPTAG(arg->ui);
	Arg sparg = {.v = scratchpads[arg->ui].cmd};

	for (mon = mons; mon && !found; mon = mon->next)
		for (c = mon->clients; c && !(found = c->tags & scratchtag); c = c->next);

	if (found) {
		if (c->mon != selmon) {
			if (c->mon->tagset[c->mon->seltags] & SPTAGMASK)
				c->mon->tagset[c->mon->seltags] ^= scratchtag;
			sendmon(c, selmon);
		}

		unsigned int newtagset = selmon->tagset[selmon->seltags] ^ scratchtag;
		if (newtagset) {
			selmon->tagset[selmon->seltags] = newtagset;
			focus(NULL);
			arrange(selmon);
		}
		if (ISVISIBLE(c)) {
			focus(c);
			restack(selmon);
		}
	} else {
		selmon->tagset[selmon->seltags] |= scratchtag;
		spawn(&sparg);
	}
}