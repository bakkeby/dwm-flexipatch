void
togglenomodbuttons(const Arg *arg)
{
	nomodbuttons = !nomodbuttons;
	if (selmon->sel)
		grabbuttons(selmon->sel, 1);
}

