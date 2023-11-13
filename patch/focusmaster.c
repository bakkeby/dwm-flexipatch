void
focusmaster(const Arg *arg)
{
	Client *master;
	Monitor *m = selmon;
	#if FOCUSMASTER_RETURN_PATCH
	int i;
	#endif // FOCUSMASTER_RETURN_PATCH

	if (m->nmaster < 1)
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (!m->sel || (m->sel->isfullscreen && m->sel->fakefullscreen != 1 && lockfullscreen))
		return;
	#else
	if (!m->sel || (m->sel->isfullscreen && lockfullscreen))
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // FAKEFULLSCREEN_PATCH

	master = nexttiled(m->clients);

	if (!master)
		return;

	#if FOCUSMASTER_RETURN_PATCH
	for (i = 0; !(m->tagset[m->seltags] & 1 << i); i++);
	i++;

	if (m->sel == master) {
		if (m->tagmarked[i] && ISVISIBLE(m->tagmarked[i]))
			focus(m->tagmarked[i]);
	} else {
		m->tagmarked[i] = m->sel;
		focus(master);
	}
	#else
	focus(master);
	#endif // FOCUSMASTER_RETURN_PATCH
	restack(m);
}
