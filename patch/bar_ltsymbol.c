int
width_ltsymbol(Bar *bar, BarArg *a)
{
	return TEXTW(bar->mon->ltsymbol);
}

int
draw_ltsymbol(Bar *bar, BarArg *a)
{
	#if BAR_LTSYMBOL_SCHEME_PATCH
	drw_setscheme(drw, scheme[SchemeLtSymbol]);
	#endif // BAR_LTSYMBOL_SCHEME_PATCH
	return drw_text(drw, a->x, a->y, a->w, a->h, lrpad / 2, bar->mon->ltsymbol, 0, False);
}

int
click_ltsymbol(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkLtSymbol;
}
