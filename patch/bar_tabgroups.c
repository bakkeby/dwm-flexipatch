/* Bartabgroups properties, you can override these in your config.h if you want. */
#ifndef BARTAB_BORDERS
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#endif
#ifndef BARTAB_SHOWFLOATING
#define BARTAB_SHOWFLOATING 0  // whether to show titles for floating windows, hidden clients are always shown
#endif
#ifndef BARTAB_STACKWEIGHT
#define BARTAB_STACKWEIGHT 1   // stack weight compared to hidden and floating window titles
#endif
#ifndef BARTAB_HIDDENWEIGHT
#define BARTAB_HIDDENWEIGHT 1  // hidden window title weight
#endif
#ifndef BARTAB_FLOATWEIGHT
#define BARTAB_FLOATWEIGHT 1   // floating window title weight, set to 0 to not show floating windows
#endif

int
width_bartabgroups(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_bartabgroups(Bar *bar, BarArg *a)
{
	drw_rect(drw, a->x, a->y, a->w, a->h, 1, 1);
	return bartabcalculate(bar->mon, a->x, a->w, -1, bartabdraw, NULL, a);
}

int
click_bartabgroups(Bar *bar, Arg *arg, BarArg *a)
{
	bartabcalculate(bar->mon, 0, a->w, a->x, bartabclick, arg, a);
	return ClkWinTitle;
}

void
bartabdraw(Monitor *m, Client *c, int unused, int x, int w, int groupactive, Arg *arg, BarArg *a)
{
	if (!c)
		return;
	int i, nclienttags = 0, nviewtags = 0;
	int tpad = lrpad / 2;
	#if BAR_WINICON_PATCH
	int ipad = c->icon ? c->icw + ICONSPACING : 0;
	#endif // BAR_WINICON_PATCH
	#if BAR_CENTEREDWINDOWNAME_PATCH
	int cpad = 0;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH
	int tx = x;
	int tw = w;

	drw_setscheme(drw, scheme[
		m->sel == c
		#ifdef HIDDEN
		    && HIDDEN(c)
		? SchemeHidSel
		: HIDDEN(c)
		? SchemeHidNorm
		: m->sel == c
		#endif
		? SchemeSel
		: groupactive
		? SchemeTitleSel
		: SchemeTitleNorm
	]);
	if (w <= TEXTW("A") - lrpad + tpad) // reduce text padding if wintitle is too small
		tpad = (w - TEXTW("A") + lrpad < 0 ? 0 : (w - TEXTW("A") + lrpad) / 2);
	#if BAR_WINICON_PATCH && BAR_CENTEREDWINDOWNAME_PATCH
	else if (TEXTW(c->name) + ipad < w)
		cpad = (w - TEXTW(c->name) - ipad) / 2;
	#elif BAR_CENTEREDWINDOWNAME_PATCH
	else if (TEXTW(c->name) < w)
		cpad = (w - TEXTW(c->name)) / 2;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH

	XSetForeground(drw->dpy, drw->gc, drw->scheme[ColBg].pixel);
	XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, a->y, w, a->h);

	#if BAR_CENTEREDWINDOWNAME_PATCH
	/* Apply center padding, if any */
	tx += cpad;
	tw -= cpad;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH

	tx += tpad;
	tw -= lrpad;

	#if BAR_WINICON_PATCH
	if (ipad) {
		drw_pic(drw, tx, a->y + (a->h - c->ich) / 2, c->icw, c->ich, c->icon);
		tx += ipad;
		tw -= ipad;
	}
	#endif // BAR_WINICON_PATCH

	drw_text(drw, tx, a->y, tw, a->h, 0, c->name, 0, False);

	drawstateindicator(m, c, 1, x, a->y, w, a->h, 0, 0, c->isfixed);

	if (BARTAB_BORDERS) {
		XSetForeground(drw->dpy, drw->gc, scheme[SchemeSel][ColBorder].pixel);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, a->y, 1, a->h);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x + w - (x + w >= a->w ? 1 : 0), a->y, 1, a->h);
	}
	/* Optional tags icons */
	for (i = 0; i < NUMTAGS; i++) {
		if ((m->tagset[m->seltags] >> i) & 1)
			nviewtags++;
		if ((c->tags >> i) & 1)
			nclienttags++;
	}

	if (TAGSINDICATOR == 2 || nclienttags > 1 || nviewtags > 1)
		drawindicator(m, c, 1, x, a->y, w, a->h, 0, 0, 0, INDICATOR_RIGHT_TAGS);
}

#ifndef HIDDEN
#define HIDDEN(C) 0
#endif

