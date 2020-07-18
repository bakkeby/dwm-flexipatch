int
width_status_eb(Bar *bar, BarWidthArg *a)
{
	#if BAR_PANGO_PATCH
	return TEXTWM(estext) - lrpad;
	#else
	return TEXTW(estext) - lrpad;
	#endif // BAR_PANGO_PATCH
}

int
draw_status_eb(Bar *bar, BarDrawArg *a)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, a->x, 0, a->w, bh, 0, estext, 0, True);
	#else
	return drw_text(drw, a->x, 0, a->w, bh, 0, estext, 0);
	#endif // BAR_PANGO_PATCH
}