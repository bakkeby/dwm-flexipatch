int
width_stbutton(Monitor *m, int max_width)
{
	return TEXTW(buttonbar);
}

int
draw_stbutton(Monitor *m, int x, int w)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, x, 0, w, bh, lrpad / 2, buttonbar, 0, False);
	#else
	return drw_text(drw, x, 0, w, bh, lrpad / 2, buttonbar, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_stbutton(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h)
{
	return ClkButton;
}
