
#if !PERTAG_PATCH
static Client *prevzoom = NULL;
#endif // PERTAG_PATCH

Client *
findbefore(Client *c) {
	Client *p;
	if (!c || c == c->mon->clients)
		return NULL;
	for (p = c->mon->clients; p && p->next != c; p = p->next);
	return p;
}