void
bartabclick(Monitor *m, Client *c, int passx, int x, int w, int unused, Arg *arg, BarArg *barg)
{
	if (passx >= x && passx <= x + w)
		arg->v = c;
}

int
bartabcalculate(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg, BarArg *barg),
	Arg *arg, BarArg *barg
) {
	Client *c;
	int
		i, clientsnmaster = 0, clientsnstack = 0, clientsnfloating = 0, clientsnhidden = 0,
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
		if (HIDDEN(c)) {
			clientsnhidden++;
			continue;
		}
		if (c->isfloating) {
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

	if (clientsnmaster + clientsnstack + clientsnfloating + clientsnhidden == 0)
	 	return 0;

	tgactive = 1;
	num = tabw;
	/* floating mode */
	if ((fulllayout && BARTAB_FLOATWEIGHT) || clientsnmaster + clientsnstack == 0 || !m->lt[m->sellt]->arrange) {
		den = clientsnmaster + clientsnstack + clientsnfloating + clientsnhidden;
		r = num % den;
		w = num / den;
		for (c = m->clients, i = 0; c; c = c->next) {
			if (!ISVISIBLE(c))
				continue;
			tabfn(m, c, passx, x, w + (i < r ? 1 : 0), tgactive, arg, barg);
			x += w + (i < r ? 1 : 0);
			i++;
		}
	/* no master and stack mode, e.g. monocole, grid layouts, fibonacci */
	} else if (fulllayout) {
		den = clientsnmaster + clientsnstack + clientsnhidden;
		r = num % den;
		w = num / den;
		for (c = m->clients, i = 0; c; c = c->next) {
			if (!ISVISIBLE(c) || (c->isfloating && !HIDDEN(c)))
				continue;
			tabfn(m, c, passx, x, w + (i < r ? 1 : 0), tgactive, arg, barg);
			x += w + (i < r ? 1 : 0);
			i++;
		}
	/* tiled mode */
	} else {
		den = clientsnmaster;
		c = m->clients;
		i = 0;
		if (den) {
			if (clientsnstack + clientsnfloating * BARTAB_FLOATWEIGHT + clientsnhidden) {
				tgactive = masteractive;
				num = tabw * m->mfact;
			}
			r = num % den;
			w = num / den;
			for (; c && i < m->nmaster; c = c->next) { // tiled master
				if (!ISVISIBLE(c) || c->isfloating || HIDDEN(c))
					continue;
				tabfn(m, c, passx, x, w + (i < r ? 1 : 0), tgactive, arg, barg);
				x += w + (i < r ? 1 : 0);
				i++;
			}
			tgactive = !tgactive;
			num = tabw - num;
		}

		den = clientsnstack * BARTAB_STACKWEIGHT + clientsnfloating * BARTAB_FLOATWEIGHT + clientsnhidden * BARTAB_HIDDENWEIGHT;
		if (!den)
			return 1;

		r = num % den;
		w = num / den;
		#if BARTAB_STACKWEIGHT
		for (; c; c = c->next) { // tiled stack
			if (!ISVISIBLE(c) || HIDDEN(c) || c->isfloating)
				continue;
			tabfn(m, c, passx, x, w * BARTAB_STACKWEIGHT + (i - m->nmaster < r ? 1 : 0), tgactive, arg, barg);
			x += w * BARTAB_STACKWEIGHT + (i - m->nmaster < r ? 1 : 0);
			i++;
		}
		#endif // BARTAB_STACKWEIGHT

		#if BARTAB_HIDDENWEIGHT
		for (c = m->clients; c; c = c->next) { // hidden windows
			if (!ISVISIBLE(c) || !HIDDEN(c))
				continue;
			tabfn(m, c, passx, x, w * BARTAB_HIDDENWEIGHT + (i - m->nmaster < r ? 1 : 0), tgactive, arg, barg);
			x += w * BARTAB_HIDDENWEIGHT + (i - m->nmaster < r ? 1 : 0);
			i++;
		}
		#endif // BARTAB_HIDDENWEIGHT

		#if BARTAB_FLOATWEIGHT
		for (c = m->clients; c; c = c->next) { // floating windows
			if (!ISVISIBLE(c) || HIDDEN(c) || !c->isfloating)
				continue;
			tabfn(m, c, passx, x, w * BARTAB_FLOATWEIGHT + (i - m->nmaster < r ? 1 : 0), tgactive, arg, barg);
			x += w * BARTAB_FLOATWEIGHT + (i - m->nmaster < r ? 1 : 0);
			i++;
		}
		#endif // BARTAB_FLOATWEIGHT
	}
	return 1;
}

