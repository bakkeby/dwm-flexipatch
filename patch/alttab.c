int alttabn;          /* move that many clients forward */
int ntabs;            /* number of active clients in tag */
int isalt;
Client **altsnext;    /* array of all clients in the tag */
Window alttabwin;

void
alttab()
{
	Monitor *m = selmon;

	/* move to next window */
	if (m->sel && m->sel->snext) {
		alttabn++;
		if (alttabn >= ntabs)
			alttabn = 0; /* reset alttabn */

		focus(altsnext[alttabn]);
		restack(m);
	}

	/* redraw tab */
	XRaiseWindow(dpy, alttabwin);
	drawtab(ntabs, 0, m);
}

void
alttabend()
{
	Monitor *m = selmon;
	Client *buff;
	int i;

	if (!isalt)
		return;

	/* Move all clients between first and choosen position,
	 * one down in stack and put choosen client to the first position
	 * so they remain in right order for the next time that alt-tab is used
	 */
	if (ntabs > 1) {
		if (alttabn != 0) { /* if user picked original client do nothing */
			buff = altsnext[alttabn];
			if (alttabn > 1)
				for (i = alttabn; i > 0; i--)
					altsnext[i] = altsnext[i - 1];
			else /* swap them if there are just 2 clients */
				altsnext[alttabn] = altsnext[0];
			altsnext[0] = buff;
		}

		/* restack clients */
		for (i = ntabs - 1; i >= 0; i--) {
		    focus(altsnext[i]);
		    restack(m);
		}

		free(altsnext); /* free list of clients */
	}

	/* destroy the window */
	isalt = 0;
	ntabs = 0;
	XUnmapWindow(dpy, alttabwin);
	XDestroyWindow(dpy, alttabwin);
}

void
drawtab(int nwins, int first, Monitor *m)
{
	Client *c;
	int i, h;
	int y = 0;
	int px = m->mx;
	int py = m->my;

	if (first) {
		XSetWindowAttributes wa = {
			.override_redirect = True,
			#if BAR_ALPHA_PATCH
			.background_pixel = 0,
			.border_pixel = 0,
			.colormap = cmap,
			#else
			.background_pixmap = ParentRelative,
			#endif // BAR_ALPHA_PATCH
			.event_mask = ButtonPressMask|ExposureMask
		};

		/* decide position of tabwin */
		if (tabposx == 1)
			px = m->mx + (m->mw / 2) - (maxwtab / 2);
		else if (tabposx == 2)
			px = m->mx + m->mw - maxwtab;

		if (tabposy == 1)
			py = m->my + (m->mh / 2) - (maxhtab / 2);
		else if (tabposy == 2)
			py = m->my + m->mh - maxhtab;

		h = maxhtab;

		#if BAR_ALPHA_PATCH
		alttabwin = XCreateWindow(dpy, root, px, py, maxwtab, maxhtab, 2, depth,
		                             InputOutput, visual,
		                             CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
		#else
		alttabwin = XCreateWindow(dpy, root, px, py, maxwtab, maxhtab, 2, DefaultDepth(dpy, screen),
		                             CopyFromParent, DefaultVisual(dpy, screen),
		                             CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
		#endif // BAR_ALPHA_PATCH

		XDefineCursor(dpy, alttabwin, cursor[CurNormal]->cursor);
		XMapRaised(dpy, alttabwin);
	}

	h = maxhtab / ntabs;
	for (i = 0; i < ntabs; i++) { /* draw all clients into tabwin */
		c = altsnext[i];
		if (!ISVISIBLE(c))
			continue;
		if (HIDDEN(c))
			continue;

		drw_setscheme(drw, scheme[c == m->sel ? SchemeSel : SchemeNorm]);
		drw_text(drw, 0, y, maxwtab, h, 0, c->name, 0, 0);
		y += h;
	}

	drw_setscheme(drw, scheme[SchemeNorm]);
	drw_map(drw, alttabwin, 0, 0, maxwtab, maxhtab);
}

void
alttabstart(const Arg *arg)
{
	Client *c;
	Monitor *m = selmon;
	int grabbed;
	int i;

	altsnext = NULL;
	if (alttabwin)
		alttabend();

	if (isalt == 1) {
		alttabend();
		return;
	}

	isalt = 1;
	alttabn = 0;
	ntabs = 0;

	for (c = m->clients; c; c = c->next) {
		if (!ISVISIBLE(c))
			continue;
		if (HIDDEN(c))
			continue;

		++ntabs;
	}

	if (!ntabs) {
		alttabend();
		return;
	}

	altsnext = (Client **) malloc(ntabs * sizeof(Client *));

	for (i = 0, c = m->stack; c; c = c->snext) {
		if (!ISVISIBLE(c))
			continue;
		if (HIDDEN(c))
			continue;

		altsnext[i] = c;
		i++;
	}

	drawtab(ntabs, 1, m);

	struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000000 };

	/* grab keyboard (take all input from keyboard) */
	grabbed = 1;
	for (i = 0; i < 1000; i++) {
		if (XGrabKeyboard(dpy, root, True, GrabModeAsync, GrabModeAsync, CurrentTime) == GrabSuccess)
			break;
		nanosleep(&ts, NULL);
		if (i == 1000 - 1)
			grabbed = 0;
	}

	XEvent event;
	alttab();

	if (grabbed == 0) {
		alttabend();
		return;
	}

	while (grabbed) {
		XNextEvent(dpy, &event);
		if (event.type == KeyPress || event.type == KeyRelease) {
			if (event.type == KeyRelease && event.xkey.keycode == tabmodkey) /* if mod key is released break cycle */
				break;

			if (event.type == KeyPress) {
				if (event.xkey.keycode == tabcyclekey) { /* if tab is pressed move to the next window */
					alttab();
				}
			}
		}
	}

	c = m->sel;
	alttabend();

	XUngrabKeyboard(dpy, CurrentTime);
	focus(c);
	restack(m);
}
