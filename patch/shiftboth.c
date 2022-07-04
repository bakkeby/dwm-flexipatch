void
shiftboth(const Arg *arg)
{
	Arg shifted;
	#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
	shifted.ui = selmon->tagset[selmon->seltags];
	#else
	shifted.ui = selmon->tagset[selmon->seltags];
	#endif // SCRATCHPADS_PATCH

	if (arg->i > 0) /* left circular shift */
		shifted.ui = ((shifted.ui << arg->i) | (shifted.ui >> (NUMTAGS - arg->i)));
	else /* right circular shift */
		shifted.ui = ((shifted.ui >> -arg->i) | (shifted.ui << (NUMTAGS + arg->i)));
	#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
	shifted.ui &= ~SPTAGMASK;
	#endif // SCRATCHPADS_PATCH

	tag(&shifted);
	view(&shifted);
}
