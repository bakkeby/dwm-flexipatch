void
tagtoleft(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		focus(NULL);
		arrange(selmon);
	}
}

void
tagtoright(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] & (MASK >> 1)) {
		selmon->sel->tags <<= 1;
		focus(NULL);
		arrange(selmon);
	}
}

void
viewtoleft(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] >> 1 }));
	}
}

void
viewtoright(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] & (MASK >> 1)) {
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] << 1 }));
	}
}

void
tagandviewtoleft(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] >> 1 }));
	}
}

void
tagandviewtoright(const Arg *arg)
{
	unsigned int MASK = (1 << NUMTAGS) - 1;
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & MASK) == 1
	&& selmon->tagset[selmon->seltags] & (MASK >> 1)) {
		selmon->sel->tags <<= 1;
		view(&((Arg) { .ui = selmon->tagset[selmon->seltags] << 1 }));
	}
}
