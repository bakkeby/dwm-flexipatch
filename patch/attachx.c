void
attachx(Client *c)
{
	#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBOTTOM_PATCH || SEAMLESS_RESTART_PATCH
	Client *at;
	#endif // ATTACHABOVE_PATCH | ATTACHASIDE_PATCH | ATTACHBOTTOM_PATCH | SEAMLESS_RESTART_PATCH

	#if SEAMLESS_RESTART_PATCH
	if (c->idx > 0) { /* then the client has a designated position in the client list */
		for (at = c->mon->clients; at; at = at->next) {
			if (c->idx < at->idx) {
				c->next = at;
				c->mon->clients = c;
				return;
			} else if (at->idx <= c->idx && (!at->next || c->idx <= at->next->idx)) {
				c->next = at->next;
				at->next = c;
				return;
			}
		}
	}
	#endif // SEAMLESS_RESTART_PATCH

	#if ATTACHABOVE_PATCH
	if (!(c->mon->sel == NULL || c->mon->sel == c->mon->clients || c->mon->sel->isfloating)) {
		for (at = c->mon->clients; at->next != c->mon->sel; at = at->next);
		c->next = at->next;
		at->next = c;
		return;
	}
	#elif ATTACHASIDE_PATCH
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
	for (at = c->mon->clients; at && at->next; at = at->next);
	if (at) {
		at->next = c;
		c->next = NULL;
		return;
	}
	#endif
	attach(c); // master (default)
}

