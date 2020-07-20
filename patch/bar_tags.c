int
width_tags(Bar *bar, BarWidthArg *a)
{
	int w, i;
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
		w += selmon->alttag ? TEXTW(tagsalt[i]) : TEXTW(tags[i]);
		#else
		w += TEXTW(tags[i]);
		#endif // BAR_ALTERNATIVE_TAGS_PATCH
	}
	return w;
}

int
draw_tags(Bar *bar, BarDrawArg *a)
{
	int invert;
	int w, x = a->x;
	#if BAR_ALTERNATIVE_TAGS_PATCH
	int wdelta;
	#endif // BAR_ALTERNATIVE_TAGS_PATCH
	#if !BAR_HIDEVACANTTAGS_PATCH
	#if !BAR_ACTIVETAGINDICATORBAR_PATCH && !BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	int boxs = drw->fonts->h / 9;
	#endif // BAR_ACTIVETAGINDICATORBAR_PATCH | BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
	int boxw = drw->fonts->h / 6 + 2;
	#endif // BAR_HIDEVACANTTAGS_PATCH
	unsigned int i, occ = 0, urg = 0;
	Client *c;
	Monitor *m = bar->mon;

	for (c = m->clients; c; c = c->next) {
		#if BAR_HIDEVACANTTAGS_PATCH
		occ |= c->tags == 255 ? 0 : c->tags;
		#else
		occ |= c->tags;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		if (c->isurgent)
			urg |= c->tags;
	}

	for (i = 0; i < LENGTH(tags); i++) {
		#if BAR_HIDEVACANTTAGS_PATCH
		/* do not draw vacant tags */
		if (!(occ & 1 << i || m->tagset[m->seltags] & 1 << i))
			continue;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		#if URGENTBORDER_PATCH
		invert = 0;
		#else
		invert = urg & 1 << i;
		#endif // URGENTBORDER_PATCH
		w = TEXTW(tags[i]);
		#if BAR_ALTERNATIVE_TAGS_PATCH
		wdelta = selmon->alttag ? abs(TEXTW(tags[i]) - TEXTW(tagsalt[i])) / 2 : 0;
		#endif // BAR_ALTERNATIVE_TAGS_PATCH
		#if URGENTBORDER_PATCH
		if (m->tagset[m->seltags] & 1 << i)
			#if BAR_VTCOLORS_PATCH
			drw_setscheme(drw, scheme[SchemeTagsSel]);
			#else
			drw_setscheme(drw, scheme[SchemeSel]);
			#endif // BAR_VTCOLORS_PATCH
		else
			#if BAR_VTCOLORS_PATCH
			drw_setscheme(drw, scheme[urg & 1 << i ? SchemeUrg : SchemeTagsNorm]);
			#else
			drw_setscheme(drw, scheme[urg & 1 << i ? SchemeUrg : SchemeNorm]);
			#endif // BAR_VTCOLORS_PATCH
		#elif BAR_VTCOLORS_PATCH
		drw_setscheme(drw, scheme[m->tagset[m->seltags] & 1 << i ? SchemeTagsSel : SchemeTagsNorm]);
		#else // URGENTBORDER_PATCH
		drw_setscheme(drw, scheme[m->tagset[m->seltags] & 1 << i ? SchemeSel : SchemeNorm]);
		#endif // URGENTBORDER_PATCH
		#if BAR_ALTERNATIVE_TAGS_PATCH && BAR_PANGO_PATCH
		drw_text(drw, x, 0, w, bh, wdelta + lrpad / 2, (selmon->alttag ? tagsalt[i] : tags[i]), invert, False);
		#elif BAR_ALTERNATIVE_TAGS_PATCH
		drw_text(drw, x, 0, w, bh, wdelta + lrpad / 2, (selmon->alttag ? tagsalt[i] : tags[i]), invert);
		#elif BAR_PANGO_PATCH
		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], invert, False);
		#else
		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], invert);
		#endif // BAR_ALTERNATIVE_TAGS_PATCH
		#if !BAR_HIDEVACANTTAGS_PATCH
		if (occ & 1 << i)
			#if BAR_ACTIVETAGINDICATORBAR_PATCH
			drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw,
			#elif BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH
			drw_rect(drw, x + boxw, bh - boxw/2, w - ( 2 * boxw + 1), boxw/2,
			#else
			drw_rect(drw, x + boxs, boxs, boxw, boxw,
			#endif // BAR_ACTIVETAGINDICATORBAR_PATCH
				m == selmon && selmon->sel && selmon->sel->tags & 1 << i, invert);
		#endif // BAR_HIDEVACANTTAGS_PATCH
		x += w;
	}

	return x;
}

int
click_tags(Bar *bar, Arg *arg, BarClickArg *a)
{
	int i = 0, x = lrpad / 2;
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
		x += selmon->alttag ? TEXTW(tagsalt[i]) : TEXTW(tags[i]);
		#else
		x += TEXTW(tags[i]);
		#endif
	} while (a->rel_x >= x && ++i < LENGTH(tags));
	if (i < LENGTH(tags)) {
		arg->ui = 1 << i;
	}
	return ClkTagBar;
}
