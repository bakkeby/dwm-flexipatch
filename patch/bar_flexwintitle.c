/* Flexwintitle properties, you can override these in your config.h if you want. */
#ifndef FLEXWINTITLE_BORDERS
#define FLEXWINTITLE_BORDERS 1       // 0 = off, 1 = on
#endif
#ifndef FLEXWINTITLE_SHOWFLOATING
#define FLEXWINTITLE_SHOWFLOATING 0  // whether to show titles for floating windows, hidden clients are always shown
#endif
#ifndef FLEXWINTITLE_MASTERWEIGHT
#define FLEXWINTITLE_MASTERWEIGHT 9  // master weight compared to stack, hidden and floating window titles
#endif
#ifndef FLEXWINTITLE_STACKWEIGHT
#define FLEXWINTITLE_STACKWEIGHT 3   // stack weight compared to master, hidden and floating window titles
#endif
#ifndef FLEXWINTITLE_HIDDENWEIGHT
#define FLEXWINTITLE_HIDDENWEIGHT 1  // hidden window title weight
#endif
#ifndef FLEXWINTITLE_FLOATWEIGHT
#define FLEXWINTITLE_FLOATWEIGHT 1   // floating window title weight, set to 0 to not show floating windows
#endif

#define SCHEMEFOR(c) getschemefor(m, c, groupactive == c)

enum { GRP_NOSELECTION, GRP_MASTER, GRP_STACK1, GRP_STACK2, GRP_FLOAT, GRP_HIDDEN };

int
width_flexwintitle(Bar *bar, BarArg *a)
{
	return a->w;
}

int
draw_flexwintitle(Bar *bar, BarArg *a)
{
	drw_rect(drw, a->x, a->y, a->w, a->h, 1, 1);
	return flextitlecalculate(bar->mon, a->x, a->w, -1, flextitledraw, NULL, a);
}

int
click_flexwintitle(Bar *bar, Arg *arg, BarArg *a)
{
	flextitlecalculate(bar->mon, 0, a->w, a->x, flextitleclick, arg, a);
	return ClkWinTitle;
}

Client *
flextitledrawarea(Monitor *m, Client *c, int x, int r, int w, int max_clients, int scheme, int draw_tiled, int draw_hidden, int draw_floating,
	int passx, void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg, BarArg *barg), Arg *arg, BarArg *barg)
{
	int i;
	for (i = 0; c && i < max_clients; c = c->next) {
		if (
			ISVISIBLE(c) &&
			(
				(draw_tiled && !c->isfloating && !HIDDEN(c)) ||
				(draw_floating && c->isfloating && !HIDDEN(c)) ||
				(draw_hidden && HIDDEN(c))
			)
		) {
			tabfn(m, c, passx, x, w + (i < r ? 1 : 0), scheme, arg, barg);
			x += w + (i < r ? 1 : 0);
			i++;
		}
	}
	return c;
}

