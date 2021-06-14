#if VANITYGAPS_PATCH && VANITYGAPS_MONOCLE_PATCH
void
monocle(Monitor *m)
{
	unsigned int n;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	#if !MONOCLESYMBOL_PATCH
	if (n > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
	#endif // MONOCLESYMBOL_PATCH
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2 * c->bw - 2 * ov, m->wh - 2 * c->bw - 2 * oh, 0);
}
#else
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
#endif // VANITYGAPS_PATCH

