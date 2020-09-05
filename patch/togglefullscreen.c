void
togglefullscreen(const Arg *arg)
{
	if (!selmon->sel)
		return;

	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	if (selmon->sel->fakefullscreen == 1) {
		selmon->sel->fakefullscreen = 2;
		if (selmon->sel->isfullscreen) {
			selmon->sel->isfullscreen = 0;
		}
	} else if (selmon->sel->fakefullscreen == 2) {
		selmon->sel->fakefullscreen = 0;
		togglefakefullscreen(NULL);
		arrange(selmon);
		return;
	}
	#endif // FAKEFULLSCREEN_CLIENT_PATCH

	setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
	if (!selmon->sel->isfullscreen)
		arrange(selmon);
}
