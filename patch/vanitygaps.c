/* Settings */
static int enablegaps = 1;

static void
setgaps(int oh, int ov, int ih, int iv)
{
	if (oh < 0) oh = 0;
	if (ov < 0) ov = 0;
	if (ih < 0) ih = 0;
	if (iv < 0) iv = 0;

	selmon->gappoh = oh;
	selmon->gappov = ov;
	selmon->gappih = ih;
	selmon->gappiv = iv;
	arrange(selmon);
}

static void
togglegaps(const Arg *arg)
{
	enablegaps = !enablegaps;
	arrange(NULL);
}

static void
defaultgaps(const Arg *arg)
{
	setgaps(gappoh, gappov, gappih, gappiv);
}

static void
incrgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov + arg->i,
		selmon->gappih + arg->i,
		selmon->gappiv + arg->i
	);
}

static void
incrigaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov,
		selmon->gappih + arg->i,
		selmon->gappiv + arg->i
	);
}

static void
incrogaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov + arg->i,
		selmon->gappih,
		selmon->gappiv
	);
}

static void
incrohgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov,
		selmon->gappih,
		selmon->gappiv
	);
}

static void
incrovgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov + arg->i,
		selmon->gappih,
		selmon->gappiv
	);
}

static void
incrihgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov,
		selmon->gappih + arg->i,
		selmon->gappiv
	);
}

static void
incrivgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov,
		selmon->gappih,
		selmon->gappiv + arg->i
	);
}

static void
#if CFACTS_PATCH
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc, float *mf, float *sf)
#else
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc )
#endif // CFACTS_PATCH
{
	unsigned int n, oe = enablegaps, ie = enablegaps;
	#if CFACTS_PATCH
	float mfacts = 0, sfacts = 0;
	#endif // CFACTS_PATCH
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		#if CFACTS_PATCH
		if (!m->nmaster || n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
		#endif // CFACTS_PATCH
	}
	if (smartgaps && n == 1) {
		oe = 0; // outer gaps disabled when only one client
	}

	*oh = m->gappoh*oe; // outer horizontal gap
	*ov = m->gappov*oe; // outer vertical gap
	*ih = m->gappih*ie; // inner horizontal gap
	*iv = m->gappiv*ie; // inner vertical gap
	*nc = n;            // number of clients
	#if CFACTS_PATCH
	*mf = mfacts;       // total factor of master area
	*sf = sfacts;       // total factor of slave area
	#endif // CFACTS_PATCH
}