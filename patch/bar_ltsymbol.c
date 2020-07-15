int
width_ltsymbol(Monitor *m, int max_width)
{
	return TEXTW(m->ltsymbol);
}

int
draw_ltsymbol(Monitor *m, int x, int w)
{
	#if BAR_PANGO_PATCH
	return drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0, False);
	#else
	return drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0);
	#endif // BAR_PANGO_PATCH
}

int
click_ltsymbol(Monitor *m, Arg *arg, int rel_x, int rel_y, int rel_w, int rel_h)
{
	return ClkLtSymbol;
}