void
attachx(Client *c)
{
	#if ATTACHABOVE_PATCH
	Client *at;
	if (!(c->mon->sel == NULL || c->mon->sel == c->mon->clients || c->mon->sel->isfloating)) {
		for (at = c->mon->clients; at->next != c->mon->sel; at = at->next);
		c->next = at->next;
		at->next = c;
		return;
	}
	#elif ATTACHASIDE_PATCH
	Client *at;
	unsigned int n;

	for (at = c->mon->clients, n = 0; at; at = at->next)
		if (!at->isfloating && ISVISIBLEONTAG(at, c->tags))
			if (++n >= c->mon->nmaster)
				break;

	if (at && c->mon->nmaster) {
		c->next = at->next;
		at->next = c;
		return;
	}
	#elif ATTACHBELOW_PATCH
	if (!(c->mon->sel == NULL || c->mon->sel == c || c->mon->sel->isfloating)) {
		c->next = c->mon->sel->next;
		c->mon->sel->next = c;
		return;
	}
	#elif ATTACHBOTTOM_PATCH
	Client *at;
	for (at = c->mon->clients; at && at->next; at = at->next);
	if (at) {
		at->next = c;
		c->next = NULL;
		return;
	}
	#endif
	attach(c); // master (default)
}