int
getschemefor(Monitor *m, int group, int activegroup)
{
	switch (group) {
	case GRP_NOSELECTION:
	case GRP_MASTER:
	case GRP_STACK1:
	case GRP_STACK2:
		#if BSTACK_LAYOUT
		if (m->lt[m->sellt]->arrange == &bstack)
			return (activegroup ? SchemeFlexActLTR : SchemeFlexInaLTR);
		#endif // BSTACK_LAYOUT
		#if BSTACKHORIZ_LAYOUT
		if (m->lt[m->sellt]->arrange == &bstackhoriz) {
			if (group == GRP_MASTER)
				return (activegroup ? SchemeFlexActLTR : SchemeFlexInaLTR);
			else
				return (activegroup ? SchemeFlexActTTB : SchemeFlexInaTTB);
		}
		#endif // BSTACKHORIZ_LAYOUT
		#if CENTEREDMASTER_LAYOUT
		if (m->lt[m->sellt]->arrange == &centeredmaster)
			return (activegroup ? SchemeFlexActTTB : SchemeFlexInaTTB);
		#endif // CENTEREDMASTER_LAYOUT
		#if CENTEREDFLOATINGMASTER_LAYOUT
		if (m->lt[m->sellt]->arrange == &centeredfloatingmaster)
			return (activegroup ? SchemeFlexActLTR : SchemeFlexInaLTR);
		#endif // CENTEREDFLOATINGMASTER_LAYOUT
		#if COLUMNS_LAYOUT
		if (m->lt[m->sellt]->arrange == &col) {
			if (group == GRP_MASTER)
				return (activegroup ? SchemeFlexActLTR : SchemeFlexInaLTR);
			else
				return (activegroup ? SchemeFlexActTTB : SchemeFlexInaTTB);
		}
		#endif // COLUMNS_LAYOUT
		#if DECK_LAYOUT
		if (m->lt[m->sellt]->arrange == &deck) {
			if (group == GRP_MASTER)
				return (activegroup ? SchemeFlexActTTB : SchemeFlexInaTTB);
			else
				return (activegroup ? SchemeFlexActMONO : SchemeFlexInaMONO);
		}
		#endif // DECK_LAYOUT
		#if FIBONACCI_DWINDLE_LAYOUT
		if (m->lt[m->sellt]->arrange == &dwindle)
			return (activegroup ? SchemeFlexActDWDL : SchemeFlexInaDWDL);
		#endif // FIBONACCI_DWINDLE_LAYOUT
		#if FIBONACCI_SPIRAL_LAYOUT
		if (m->lt[m->sellt]->arrange == &spiral)
			return (activegroup ? SchemeFlexActSPRL : SchemeFlexInaSPRL);
		#endif // FIBONACCI_SPIRAL_LAYOUT
		#if FLEXTILE_DELUXE_LAYOUT
		if (m->lt[m->sellt]->arrange == &flextile)
			return (activegroup ? SchemeFlexActTTB + m->ltaxis[group] : SchemeFlexInaTTB + m->ltaxis[group]);
		#endif // FLEXTILE_DELUXE_LAYOUT
		#if GAPPLESSGRID_LAYOUT
		if (m->lt[m->sellt]->arrange == &gaplessgrid)
			return (activegroup ? SchemeFlexActGRID : SchemeFlexInaGRID);
		#endif // GAPPLESSGRID_LAYOUT
		#if GRIDMODE_LAYOUT
		if (m->lt[m->sellt]->arrange == &grid)
			return (activegroup ? SchemeFlexActGRDM : SchemeFlexInaGRDM);
		#endif // GRIDMODE_LAYOUT
		#if HORIZGRID_LAYOUT
		if (m->lt[m->sellt]->arrange == &horizgrid)
			return (activegroup ? SchemeFlexActHGRD : SchemeFlexInaHGRD);
		#endif // HORIZGRID_LAYOUT
		#if NROWGRID_LAYOUT
		if (m->lt[m->sellt]->arrange == &nrowgrid)
			return (activegroup ? SchemeFlexActGRD1 : SchemeFlexInaGRD1);
		#endif // NROWGRID_LAYOUT
		#if TILE_LAYOUT
		if (m->lt[m->sellt]->arrange == &tile)
			return (activegroup ? SchemeFlexActTTB : SchemeFlexInaTTB);
		#endif // TILE_LAYOUT
		#if MONOCLE_LAYOUT
		if (m->lt[m->sellt]->arrange == &monocle)
			return (activegroup ? SchemeFlexActMONO : SchemeFlexInaMONO);
		#endif // MONOCLE_LAYOUT
		return SchemeTitleNorm;
	case GRP_HIDDEN:
		return SchemeHidNorm;
	case GRP_FLOAT:
		return (activegroup ? SchemeFlexActFloat : SchemeFlexInaFloat);
	}
	return SchemeTitleNorm;
}

int
getselschemefor(int scheme)
{
	if (scheme == SchemeFlexActFloat || scheme == SchemeFlexInaFloat)
		return SchemeFlexSelFloat;
	if (scheme >= SchemeFlexInaTTB)
		return scheme + SchemeFlexInaTTB - SchemeFlexActTTB;
	if (scheme >= SchemeFlexActTTB)
		return scheme + SchemeFlexSelTTB - SchemeFlexActTTB;
	return SchemeTitleSel;
}

