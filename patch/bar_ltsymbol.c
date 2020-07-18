int
width_ltsymbol(Bar *bar, BarWidthArg *a)
{
	return TEXTW(bar->mon->ltsymbol);
}

int
draw_ltsymbol(Bar *bar, BarDrawArg *a)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, bar->mon->ltsymbol, 0, False);
	#else
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, bar->mon->ltsymbol, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_ltsymbol(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkLtSymbol;
}