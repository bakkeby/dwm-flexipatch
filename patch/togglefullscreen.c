void
togglefullscreen(const Arg *arg)
{
	Client *c = selmon->sel;
	if (!c)
		return;

	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	if (c->fakefullscreen == 1) {
		c->fakefullscreen = 2;
		if (c->isfullscreen)
			c->isfullscreen = 0;
	} else if (c->fakefullscreen == 2) {
		c->fakefullscreen = 0;
		togglefakefullscreen(NULL);
		return;
	}
	#endif // FAKEFULLSCREEN_CLIENT_PATCH

	setfullscreen(c, !c->isfullscreen);
}
