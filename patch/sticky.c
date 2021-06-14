void
togglesticky(const Arg *arg)
{
	if (!selmon->sel)
		return;
	selmon->sel->issticky = !selmon->sel->issticky;
	arrange(selmon);
}

