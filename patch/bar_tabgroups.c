int
width_bartabgroups(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_bartabgroups(Bar *bar, BarDrawArg *a)
{
	drw_rect(drw, a->x, 0, a->w, bh, 1, 1);
	bartabcalculate(bar->mon, a->x, a->w, -1, bartabdraw, NULL);
	return a->x + a->w;
}

int
click_bartabgroups(Bar *bar, Arg *arg, BarClickArg *a)
{
	bartabcalculate(bar->mon, 0, a->rel_w, a->rel_x, bartabclick, arg);
	return ClkWinTitle;
}

void
bartabdraw(Monitor *m, Client *c, int unused, int x, int w, int groupactive, Arg *arg)
{
	if (!c)
		return;
	int i, nclienttags = 0, nviewtags = 0;
	drw_setscheme(drw, scheme[
		m->sel == c
		? SchemeSel
		: HIDDEN(c)
		? SchemeHid
		: groupactive
		? SchemeTabActive
		: SchemeTabInactive
	]);
	drw_text(drw, x, 0, w, bh, lrpad / 2, c->name, 0);
	if (c->isfloating)
		drw_rect(drw, x + 2, 2, 5, 5, 0, 0);

	if (BARTAB_BORDERS) {
		XSetForeground(drw->dpy, drw->gc, scheme[SchemeSel][ColBorder].pixel);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, 0, 1, bh);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x + w, 0, 1, bh);
	}
	/* Optional tags icons */
	for (i = 0; i < LENGTH(tags); i++) {
		if ((m->tagset[m->seltags] >> i) & 1)
			nviewtags++;
		if ((c->tags >> i) & 1)
			nclienttags++;
	}

	if (BARTAB_TAGSINDICATOR == 2 || nclienttags > 1 || nviewtags > 1) {
		for (i = 0; i < LENGTH(tags); i++) {
			drw_rect(drw,
				( x + w - 2 - ((LENGTH(tags) / BARTAB_TAGSROWS) * BARTAB_TAGSPX)
					- (i % (LENGTH(tags)/BARTAB_TAGSROWS)) + ((i % (LENGTH(tags) / BARTAB_TAGSROWS)) * BARTAB_TAGSPX)
				),
				( 2 + ((i / (LENGTH(tags)/BARTAB_TAGSROWS)) * BARTAB_TAGSPX)
					- ((i / (LENGTH(tags)/BARTAB_TAGSROWS)))
				),
				BARTAB_TAGSPX, BARTAB_TAGSPX, (c->tags >> i) & 1, 0
			);
		}
	}
}

void
bartabclick(Monitor *m, Client *c, int passx, int x, int w, int unused, Arg *arg)
{
	if (passx >= x && passx <= x + w)
		arg->v = c;
}

void
bartabcalculate(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg), Arg *arg
) {
	Client *c;
	int
		i, clientsnmaster = 0, clientsnstack = 0, clientsnfloating = 0,
		masteractive = 0, fulllayout = 0,
		x = offx, w, r, num = 0, den, tgactive;

	for (i = 0; i < LENGTH(bartabmonfns); i++)
		if (m ->lt[m->sellt]->arrange == bartabmonfns[i]) {
			fulllayout = 1;
			break;
		}

	for (i = 0, c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c))
			continue;
		if (!HIDDEN(c) && c->isfloating && !BARTAB_SHOWFLOATING) {
			clientsnfloating++;
			continue;
		}
		if (m->sel == c)
			masteractive = i < m->nmaster;
		if (i < m->nmaster)
			clientsnmaster++;
		else
			clientsnstack++;
		i++;
	}

	if (!i)
		return;

	/* floating mode */
	if (clientsnmaster + clientsnstack == 0 || !m->lt[m->sellt]->arrange) {
		tgactive = 1;
		num = tabw;
		den = clientsnmaster + clientsnstack + clientsnfloating;
		r = num % den;
		w = num / den;
		for (c = m->clients, i = 0; c; c = c->next) {
			if (!ISVISIBLE(c))
				continue;
			tabfn(m, c, passx, x, w + (i < r ? 1 : 0), tgactive, arg);
			x += w + (i < r ? 1 : 0);
			i++;
		}
	/* monocle mode */
	} else if (fulllayout || ((clientsnmaster == 0) ^ (clientsnstack == 0))) {
		tgactive = 1;
		num = tabw;
		den = clientsnmaster + clientsnstack;
		r = num % den;
		w = num / den;
		for (c = m->clients, i = 0; c; c = c->next) {
			if (!ISVISIBLE(c) || (!HIDDEN(c) && c->isfloating && !BARTAB_SHOWFLOATING))
				continue;
			tabfn(m, c, passx, x, w + (i < r ? 1 : 0), tgactive, arg);
			x += w + (i < r ? 1 : 0);
			i++;
		}
	/* tiled mode */
	} else {
		tgactive = masteractive;
		num = clientsnstack ? tabw * m->mfact : tabw;
		den = clientsnmaster;
		r = num % den;
		w = num / den;
		for (c = m->clients, i = 0; c && i < m->nmaster; c = c->next) {
			if (!ISVISIBLE(c) || (!HIDDEN(c) && c->isfloating && !BARTAB_SHOWFLOATING))
				continue;
			tabfn(m, c, passx, x, w + (i < r ? 1 : 0), tgactive, arg);
			x += w + (i < r ? 1 : 0);
			i++;
		}

		tgactive = !tgactive;
		num = tabw - num;
		den = clientsnstack;
		r = num % den;
		w = num / den;
		for (; c; c = c->next) {
			if (!ISVISIBLE(c) || (!HIDDEN(c) && c->isfloating && !BARTAB_SHOWFLOATING))
				continue;
			tabfn(m, c, passx, x, w + (i - m->nmaster < r ? 1 : 0), tgactive, arg);
			x += w + (i - m->nmaster < r ? 1 : 0);
			i++;
		}
	}
}