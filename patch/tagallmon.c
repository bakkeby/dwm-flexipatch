void
tagallmon(const Arg *arg)
{
	Monitor *m;
	Client *c, *last, *slast, *next;

	if (!mons->next)
		return;

	m = dirtomon(arg->i);
	for (last = m->clients; last && last->next; last = last->next);
	for (slast = m->stack; slast && slast->snext; slast = slast->snext);

	for (c = selmon->clients; c; c = next) {
		next = c->next;
		if (!ISVISIBLE(c))
			continue;
		unfocus(c, 1, NULL);
		detach(c);
		detachstack(c);
		c->mon = m;
		c->tags = m->tagset[m->seltags]; /* assign tags of target monitor */
		c->next = NULL;
		c->snext = NULL;
		if (last)
			last = last->next = c;
		else
			m->clients = last = c;
		if (slast)
			slast = slast->snext = c;
		else
			m->stack = slast = c;
		if (c->isfullscreen) {
			#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
			if (c->fakefullscreen != 1) {
				resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
				XRaiseWindow(dpy, c->win);
			}
			#elif !FAKEFULLSCREEN_PATCH
			resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
			XRaiseWindow(dpy, c->win);
			#endif // FAKEFULLSCREEN_CLIENT_PATCH
		}
	}

	focus(NULL);
	arrange(NULL);
}

