void
swaptags(const Arg *arg)
{
	Client *c;
	unsigned int newtag = arg->ui & TAGMASK;
	unsigned int curtag = selmon->tagset[selmon->seltags];

	if (newtag == curtag || !curtag || (curtag & (curtag-1)))
		return;

	#if TAGSYNC_PATCH
	Monitor *origselmon = selmon;
	for (selmon = mons; selmon; selmon = selmon->next) {
	#endif // TAGSYNC_PATCH

	for (c = selmon->clients; c != NULL; c = c->next) {
		if ((c->tags & newtag) || (c->tags & curtag))
			c->tags ^= curtag ^ newtag;

		if (!c->tags)
				c->tags = newtag;
	}

	#if TAGSYNC_PATCH
	}
	selmon = origselmon;
	#endif // TAGSYNC_PATCH

	view(&((Arg) { .ui = newtag }));
}
