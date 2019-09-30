struct Pertag {
	unsigned int curtag, prevtag; /* current and previous tag */
	int nmasters[LENGTH(tags) + 1]; /* number of windows in master area */
	#if FLEXTILE_DELUXE_LAYOUT
	int nstacks[LENGTH(tags) + 1]; /* number of windows in primary stack area */
	int ltaxis[LENGTH(tags) + 1][LTAXIS_LAST];
	const Layout *ltidxs[LENGTH(tags) + 1][3]; /* matrix of tags and layouts indexes  */
	#else
	const Layout *ltidxs[LENGTH(tags) + 1][2]; /* matrix of tags and layouts indexes  */
	#endif // FLEXTILE_DELUXE_LAYOUT
	float mfacts[LENGTH(tags) + 1]; /* mfacts per tag */
	unsigned int sellts[LENGTH(tags) + 1]; /* selected layouts */
	#if PERTAGBAR_PATCH
	Bool showbars[LENGTH(tags) + 1]; /* display bar for the current tag */
	#endif // PERTAGBAR_PATCH
	#if ZOOMSWAP_PATCH
	Client *prevzooms[LENGTH(tags) + 1]; /* store zoom information */
	#endif // ZOOMSWAP_PATCH
};

void
pertagview(const Arg *arg)
{
	int i;
	unsigned int tmptag;

	if (arg->ui & TAGMASK) {
		selmon->pertag->prevtag = selmon->pertag->curtag;
		selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
		if (arg->ui == ~0)
			selmon->pertag->curtag = 0;
		else {
			for (i=0; !(arg->ui & 1 << i); i++) ;
			selmon->pertag->curtag = i + 1;
		}
	} else {
		tmptag = selmon->pertag->prevtag;
		selmon->pertag->prevtag = selmon->pertag->curtag;
		selmon->pertag->curtag = tmptag;
	}
	selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];
	#if FLEXTILE_DELUXE_LAYOUT
	selmon->nstack = selmon->pertag->nstacks[selmon->pertag->curtag];
	#endif // FLEXTILE_DELUXE_LAYOUT
	selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];
	selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
	selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
	selmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];
	#if FLEXTILE_DELUXE_LAYOUT && MONITOR_RULES_PATCH
	selmon->ltaxis[LAYOUT] = selmon->pertag->ltaxis[selmon->pertag->curtag][LAYOUT];
	selmon->ltaxis[MASTER] = selmon->pertag->ltaxis[selmon->pertag->curtag][MASTER];
	selmon->ltaxis[STACK]  = selmon->pertag->ltaxis[selmon->pertag->curtag][STACK];
	selmon->ltaxis[STACK2] = selmon->pertag->ltaxis[selmon->pertag->curtag][STACK2];
	#endif // FLEXTILE_DELUXE_LAYOUT && MONITOR_RULES_PATCH
	#if PERTAGBAR_PATCH
	if (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])
		togglebar(NULL);
	#endif // PERTAGBAR_PATCH
	// strncpy(selmon->ltsymbol, selmon->lt[selmon->sellt]->symbol, sizeof selmon->ltsymbol); // ??
	// strncpy(m->ltsymbol, m->lt[m->sellt]->symbol, sizeof m->ltsymbol);
}