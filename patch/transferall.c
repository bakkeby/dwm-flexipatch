void
transferall(const Arg *arg)
{
	Client *c, *n = selmon->clients, *attachfrom = NULL;
	int i = 0, nstackclients = 0;
	while (n) {
		c = n;
		n = c->next;
		if (!ISVISIBLE(c) || c->isfloating) continue;
		if (i >= selmon->nmaster) {
			detach(c);
			if (!attachfrom) {
				attach(c);
			} else {
				c->next = attachfrom->next;
				attachfrom->next = c;
			}
			attachfrom = c;
			nstackclients++;
		}
		i++;
	}
	selmon->nmaster = nstackclients;
	arrange(selmon);
}

