void
removescratch(const Arg *arg)
{
	Client *c = selmon->sel;
	if (!c)
		return;
	c->scratchkey = 0;
}

void
setscratch(const Arg *arg)
{
	Client *c = selmon->sel;
	if (!c)
		return;

	c->scratchkey = ((char**)arg->v)[0][0];
}

void spawnscratch(const Arg *arg)
{
	if (fork() == 0) {
		if (dpy)
			close(ConnectionNumber(dpy));
		setsid();
		execvp(((char **)arg->v)[1], ((char **)arg->v)+1);
		fprintf(stderr, "dwm: execvp %s", ((char **)arg->v)[1]);
		perror(" failed");
		exit(EXIT_SUCCESS);
	}
}

void
togglescratch(const Arg *arg)
{
	Client *c, *next, *last = NULL, *found = NULL, *monclients = NULL;
	Monitor *mon;
	int scratchvisible = 0; // whether the scratchpads are currently visible or not
	int multimonscratch = 0; // whether we have scratchpads that are placed on multiple monitors
	int scratchmon = -1; // the monitor where the scratchpads exist
	int numscratchpads = 0; // count of scratchpads

	/* Looping through monitors and client's twice, the first time to work out whether we need
	   to move clients across from one monitor to another or not */
	for (mon = mons; mon; mon = mon->next)
		for (c = mon->clients; c; c = c->next) {
			if (c->scratchkey != ((char**)arg->v)[0][0])
				continue;
			if (scratchmon != -1 && scratchmon != mon->num)
				multimonscratch = 1;
			if (c->mon->tagset[c->mon->seltags] & c->tags) // && !HIDDEN(c)
				++scratchvisible;
			scratchmon = mon->num;
			++numscratchpads;
		}

	/* Now for the real deal. The logic should go like:
	    - hidden scratchpads will be shown
	    - shown scratchpads will be hidden, unless they are being moved to the current monitor
	    - the scratchpads will be moved to the current monitor if they all reside on the same monitor
	    - multiple scratchpads residing on separate monitors will be left in place
	 */
	for (mon = mons; mon; mon = mon->next) {
		for (c = mon->stack; c; c = next) {
			next = c->snext;
			if (c->scratchkey != ((char**)arg->v)[0][0])
				continue;

			#if BAR_WINTITLEACTIONS_PATCH
			/* unhide scratchpad if hidden */
			if (HIDDEN(c)) {
				XMapWindow(dpy, c->win);
				setclientstate(c, NormalState);
			}
			#endif // BAR_WINTITLEACTIONS_PATCH

			/* Record the first found scratchpad client for focus purposes, but prioritise the
			   scratchpad on the current monitor if one exists */
			if (!found || (mon == selmon && found->mon != selmon))
				found = c;

			/* If scratchpad clients reside on another monitor and we are moving them across then
			   as we are looping through monitors we could be moving a client to a monitor that has
			   not been processed yet, hence we could be processing a scratchpad twice. To avoid
			   this we detach them and add them to a temporary list (monclients) which is to be
			   processed later. */
			if (!multimonscratch && c->mon != selmon) {
				detach(c);
				detachstack(c);
				c->next = NULL;
				/* Note that we are adding clients at the end of the list, this is to preserve the
				   order of clients as they were on the adjacent monitor (relevant when tiled) */
				if (last)
					last = last->next = c;
				else
					last = monclients = c;
			} else if (scratchvisible == numscratchpads) {
				c->tags = 0;
			} else {
				XSetWindowBorder(dpy, c->win, scheme[SchemeScratchNorm][ColBorder].pixel);
				c->tags = c->mon->tagset[c->mon->seltags];
				if (c->isfloating)
					XRaiseWindow(dpy, c->win);
			}
		}
	}

	/* Attach moved scratchpad clients on the selected monitor */
	for (c = monclients; c; c = next) {
		next = c->next;
		mon = c->mon;
		c->mon = selmon;
		c->tags = selmon->tagset[selmon->seltags];
		/* Attach scratchpad clients from other monitors at the bottom of the stack */
		if (selmon->clients) {
			for (last = selmon->clients; last && last->next; last = last->next);
			last->next = c;
		} else
			selmon->clients = c;
		c->next = NULL;
		attachstack(c);

		/* Center floating scratchpad windows when moved from one monitor to another */
		if (c->isfloating) {
			if (c->w > selmon->ww)
				c->w = selmon->ww - c->bw * 2;
			if (c->h > selmon->wh)
				c->h = selmon->wh - c->bw * 2;

			if (numscratchpads > 1) {
				c->x = c->mon->wx + (c->x - mon->wx) * ((double)(abs(c->mon->ww - WIDTH(c))) / MAX(abs(mon->ww - WIDTH(c)), 1));
				c->y = c->mon->wy + (c->y - mon->wy) * ((double)(abs(c->mon->wh - HEIGHT(c))) / MAX(abs(mon->wh - HEIGHT(c)), 1));
			} else if (c->x < c->mon->mx || c->x > c->mon->mx + c->mon->mw ||
			           c->y < c->mon->my || c->y > c->mon->my + c->mon->mh)	{
				c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
				c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
			}
			resizeclient(c, c->x, c->y, c->w, c->h);
			XRaiseWindow(dpy, c->win);
		}
	}

	if (found) {
		focus(ISVISIBLE(found) ? found : NULL);
		arrange(NULL);
		if (found->isfloating)
			XRaiseWindow(dpy, found->win);
	} else {
		spawnscratch(arg);
	}
}

