void
shiftviewclients(const Arg *arg)
{
	Arg shifted;
	Client *c;
	unsigned int tagmask = 0;

	for (c = selmon->clients; c; c = c->next)
		#if SCRATCHPADS_PATCH
		if (!(c->tags & SPTAGMASK))
			tagmask = tagmask | c->tags;
		#elif SCRATCHPAD_ALT_1_PATCH
		if (!(c->tags & SCRATCHPAD_MASK))
			tagmask = tagmask | c->tags;
		#else
		tagmask = tagmask | c->tags;
		#endif // SCRATCHPADS_PATCH

	#if SCRATCHPADS_PATCH
	shifted.ui = selmon->tagset[selmon->seltags] & ~SPTAGMASK;
	#else
	shifted.ui = selmon->tagset[selmon->seltags];
	#endif // SCRATCHPADS_PATCH
	if (arg->i > 0) // left circular shift
		do {
			shifted.ui = (shifted.ui << arg->i)
			   | (shifted.ui >> (NUMTAGS - arg->i));
			#if SCRATCHPADS_PATCH
			shifted.ui &= ~SPTAGMASK;
			#endif // SCRATCHPADS_PATCH
		} while (tagmask && !(shifted.ui & tagmask));
	else // right circular shift
		do {
			shifted.ui = (shifted.ui >> (- arg->i)
			   | shifted.ui << (NUMTAGS + arg->i));
			#if SCRATCHPADS_PATCH
			shifted.ui &= ~SPTAGMASK;
			#endif // SCRATCHPADS_PATCH
		} while (tagmask && !(shifted.ui & tagmask));

	view(&shifted);
}

