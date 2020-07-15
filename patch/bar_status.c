int
width_status(Monitor *m, BarWidthArg *a)
{
	#if BAR_PANGO_PATCH
	return TEXTWM(stext) - lrpad + 2; /* 2px right padding */
	#else
	return TEXTW(stext) - lrpad + 2; /* 2px right padding */
	#endif // BAR_PANGO_PATCH
}

int
draw_status(Monitor *m, BarDrawArg *a)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, a->x, 0, a->w, bh, 0, stext, 0, True);
	#else
	return drw_text(drw, a->x, 0, a->w, bh, 0, stext, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_status(Monitor *m, Arg *arg, BarClickArg *a)
{
	return ClkStatusText;
}
