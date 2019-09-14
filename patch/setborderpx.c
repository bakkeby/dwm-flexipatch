void
setborderpx(const Arg *arg)
{
	Client *c;

	if (arg->i == 0)
		mons->borderpx = borderpx;
	else if (mons->borderpx + arg->i < 0)
		mons->borderpx = 0;
	else
		mons->borderpx += arg->i;

	for (c = mons->clients; c; c = c->next)
		if (c->bw + arg->i < 0)
			c->bw = mons->borderpx = 0;
		else
			c->bw = mons->borderpx;

	arrange(selmon);
}