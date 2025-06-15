void
togglealwaysontop(const Arg *arg)
{
	Client *c = selmon->sel;

	if (!c)
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfullscreen && !c->fakefullscreen)
	    return;
	#else
	if (c->isfullscreen)
	    return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // FAKEFULLSCREEN_PATCH

	c->alwaysontop = !c->alwaysontop;
	restack(selmon);
}
