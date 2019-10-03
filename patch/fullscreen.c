Layout *last_layout = &layouts[0];

void
fullscreen(const Arg *arg)
{
	if (selmon->showbar) {
		for (last_layout = (Layout *)layouts; last_layout != selmon->lt[selmon->sellt]; last_layout++);
		setlayout(&((Arg) { .v = &layouts[MONOCLE_LAYOUT_POS] }));
	} else {
		setlayout(&((Arg) { .v = last_layout }));
	}
	togglebar(arg);
}