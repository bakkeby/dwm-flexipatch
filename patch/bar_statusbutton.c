int
width_stbutton(Bar *bar, BarArg *a)
{
	return TEXTW(buttonbar);
}

int
draw_stbutton(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, a->y, a->w, a->h, lrpad / 2, buttonbar, 0, False);
}

int
click_stbutton(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkButton;
}