void
flextitledraw(Monitor *m, Client *c, int unused, int x, int w, int tabscheme, Arg *arg, BarArg *barg)
{
	if (!c)
		return;
	int i, nclienttags = 0, nviewtags = 0, pad = lrpad / 2;
	int clientscheme = (
		c == selmon->sel && HIDDEN(c)
		? SchemeHidSel
		: HIDDEN(c)
		? SchemeHidNorm
		: c == selmon->sel
		? getselschemefor(tabscheme)
		: c->isurgent
		? SchemeUrg
		: tabscheme
	);
	drw_setscheme(drw, scheme[clientscheme]);
	XSetWindowBorder(dpy, c->win, scheme[clientscheme][ColBorder].pixel);
	if (w <= TEXTW("A") - lrpad + pad) // reduce text padding if wintitle is too small
		pad = (w - TEXTW("A") + lrpad < 0 ? 0 : (w - TEXTW("A") + lrpad) / 2);
	#if BAR_CENTEREDWINDOWNAME_PATCH
	else if (TEXTW(c->name) < w)
		pad = (w - TEXTW(c->name) + lrpad) / 2;
	#endif // BAR_CENTEREDWINDOWNAME_PATCH

	#if BAR_WINICON_PATCH
	drw_text(drw, x, barg->y, w, barg->h, pad + (c->icon ? c->icon->width + ICONSPACING : 0), c->name, 0, False);
	if (c->icon)
		drw_img(drw, x + pad, barg->y + (barg->h - c->icon->height) / 2, c->icon, tmpicon);
	#else
	drw_text(drw, x, barg->y, w, barg->h, pad, c->name, 0, False);
	#endif // BAR_WINICON_PATCH

	drawstateindicator(m, c, 1, x + 2, barg->y, w, barg->h, 0, 0, 0);

	if (FLEXWINTITLE_BORDERS) {
		XSetForeground(drw->dpy, drw->gc, scheme[SchemeSel][ColBorder].pixel);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, barg->y, 1, barg->h);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x + w - (x + w >= barg->w ? 1 : 0), barg->y, 1, barg->h);
	}
	/* Optional tags icons */
	for (i = 0; i < NUMTAGS; i++) {
		if ((m->tagset[m->seltags] >> i) & 1)
			nviewtags++;
		if ((c->tags >> i) & 1)
			nclienttags++;
	}

	if (TAGSINDICATOR == 2 || nclienttags > 1 || nviewtags > 1)
		drawindicator(m, c, 1, x, barg->y, w, barg->h, 0, 0, 0, INDICATOR_RIGHT_TAGS);
}

#ifndef HIDDEN
#define HIDDEN(C) 0
#endif

void
flextitleclick(Monitor *m, Client *c, int passx, int x, int w, int unused, Arg *arg, BarArg *barg)
{
	if (passx >= x && passx <= x + w)
		arg->v = c;
}

