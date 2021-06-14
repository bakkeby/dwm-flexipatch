void
cyclelayout(const Arg *arg)
{
	Layout *l;
	for (l = (Layout *)layouts; l != selmon->lt[selmon->sellt]; l++);
	if (arg->i > 0) {
		if (l->symbol && (l + 1)->symbol)
			setlayout(&((Arg) { .v = (l + 1) }));
		else
			setlayout(&((Arg) { .v = layouts }));
	} else {
		if (l != layouts && (l - 1)->symbol)
			setlayout(&((Arg) { .v = (l - 1) }));
		else
			setlayout(&((Arg) { .v = &layouts[LENGTH(layouts) - 2] }));
	}
}

