int
width_taglabels(Bar *bar, BarArg *a)
{
	int w, i;
	Client *c;
	Monitor *m = bar->mon;
	char *icon;
	unsigned int occ = 0;

	for (c = m->clients; c; c = c->next)
		occ |= c->tags == 255 ? 0 : c->tags;

	for (w = 0, i = 0; i < NUMTAGS; i++) {
		m->taglabel[i][0] = '\0';
		#if BAR_HIDEVACANTTAGS_PATCH
		if (!(occ & 1 << i || m->tagset[m->seltags] & 1 << i))
			continue;
		#endif // BAR_HIDEVACANTTAGS_PATCH
		icon = tagicon(m, i);
		XClassHint ch = { NULL, NULL };
		for (c = m->clients; c; c = c->next) {
			if (c->tags & (1 << i)) {
				XGetClassHint(dpy, c->win, &ch);
				break;
			}
		}
		if (ch.res_class) {
			if (lcaselbl)
				ch.res_class[0] = tolower(ch.res_class[0]);
			snprintf(m->taglabel[i], 64, ptagf, icon, ch.res_class);
		} else
			snprintf(m->taglabel[i], 64, etagf, icon);

		w += TEXTW(m->taglabel[i]);
	}
	return w;
}

int
draw_taglabels(Bar *bar, BarArg *a)
{
	int invert = 0;
	int w, x = a->x;
	unsigned int i, occ = 0, urg = 0;
	Client *c;
	Monitor *m = bar->mon;

	for (c = m->clients; c; c = c->next)
		if (c->isurgent)
			urg |= c->tags;

	for (i = 0; i < NUMTAGS; i++) {
		/* do not draw vacant tags */
		if (!m->taglabel[i][0])
			continue;
		drw_setscheme(drw, scheme[
			m->tagset[m->seltags] & 1 << i
			? SchemeTagsSel
			: urg & 1 << i
			? SchemeUrg
			: SchemeTagsNorm
		]);
		w = TEXTW(m->taglabel[i]);
		drw_text(drw, x, a->y, w, a->h, lrpad / 2, m->taglabel[i], invert, False);
		drawindicator(m, NULL, occ, x, a->y, w, a->h, i, -1, invert, tagindicatortype);
		#if BAR_UNDERLINETAGS_PATCH
		if (ulineall || m->tagset[m->seltags] & 1 << i)
			drw_rect(drw, x + ulinepad, bh - ulinestroke - ulinevoffset, w - (ulinepad * 2), ulinestroke, 1, 0);
		#endif // BAR_UNDERLINETAGS_PATCH
		x += w;
	}

	return 1;
}

int
click_taglabels(Bar *bar, Arg *arg, BarArg *a)
{
	int i = 0, x = lrpad / 2;
	Monitor *m = bar->mon;

	do {
		if (!m->taglabel[i][0])
			continue;
		x += TEXTW(m->taglabel[i]);
	} while (a->x >= x && ++i < NUMTAGS);
	if (i < NUMTAGS) {
		arg->ui = 1 << i;
	}
	return ClkTagBar;
}
