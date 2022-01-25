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
		#if TAGSYNC_PATCH
		Monitor *origselmon = selmon;
		for (selmon = mons; selmon; selmon = selmon->next) {
		#endif // TAGSYNC_PATCH
		selmon->seltags ^= 1; /* toggle sel tagset */
		#if PERTAG_PATCH
		pertagview(&((Arg) { .ui = selmon->tagset[selmon->seltags ^ 1] >> 1 }));
		#else
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] >> 1;
		#endif // pertagview
		#if TAGSYNC_PATCH
		}
		selmon = origselmon;
		#endif // TAGSYNC_PATCH
		focus(NULL);
		#if TAGSYNC_PATCH
		arrange(NULL);
		#else
		arrange(selmon);
		#endif // TAGSYNC_PATCH
		#if BAR_EWMHTAGS_PATCH && TAGSYNC_PATCH
		Monitor *m;
		for (m = mons; m; m = m->next)
			updatemonitordesktop(m);
		#elif BAR_EWMHTAGS_PATCH
		updatecurrentdesktop();
		#endif // BAR_EWMHTAGS_PATCH
	}
}

void
viewtoright(const Arg *arg)
{
	if (__builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
		#if TAGSYNC_PATCH
		Monitor *origselmon = selmon;
		for (selmon = mons; selmon; selmon = selmon->next) {
		#endif // TAGSYNC_PATCH
		selmon->seltags ^= 1; /* toggle sel tagset */
		#if PERTAG_PATCH
		pertagview(&((Arg) { .ui = selmon->tagset[selmon->seltags ^ 1] << 1 }));
		#else
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] << 1;
		#endif // pertagview
		#if TAGSYNC_PATCH
		}
		selmon = origselmon;
		#endif // TAGSYNC_PATCH
		focus(NULL);
		#if TAGSYNC_PATCH
		arrange(NULL);
		#else
		arrange(selmon);
		#endif // TAGSYNC_PATCH
		#if BAR_EWMHTAGS_PATCH && TAGSYNC_PATCH
		Monitor *m;
		for (m = mons; m; m = m->next)
			updatemonitordesktop(m);
		#elif BAR_EWMHTAGS_PATCH
		updatecurrentdesktop();
		#endif // BAR_EWMHTAGS_PATCH
	}
}

void
tagandviewtoleft(const Arg *arg)
{
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] > 1) {
		selmon->sel->tags >>= 1;
		#if TAGSYNC_PATCH
		Monitor *origselmon = selmon;
		for (selmon = mons; selmon; selmon = selmon->next) {
		#endif // TAGSYNC_PATCH
		selmon->seltags ^= 1; /* toggle sel tagset */
		#if PERTAG_PATCH
		pertagview(&((Arg) { .ui = selmon->tagset[selmon->seltags ^ 1] >> 1 }));
		#else
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] >> 1;
		#endif // pertagview
		#if TAGSYNC_PATCH
		}
		selmon = origselmon;
		#endif // TAGSYNC_PATCH
		focus(selmon->sel);
		#if TAGSYNC_PATCH
		arrange(NULL);
		#else
		arrange(selmon);
		#endif // TAGSYNC_PATCH
		#if BAR_EWMHTAGS_PATCH && TAGSYNC_PATCH
		Monitor *m;
		for (m = mons; m; m = m->next)
			updatemonitordesktop(m);
		#elif BAR_EWMHTAGS_PATCH
		updatecurrentdesktop();
		#endif // BAR_EWMHTAGS_PATCH
	}
}

void
tagandviewtoright(const Arg *arg)
{
	if (selmon->sel != NULL
	&& __builtin_popcount(selmon->tagset[selmon->seltags] & TAGMASK) == 1
	&& selmon->tagset[selmon->seltags] & (TAGMASK >> 1)) {
		selmon->sel->tags <<= 1;
		#if TAGSYNC_PATCH
		Monitor *origselmon = selmon;
		for (selmon = mons; selmon; selmon = selmon->next) {
		#endif // TAGSYNC_PATCH
		selmon->seltags ^= 1; /* toggle sel tagset */
		#if PERTAG_PATCH
		pertagview(&((Arg) { .ui = selmon->tagset[selmon->seltags ^ 1] << 1 }));
		#else
		selmon->tagset[selmon->seltags] = selmon->tagset[selmon->seltags ^ 1] << 1;
		#endif // pertagview
		#if TAGSYNC_PATCH
		}
		selmon = origselmon;
		#endif // TAGSYNC_PATCH
		focus(selmon->sel);
		#if TAGSYNC_PATCH
		arrange(NULL);
		#else
		arrange(selmon);
		#endif // TAGSYNC_PATCH
		#if BAR_EWMHTAGS_PATCH && TAGSYNC_PATCH
		Monitor *m;
		for (m = mons; m; m = m->next)
			updatemonitordesktop(m);
		#elif BAR_EWMHTAGS_PATCH
		updatecurrentdesktop();
		#endif // BAR_EWMHTAGS_PATCH
	}
}

