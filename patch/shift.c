static Arg
shift(const Arg *arg, int clients)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
	shifted.ui = selmon->tagset[selmon->seltags];
	#else
	shifted.ui = selmon->tagset[selmon->seltags];
	#endif // SCRATCHPADS_PATCH

	#if TAGSYNC_PATCH
	Monitor *origselmon = selmon;
	for (selmon = mons; selmon; selmon = selmon->next)
	#endif // TAGSYNC_PATCH
	for (c = selmon->clients; c && clients; c = c->next) {
		if (c == selmon->sel)
			continue;
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if (!(c->tags & SPTAGMASK))
			tagmask |= c->tags;
		#elif SCRATCHPAD_ALT_1_PATCH
		if (!(c->tags & SCRATCHPAD_MASK))
			tagmask |= c->tags;
		#else
		tagmask |= c->tags;
		#endif // SCRATCHPADS_PATCH
	}
	#if TAGSYNC_PATCH
	selmon = origselmon;
	#endif // TAGSYNC_PATCH

	do {
		if (arg->i > 0) // left circular shift
			shifted.ui = (shifted.ui << arg->i) | (shifted.ui >> (NUMTAGS - arg->i));
		else // right circular shift
			shifted.ui = (shifted.ui >> -arg->i) | (shifted.ui << (NUMTAGS + arg->i));
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		shifted.ui &= ~SPTAGMASK;
		#endif // SCRATCHPADS_PATCH
	} while (tagmask && !(shifted.ui & tagmask));

	return shifted;
}
