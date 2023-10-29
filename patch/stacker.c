void
focusstack(const Arg *arg)
{
	int i = stackpos(arg);
	Client *c, *p;

	if (i < 0)
 		return;

	#if LOSEFULLSCREEN_PATCH
	if (!selmon->sel)
		return;
	#elif FAKEFULLSCREEN_CLIENT_PATCH
	if (!selmon->sel || (selmon->sel->isfullscreen && !selmon->sel->fakefullscreen))
		return;
	#else
	if (!selmon->sel || selmon->sel->isfullscreen)
		return;
	#endif // LOSEFULLSCREEN_PATCH

	#if BAR_WINTITLEACTIONS_PATCH
	for (p = NULL, c = selmon->clients; c && (i || !ISVISIBLE(c) || HIDDEN(c));
		i -= (ISVISIBLE(c) && !HIDDEN(c) ? 1 : 0), p = c, c = c->next);
	#else
	for (p = NULL, c = selmon->clients; c && (i || !ISVISIBLE(c));
		i -= (ISVISIBLE(c) ? 1 : 0), p = c, c = c->next);
	#endif // BAR_WINTITLEACTIONS_PATCH
	focus(c ? c : p);
	restack(selmon);
}

void
pushstack(const Arg *arg)
{
	int i = stackpos(arg);
	Client *sel = selmon->sel, *c, *p;

	if (i < 0)
		return;
	else if (i == 0) {
		detach(sel);
		attach(sel);
	}
	else {
		for (p = NULL, c = selmon->clients; c; p = c, c = c->next)
			#if BAR_WINTITLEACTIONS_PATCH
			if (!(i -= (ISVISIBLE(c) && !HIDDEN(c) && c != sel)))
			#else
			if (!(i -= (ISVISIBLE(c) && c != sel)))
			#endif // BAR_WINTITLEACTIONS_PATCH
				break;
		c = c ? c : p;
		detach(sel);
		sel->next = c->next;
		c->next = sel;
	}
	arrange(selmon);
}

int
stackpos(const Arg *arg)
{
	int n, i;
	Client *c, *l;

	if (!selmon->clients)
		return -1;

	#if BAR_WINTITLEACTIONS_PATCH
	if (arg->i == PREVSEL) {
		for (l = selmon->stack; l && (!ISVISIBLE(l) || HIDDEN(l) || l == selmon->sel); l = l->snext);
		if (!l)
			return -1;
		for (i = 0, c = selmon->clients; c != l; i += (ISVISIBLE(c) && !HIDDEN(c) ? 1 : 0), c = c->next);
		return i;
	}
	else if (ISINC(arg->i)) {
		if (!selmon->sel)
			return -1;
		for (i = 0, c = selmon->clients; c != selmon->sel; i += (ISVISIBLE(c) && !HIDDEN(c) ? 1 : 0), c = c->next);
		for (n = i; c; n += (ISVISIBLE(c) && !HIDDEN(c) ? 1 : 0), c = c->next);
		return MOD(i + GETINC(arg->i), n);
	}
	else if (arg->i < 0) {
		for (i = 0, c = selmon->clients; c; i += (ISVISIBLE(c) && !HIDDEN(c) ? 1 : 0), c = c->next);
		return MAX(i + arg->i, 0);
	}
	else
		return arg->i;
	#else // !BAR_WINTITLEACTIONS_PATCH
	if (arg->i == PREVSEL) {
		for (l = selmon->stack; l && (!ISVISIBLE(l) || l == selmon->sel); l = l->snext);
		if (!l)
			return -1;
		for (i = 0, c = selmon->clients; c != l; i += (ISVISIBLE(c) ? 1 : 0), c = c->next);
		return i;
	}
	else if (ISINC(arg->i)) {
		if (!selmon->sel)
			return -1;
		for (i = 0, c = selmon->clients; c != selmon->sel; i += (ISVISIBLE(c) ? 1 : 0), c = c->next);
		for (n = i; c; n += (ISVISIBLE(c) ? 1 : 0), c = c->next);
		return MOD(i + GETINC(arg->i), n);
	}
	else if (arg->i < 0) {
		for (i = 0, c = selmon->clients; c; i += (ISVISIBLE(c) ? 1 : 0), c = c->next);
		return MAX(i + arg->i, 0);
	}
	else
		return arg->i;
	#endif // BAR_WINTITLEACTIONS_PATCH
}

