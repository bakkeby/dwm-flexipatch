Layout *last_layout;

void
fullscreen(const Arg *arg)
{
	if (selmon->showbar) {
		for (last_layout = (Layout *)layouts; last_layout != selmon->lt[selmon->sellt]; last_layout++);
		setlayout(&((Arg) { .v = &layouts[2] })); // <-- NB! hardcoded monocle
	} else {
		setlayout(&((Arg) { .v = last_layout }));
	}
	togglebar(arg);
}