Layout *last_layout;

void
fullscreen(const Arg *arg)
{
	int monocle_pos = 0;
	if (selmon->showbar || last_layout == NULL) {
		#if MONOCLE_LAYOUT
		for (monocle_pos = 0, last_layout = (Layout *)layouts; !last_layout->arrange || last_layout->arrange != &monocle; monocle_pos++, last_layout++ );
		#endif // MONOCLE_LAYOUT
		for (last_layout = (Layout *)layouts; last_layout != selmon->lt[selmon->sellt]; last_layout++);
		setlayout(&((Arg) { .v = &layouts[monocle_pos] }));
	} else {
		setlayout(&((Arg) { .v = last_layout }));
	}
	togglebar(arg);
}

