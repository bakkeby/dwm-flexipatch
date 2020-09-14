void
togglescratch(const Arg *arg)
{
	Client *c = NULL;
	Monitor *mon;
	unsigned int found = 0;
	unsigned int scratchtag = SPTAG(arg->ui);
	unsigned int newtagset;
	int nh = 0, nw = 0;
	Arg sparg = {.v = scratchpads[arg->ui].cmd};

	for (mon = mons; mon && !found; mon = mon->next)
		for (c = mon->clients; c && !(found = c->tags & scratchtag); c = c->next);

	if (found) {

		if (HIDDEN(c)) {
			XMapWindow(dpy, c->win);
			setclientstate(c, NormalState);
			newtagset = 0;
		} else
			newtagset = selmon->tagset[selmon->seltags] ^ scratchtag;

		if (c->mon != selmon) {
			if (c->mon->tagset[c->mon->seltags] & SPTAGMASK)
				c->mon->tagset[c->mon->seltags] ^= scratchtag;
			if (c->w > selmon->ww)
				nw = selmon->ww - c->bw * 2;
			if (c->h > selmon->wh)
				nh = selmon->wh - c->bw * 2;
			if (nw > 0 || nh > 0)
				resizeclient(c, c->x, c->y, nw ? nw : c->w, nh ? nh : c->h);
			sendmon(c, selmon);
		}

		if (newtagset) {
			selmon->tagset[selmon->seltags] = newtagset;
			focus(NULL);
			arrange(selmon);
		}
		if (ISVISIBLE(c)) {
			focus(c);
			restack(selmon);
		}
	} else {
		selmon->tagset[selmon->seltags] |= scratchtag;
		spawn(&sparg);
	}
}