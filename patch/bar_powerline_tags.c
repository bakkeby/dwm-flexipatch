int
width_pwrl_tags(Bar *bar, BarWidthArg *a)
{
	int w, i;
	int plw = drw->fonts->h / 2 + 1;
	#if BAR_HIDEVACANTTAGS_PATCH
	Client *c;
	unsigned int occ = 0;
	for (c = bar->mon->clients; c; c = c->next)
		occ |= c->tags == 255 ? 0 : c->tags;
	#endif // BAR_HIDEVACANTTAGS_PATCH

	for (w = 0, i = 0; i < LENGTH(tags); i++) {
		#if BAR_HIDEVACANTTAGS_PATCH
		if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i))
			continue;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		#if BAR_ALTERNATIVE_TAGS_PATCH
		w += selmon->alttag ? TEXTW(tagsalt[i]) : TEXTW(tags[i]) + plw;
		#else
		w += TEXTW(tags[i]) + plw;
		#endif // BAR_ALTERNATIVE_TAGS_PATCH
	}
	return w + lrpad;
}

int
draw_pwrl_tags(Bar *bar, BarDrawArg *a)
{
	int x, w;
	int invert;
	int plw = drw->fonts->h / 2 + 1;
	unsigned int i, occ = 0, urg = 0;
	Client *c;
	Clr *prevscheme, *nxtscheme;
	#if !BAR_HIDEVACANTTAGS_PATCH
	#if !BAR_ACTIVETAGINDICATORBAR_PATCH && !BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	int boxs = drw->fonts->h / 9;
	#endif // BAR_ACTIVETAGINDICATORBAR_PATCH | BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	int boxw = drw->fonts->h / 6 + 2;
	#endif // BAR_HIDEVACANTTAGS_PATCH

	for (c = bar->mon->clients; c; c = c->next) {
		#if BAR_HIDEVACANTTAGS_PATCH
		occ |= c->tags == 255 ? 0 : c->tags;
		#else
		occ |= c->tags;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		if (c->isurgent)
			urg |= c->tags;
	}
	x = a->x;
	prevscheme = scheme[SchemeNorm];
	for (i = 0; i < LENGTH(tags); i++) {
		#if BAR_HIDEVACANTTAGS_PATCH
		/* do not draw vacant tags */
		if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i))
			continue;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		#if URGENTBORDER_PATCH
		invert = 0;
		#else
		invert = urg & 1 << i;
		#endif // URGENTBORDER_PATCH
		w = TEXTW(tags[i]);
		drw_settrans(drw, prevscheme, (nxtscheme = scheme[bar->mon->tagset[bar->mon->seltags] & 1 << i ? SchemeSel : SchemeNorm]));
		#if BAR_POWERLINE_TAGS_SLASH_PATCH
		drw_arrow(drw, x, 0, plw, bh, 1, 1);
		#else
		drw_arrow(drw, x, 0, plw, bh, 1, 1);
		#endif // BAR_POWERLINE_TAGS_SLASH_PATCH
		x += plw;
		drw_setscheme(drw, nxtscheme);
		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], invert);
		#if !BAR_HIDEVACANTTAGS_PATCH
		if (occ & 1 << i)
			#if BAR_ACTIVETAGINDICATORBAR_PATCH
			drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw,
			#elif BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
			drw_rect(drw, x + boxw, bh - boxw/2, w - ( 2 * boxw + 1), boxw/2,
			#else
			drw_rect(drw, x + boxs, boxs, boxw, boxw,
			#endif // BAR_ACTIVETAGINDICATORBAR_PATCH
				bar->mon == selmon && selmon->sel && selmon->sel->tags & 1 << i, invert);
		#endif // BAR_HIDEVACANTTAGS_PATCH
		x += w;
		prevscheme = nxtscheme;
	}
	nxtscheme = scheme[SchemeNorm];

	drw_settrans(drw, prevscheme, nxtscheme);
	#if BAR_POWERLINE_TAGS_SLASH_PATCH
	drw_arrow(drw, x, 0, plw, bh, 1, 1);
	#else
	drw_arrow(drw, x, 0, plw, bh, 1, 0);
	#endif // BAR_POWERLINE_TAGS_SLASH_PATCH
	return a->x + a->w;
}

int
click_pwrl_tags(Bar *bar, Arg *arg, BarClickArg *a)
{
	int i = 0, x = lrpad / 2;
	int plw = drw->fonts->h / 2 + 1;
	#if BAR_HIDEVACANTTAGS_PATCH
	Client *c;
	unsigned int occ = 0;
	for (c = bar->mon->clients; c; c = c->next)
		occ |= c->tags == 255 ? 0 : c->tags;
	#endif // BAR_HIDEVACANTTAGS_PATCH

	do {
		#if BAR_HIDEVACANTTAGS_PATCH
		if (!(occ & 1 << i || bar->mon->tagset[bar->mon->seltags] & 1 << i))
			continue;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		#if BAR_ALTERNATIVE_TAGS_PATCH
		x += selmon->alttag ? TEXTW(tagsalt[i]) : TEXTW(tags[i]) + plw;
		#else
		x += TEXTW(tags[i]) + plw;
		#endif
	} while (a->rel_x >= x && ++i < LENGTH(tags));
	if (i < LENGTH(tags)) {
		arg->ui = 1 << i;
	}
	return ClkTagBar;
}