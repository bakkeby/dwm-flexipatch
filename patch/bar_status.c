int
width_status(Bar *bar, BarWidthArg *a)
{
	#if BAR_PANGO_PATCH
	return TEXTWM(stext);
	#else
	return TEXTW(stext);
	#endif // BAR_PANGO_PATCH
}

int
draw_status(Bar *bar, BarDrawArg *a)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, stext, 0, True);
	#else
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, stext, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_status(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkStatusText;
}
