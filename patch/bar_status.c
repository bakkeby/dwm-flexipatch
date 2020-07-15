int
width_status(Monitor *m, int max_width)
{
	#if BAR_PANGO_PATCH
	return TEXTWM(stext) - lrpad + 2; /* 2px right padding */
	#else
	return TEXTW(stext) - lrpad + 2; /* 2px right padding */
	#endif // BAR_PANGO_PATCH
}

int
draw_status(Monitor *m, int x, int w)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, x, 0, w, bh, 0, stext, 0, True);
	#else
	return drw_text(drw, x, 0, w, bh, 0, stext, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_status(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h)
{
	return ClkStatusText;
}
