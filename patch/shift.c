static Arg
shift(const Arg *arg, int clients)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	shifted.ui = selmon->tagset[selmon->seltags];

	#if TAGSYNC_PATCH
	Monitor *origselmon = selmon;
	for (selmon = mons; selmon; selmon = selmon->next)
	#endif // TAGSYNC_PATCH
	for (c = selmon->clients; c && clients; c = c->next) {
		if (c == selmon->sel)
			continue;
		#if SCRATCHPAD_ALT_1_PATCH
		if (!(c->tags & SCRATCHPAD_MASK))
			tagmask |= c->tags;
		#else
		tagmask |= c->tags;
		#endif // SCRATCHPAD_ALT_1_PATCH
	}
	#if TAGSYNC_PATCH
	selmon = origselmon;
	#endif // TAGSYNC_PATCH

	do {
		if (arg->i > 0) // left circular shift
			shifted.ui = (shifted.ui << arg->i) | (shifted.ui >> (NUMTAGS - arg->i));
		else // right circular shift
			shifted.ui = (shifted.ui >> -arg->i) | (shifted.ui << (NUMTAGS + arg->i));
	} while (tagmask && !(shifted.ui & tagmask));

	return shifted;
}
