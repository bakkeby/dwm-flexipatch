Client *
recttoclient(int x, int y, int w, int h, int include_floating)
{
	Client *c, *r = NULL;
	int a, area = 1;

	for (c = selmon->stack; c; c = c->snext) {
		if (!ISVISIBLE(c) || (c->isfloating && !include_floating))
			continue;
		if ((a = INTERSECTC(x, y, w, h, c)) >= area) {
			area = a;
			r = c;
		}
	}
	return r;
}
