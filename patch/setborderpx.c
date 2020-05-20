void
setborderpx(const Arg *arg)
{
	Client *c;
	int prev_borderpx = selmon->borderpx;

	if (arg->i == 0)
		selmon->borderpx = borderpx;
	else if (selmon->borderpx + arg->i < 0)
		selmon->borderpx = 0;
	else
		selmon->borderpx += arg->i;

	for (c = selmon->clients; c; c = c->next)
	{
		if (c->bw + arg->i < 0)
			c->bw = selmon->borderpx = 0;
		else
			c->bw = selmon->borderpx;

		if (c->isfloating || !selmon->lt[selmon->sellt]->arrange)
		{
			if (arg->i != 0)
				resize(c, c->x, c->y, c->w-(arg->i*2), c->h-(arg->i*2), 0);
			else if (prev_borderpx > borderpx)
				resize(c, c->x, c->y, c->w + 2*(prev_borderpx - borderpx), c->h + 2*(prev_borderpx - borderpx), 0);
			else if (prev_borderpx < borderpx)
				resize(c, c->x, c->y, c->w - 2*(borderpx - prev_borderpx), c->h - 2*(borderpx - prev_borderpx), 0);
		}
	}
	arrange(selmon);
}