int
flextitlecalculate(
	Monitor *m, int offx, int tabw, int passx,
	void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg, BarArg *barg),
	Arg *arg, BarArg *barg
) {
	Client *c;
	int n, center = 0, mirror = 0, fixed = 0; // layout configuration
	int clientsnmaster = 0, clientsnstack = 0, clientsnfloating = 0, clientsnhidden = 0;
	int i, w, r, num = 0, den, fulllayout = 0;
	int clientsnstack2 = 0;
	int groupactive = 0;
	int selidx = 0;
	int dualstack = 0;
	int rw, rr;

	int mas_x = offx, st1_x = offx, st2_x = offx, hid_x = offx, flt_x = offx;
	int mas_w, st1_w, st2_w, hid_w;

	for (i = 0, c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c))
			continue;
		if (HIDDEN(c)) {
			if (FLEXWINTITLE_HIDDENWEIGHT)
				clientsnhidden++;
			continue;
		}

		if (c->isfloating) {
			if (FLEXWINTITLE_FLOATWEIGHT)
				clientsnfloating++;
			continue;
		}

		if (m->sel == c)
			selidx = i;

		if (i < m->nmaster)
			clientsnmaster++;
		#if FLEXTILE_DELUXE_LAYOUT
		else if (m->nstack) {
			if (clientsnstack < m->nstack)
				clientsnstack++;
			else
				clientsnstack2++;
		}
		#endif // FLEXTILE_DELUXE_LAYOUT
		else if ((i - m->nmaster) % 2)
			clientsnstack2++;
		else
			clientsnstack++;
		i++;
	}

	if (!m->sel)
		groupactive = GRP_NOSELECTION;
	else if (HIDDEN(m->sel))
		groupactive = GRP_HIDDEN;
	else if (m->sel->isfloating)
		groupactive = GRP_FLOAT;
	else if (selidx < clientsnmaster)
		groupactive = GRP_MASTER;
	else if (selidx < clientsnmaster + clientsnstack)
		groupactive = GRP_STACK1;
	else if (selidx < clientsnmaster + clientsnstack + clientsnstack2)
		groupactive = GRP_STACK2;

	n = clientsnmaster + clientsnstack + clientsnstack2 + clientsnfloating + clientsnhidden;
	if (n == 0)
	 	return 0;
	#if FLEXTILE_DELUXE_LAYOUT
	else if (m->lt[m->sellt]->arrange == &flextile) {
		int layout = m->ltaxis[LAYOUT];
		if (layout < 0) {
			mirror = 1;
			layout *= -1;
		}
		if (layout > FLOATING_MASTER) {
			layout -= FLOATING_MASTER;
			fixed = 1;
		}

		if (layout == SPLIT_HORIZONTAL_DUAL_STACK || layout == SPLIT_HORIZONTAL_DUAL_STACK_FIXED)
			dualstack = 1;
		else if (layout == SPLIT_CENTERED_VERTICAL && (fixed || n - m->nmaster > 1))
			center = 1;
		else if (layout == FLOATING_MASTER)
			center = 1;
		else if (layout == SPLIT_CENTERED_HORIZONTAL) {
			if (fixed || n - m->nmaster > 1)
				center = 1;
		}
	}
	#endif // FLEXTILE_DELUXE_LAYOUT
	#if CENTEREDMASTER_LAYOUT
	else if (m->lt[m->sellt]->arrange == &centeredmaster && (fixed || n - m->nmaster > 1))
		center = 1;
	#endif // CENTEREDMASTER_LAYOUT
	#if CENTEREDFLOATINGMASTER_LAYOUT
	else if (m->lt[m->sellt]->arrange == &centeredfloatingmaster)
		center = 1;
	#endif // CENTEREDFLOATINGMASTER_LAYOUT

	/* Certain layouts have no master / stack areas */
	if (!m->lt[m->sellt]->arrange                            // floating layout
		|| (!n || (!fixed && m->nmaster && n <= m->nmaster)) // no master
		#if MONOCLE_LAYOUT
		|| m->lt[m->sellt]->arrange == &monocle
		#endif // MONOCLE_LAYOUT
		#if GRIDMODE_LAYOUT
		|| m->lt[m->sellt]->arrange == &grid
		#endif // GRIDMODE_LAYOUT
		#if HORIZGRID_LAYOUT
		|| m->lt[m->sellt]->arrange == &horizgrid
		#endif // HORIZGRID_LAYOUT
		#if GAPPLESSGRID_LAYOUT
		|| m->lt[m->sellt]->arrange == &gaplessgrid
		#endif // GAPPLESSGRID_LAYOUT
		#if NROWGRID_LAYOUT
		|| m->lt[m->sellt]->arrange == &nrowgrid
		#endif // NROWGRID_LAYOUT
		#if FLEXTILE_DELUXE_LAYOUT
		|| (m->lt[m->sellt]->arrange == &flextile && m->ltaxis[LAYOUT] == NO_SPLIT)
		#endif // FLEXTILE_DELUXE_LAYOUT
	)
		fulllayout = 1;

	num = tabw;
	c = m->clients;

	/* floating mode */
	if ((fulllayout && FLEXWINTITLE_FLOATWEIGHT > 0) || clientsnmaster + clientsnstack == 0 || !m->lt[m->sellt]->arrange) {
		den = clientsnmaster + clientsnstack + clientsnstack2 + clientsnfloating + clientsnhidden;
		w = num / den;
		r = num % den; // rest
		c = flextitledrawarea(m, c, mas_x, r, w, den, !m->lt[m->sellt]->arrange ? SchemeFlexActFloat : SCHEMEFOR(GRP_MASTER), 1, FLEXWINTITLE_HIDDENWEIGHT, FLEXWINTITLE_FLOATWEIGHT, passx, tabfn, arg, barg); // floating
	/* no master and stack mode, e.g. monocole, grid layouts, fibonacci */
	} else if (fulllayout) {
		den = clientsnmaster + clientsnstack + clientsnstack2 + clientsnhidden;
		w = num / den;
		r = num % den; // rest
		c = flextitledrawarea(m, c, mas_x, r, w, den, SCHEMEFOR(GRP_MASTER), 1, FLEXWINTITLE_HIDDENWEIGHT, 0, passx, tabfn, arg, barg); // full
	/* tiled mode */
	} else {
		den = clientsnmaster * FLEXWINTITLE_MASTERWEIGHT + (clientsnstack + clientsnstack2) * FLEXWINTITLE_STACKWEIGHT + clientsnfloating * FLEXWINTITLE_FLOATWEIGHT + clientsnhidden * FLEXWINTITLE_HIDDENWEIGHT;
		w = num / den; // weight width per client
		r = num % den; // weight rest width
		rw = r / n; // rest incr per client
		rr = r % n; // rest rest
		#if FLEXTILE_DELUXE_LAYOUT
		if ((!center && !dualstack) || (center && n <= m->nmaster + (m->nstack ? m->nstack : 1)))
		#else
		if ((!center && !dualstack) || (center && n <= m->nmaster + 1))
		#endif // FLEXTILE_DELUXE_LAYOUT
		{
			clientsnstack += clientsnstack2;
			clientsnstack2 = 0;
			if (groupactive == GRP_STACK2)
				groupactive = GRP_STACK1;
		}

		mas_w = clientsnmaster * rw + w * clientsnmaster * FLEXWINTITLE_MASTERWEIGHT + (rr > 0 ? MIN(rr, clientsnmaster) : 0);
		rr -= clientsnmaster;
		st1_w = clientsnstack * (rw + w * FLEXWINTITLE_STACKWEIGHT) + (rr > 0 ? MIN(rr, clientsnstack) : 0);
		rr -= clientsnstack;
		st2_w = clientsnstack2 * (rw + w * FLEXWINTITLE_STACKWEIGHT) + (rr > 0 ? MIN(rr, clientsnstack2) : 0);
		rr -= clientsnstack2;
		hid_w = clientsnhidden * (rw + w * FLEXWINTITLE_HIDDENWEIGHT) + (rr > 0 ? MIN(rr, clientsnhidden) : 0);
		rr -= clientsnhidden;
		rr = r % n;

		if (mirror) {
			if (center && clientsnstack2) {
				mas_x = st1_x + st1_w;
				st2_x = mas_x + mas_w;
				hid_x = st2_x + st2_w;
			} else {
				if (clientsnstack2) {
					st2_x = st1_x + st1_w;
					mas_x = st2_x + st2_w;
				} else
					mas_x = st1_x + st1_w;
				hid_x = mas_x + mas_w;
			}
		} else {
			if (center && clientsnstack2) {
				mas_x = st2_x + st2_w;
				st1_x = mas_x + mas_w;
				hid_x = st1_x + st1_w;
			} else {
				st1_x = mas_x + mas_w;
				if (clientsnstack2) {
					st2_x = st1_x + st1_w;
					hid_x = st2_x + st2_w;
				} else
					hid_x = st1_x + st1_w;
			}
		}

		flt_x = hid_x + hid_w;
		c = flextitledrawarea(m, c, mas_x, rr, w * FLEXWINTITLE_MASTERWEIGHT + rw, clientsnmaster, SCHEMEFOR(GRP_MASTER), 1, 0, 0, passx, tabfn, arg, barg); // master
		rr -= clientsnmaster;
		c = flextitledrawarea(m, c, st1_x, rr, w * FLEXWINTITLE_STACKWEIGHT + rw, clientsnstack, SCHEMEFOR(GRP_STACK1), 1, 0, 0, passx, tabfn, arg, barg); // stack1
		rr -= clientsnstack;
		if (clientsnstack2) {
			c = flextitledrawarea(m, c, st2_x, rr, w * FLEXWINTITLE_STACKWEIGHT + rw, clientsnstack2, SCHEMEFOR(GRP_STACK2), 1, 0, 0, passx, tabfn, arg, barg); // stack2
			rr -= clientsnstack2;
		}
		c = flextitledrawarea(m, m->clients, hid_x, rr, w * FLEXWINTITLE_HIDDENWEIGHT + rw, clientsnhidden, SCHEMEFOR(GRP_HIDDEN), 0, 1, 0, passx, tabfn, arg, barg); // hidden
		rr -= clientsnhidden;
		c = flextitledrawarea(m, m->clients, flt_x, rr, w * FLEXWINTITLE_FLOATWEIGHT + rw, clientsnfloating, SCHEMEFOR(GRP_FLOAT), 0, 0, 1, passx, tabfn, arg, barg); // floating
	}
	return 1;
}

