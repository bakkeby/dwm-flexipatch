void
monocle(Monitor *m)
{
	#if !MONOCLESYMBOL_PATCH
	unsigned int n = 0;
	#endif // MONOCLESYMBOL_PATCH
	Client *c;

	#if !MONOCLESYMBOL_PATCH
	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	if (n > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
	#endif // MONOCLESYMBOL_PATCH
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}