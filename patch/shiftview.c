void
shiftview(const Arg *arg)
{
	Arg shifted;
	#if SCRATCHPADS_PATCH
	unsigned int seltagset = selmon->tagset[selmon->seltags] & ~SPTAGMASK;
	#else
	unsigned int seltagset = selmon->tagset[selmon->seltags];
	#endif // SCRATCHPADS_PATCH
	if (arg->i > 0) // left circular shift
		shifted.ui = (seltagset << arg->i)
		   | (seltagset >> (NUMTAGS - arg->i));
	else // right circular shift
		shifted.ui = seltagset >> -arg->i
		   | seltagset << (NUMTAGS + arg->i);

	view(&shifted);
}

