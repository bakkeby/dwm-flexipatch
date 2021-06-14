void
removescratch(const Arg *arg)
{
	Client *c = selmon->sel;
	if (!c)
		return;
	unsigned int scratchtag = SPTAG(arg->ui);
	c->tags = c->mon->tagset[c->mon->seltags] ^ scratchtag;
	arrange(c->mon);
}

void
setscratch(const Arg *arg)
{
	Client *c = selmon->sel;
	if (!c)
		return;
	unsigned int scratchtag = SPTAG(arg->ui);
	c->tags = scratchtag;
	arrange(c->mon);
}

void
togglescratch(const Arg *arg)
{
	Client *c = NULL, *next = NULL, *found = NULL;
	Monitor *mon;
	unsigned int scratchtag = SPTAG(arg->ui);
	unsigned int newtagset = 0;
	int nh = 0, nw = 0;
	Arg sparg = {.v = scratchpads[arg->ui].cmd};

	for (mon = mons; mon; mon = mon->next) {
		for (c = mon->clients; c; c = next) {
			next = c->next;
			if (!(c->tags & scratchtag))
				continue;

			found = c;

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
		}
	}

	if (found) {
		if (newtagset) {
			selmon->tagset[selmon->seltags] = newtagset;
			focus(NULL);
			arrange(selmon);
		}
		if (ISVISIBLE(found)) {
			focus(found);
			restack(selmon);
		}
	} else {
		selmon->tagset[selmon->seltags] |= scratchtag;
		spawn(&sparg);
	}
}

