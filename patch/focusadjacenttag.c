void
tagtoleft(const Arg *arg)
{
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		focus(NULL);
		arrange(selmon);
	}
}

void
tagtoright(const Arg *arg)
{
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
		selmon->sel->tags <<= 1;
		focus(NULL);
		arrange(selmon);
	}
}

void
viewtoleft(const Arg *arg)
{
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->seltags ^= 1; /* toggle sel tagset */
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] >> 1;
		focus(NULL);
		arrange(selmon);
	}
}

void
viewtoright(const Arg *arg)
{
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
		selmon->seltags ^= 1; /* toggle sel tagset */
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] << 1;
		focus(NULL);
		arrange(selmon);
	}
}

void
tagandviewtoleft(const Arg *arg)
{
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		selmon->seltags ^= 1; /* toggle sel tagset */
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] >> 1;
		focus(selmon->sel);
		arrange(selmon);
	}
}

void
tagandviewtoright(const Arg *arg)
{
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
		selmon->sel->tags <<= 1;
		selmon->seltags ^= 1; /* toggle sel tagset */
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] << 1;
		focus(selmon->sel);
		arrange(selmon);
	}
}