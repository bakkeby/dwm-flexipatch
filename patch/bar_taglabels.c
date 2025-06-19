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
			drw_rect(drw, x + ulinepad, a->y + bh - ulinestroke - ulinevoffset, w - (ulinepad * 2), ulinestroke, 1, 0);
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
	#if BAR_TAGPREVIEW_PATCH
	if (selmon->previewshow != 0) {
		hidetagpreview(selmon);
	}
	#endif // BAR_TAGPREVIEW_PATCH
	return ClkTagBar;
}

int
hover_taglabels(Bar *bar, BarArg *a, XMotionEvent *ev)
{
	#if BAR_TAGPREVIEW_PATCH
	int i = 0, x = lrpad / 2;
	int px, py;
	Monitor *m = bar->mon;
	#if VANITYGAPS_PATCH
	int ov = gappov;
	int oh = gappoh;
	#else
	int ov = 0;
	int oh = 0;
	#endif // VANITYGAPS_PATCH

	do {
		if (!m->taglabel[i][0])
			continue;
		x += TEXTW(m->taglabel[i]);
	} while (a->x >= x && ++i < NUMTAGS);

	if (i < NUMTAGS) {
		if ((i + 1) != selmon->previewshow && !(selmon->tagset[selmon->seltags] & 1 << i)) {
			if (bar->by > m->my + m->mh / 2) // bottom bar
				py = bar->by - m->mh / scalepreview - oh;
			else // top bar
				py = bar->by + bar->bh + oh;
			px = bar->bx + ev->x - m->mw / scalepreview / 2;
			if (px + m->mw / scalepreview > m->mx + m->mw)
				px = m->wx + m->ww - m->mw / scalepreview - ov;
			else if (px < bar->bx)
				px = m->wx + ov;
			selmon->previewshow = i + 1;
			showtagpreview(i, px, py);
		} else if (selmon->tagset[selmon->seltags] & 1 << i) {
			hidetagpreview(selmon);
		}
	} else if (selmon->previewshow != 0) {
		hidetagpreview(selmon);
	}
	#endif // BAR_TAGPREVIEW_PATCH

	return 1;
}
