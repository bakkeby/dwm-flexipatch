int
width_ltsymbol(Monitor *m, BarWidthArg *a)
{
	return TEXTW(m->ltsymbol);
}

int
draw_ltsymbol(Monitor *m, BarDrawArg *a)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, m->ltsymbol, 0, False);
	#else
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, m->ltsymbol, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_ltsymbol(Monitor *m, Arg *arg, BarClickArg *a)
{
	return ClkLtSymbol;
}