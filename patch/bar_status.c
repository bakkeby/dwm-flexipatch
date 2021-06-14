int
width_status(Bar *bar, BarArg *a)
{
	return TEXTWM(stext);
}

#if BAR_EXTRASTATUS_PATCH
int
width_status_es(Bar *bar, BarArg *a)
{
	return TEXTWM(estext) - lrpad;
}
#endif // BAR_EXTRASTATUS_PATCH

int
draw_status(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, a->y, a->w, a->h, lrpad / 2, stext, 0, True);
}

#if BAR_EXTRASTATUS_PATCH
int
draw_status_es(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, a->y, a->w, a->h, 0, estext, 0, True);
}
#endif // BAR_EXTRASTATUS_PATCH

int
click_status(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkStatusText;
}

