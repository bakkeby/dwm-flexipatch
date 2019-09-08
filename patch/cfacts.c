void
setcfact(const Arg *arg) {
	float f;
	Client *c;

	c = selmon->sel;

	if (!arg || !c || !selmon->lt[selmon->sellt]->arrange)
		return;
	f = arg->f + c->cfact;
	if (arg->f == 0.0)
		f = 1.0;
	else if (f < 0.25 || f > 4.0)
		return;
	c->cfact = f;
	arrange(selmon);
}