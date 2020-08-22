int
width_status(Bar *bar, BarWidthArg *a)
{
	return TEXTWM(stext);
}

#if BAR_EXTRASTATUS_PATCH
int
width_status_es(Bar *bar, BarWidthArg *a)
{
	return TEXTWM(estext) - lrpad;
}
#endif // BAR_EXTRASTATUS_PATCH

int
draw_status(Bar *bar, BarDrawArg *a)
{
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, stext, 0, True);
}

#if BAR_EXTRASTATUS_PATCH
int
draw_status_es(Bar *bar, BarDrawArg *a)
{
	return drw_text(drw, a->x, 0, a->w, bh, 0, estext, 0, True);
}
#endif // BAR_EXTRASTATUS_PATCH

int
click_status(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkStatusText;
}
