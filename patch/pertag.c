struct Pertag {
	unsigned int curtag, prevtag; /* current tag index */
	int nmasters[NUMTAGS + 1]; /* number of windows in master area */
	#if FLEXTILE_DELUXE_LAYOUT
	int nstacks[NUMTAGS + 1]; /* number of windows in primary stack area */
	int ltaxis[NUMTAGS + 1][LTAXIS_LAST];
	const Layout *ltidxs[NUMTAGS + 1][3]; /* matrix of tags and layouts indexes  */
	#else
	const Layout *ltidxs[NUMTAGS + 1][2]; /* matrix of tags and layouts indexes  */
	#endif // FLEXTILE_DELUXE_LAYOUT
	float mfacts[NUMTAGS + 1]; /* mfacts per tag */
	unsigned int sellts[NUMTAGS + 1]; /* selected layouts */
	#if PERTAGBAR_PATCH
	int showbars[NUMTAGS + 1]; /* display bar for the current tag */
	#endif // PERTAGBAR_PATCH
	#if SWAPFOCUS_PATCH
	Client *prevclient[NUMTAGS + 1];
	#endif // SWAPFOCUS_PATCH
	#if ZOOMSWAP_PATCH
	Client *prevzooms[NUMTAGS + 1]; /* store zoom information */
	#endif // ZOOMSWAP_PATCH
	#if PERTAG_VANITYGAPS_PATCH && VANITYGAPS_PATCH
	int enablegaps[NUMTAGS + 1];
	int gaps[NUMTAGS + 1];
	#endif // PERTAG_VANITYGAPS_PATCH | VANITYGAPS_PATCH
	#if PERTAG_WITH_SEL_PATCH
	Client *sel[NUMTAGS + 1]; /* selected client */
	#endif // PERTAG_WITH_SEL_PATCH
};

void
pertagview(const Arg *arg)
{
	int i;

	#if PERTAG_WITH_SEL_PATCH
	unsigned int tmptag;
	if (arg->ui & TAGMASK) {
		selmon->pertag->prevtag = selmon->pertag->curtag;
	#endif // PERTAG_WITH_SEL_PATCH
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if (arg->ui == ~SPTAGMASK)
		#else
		if (arg->ui == ~0)
		#endif // SCRATCHPADS_PATCH
			selmon->pertag->curtag = 0;
		else {
			for (i = 0; !(selmon->tagset[selmon->seltags] & 1 << i); i++);
			selmon->pertag->curtag = i + 1;
		}
	#if PERTAG_WITH_SEL_PATCH
	} else {
	    tmptag = selmon->pertag->prevtag;
	    selmon->pertag->prevtag = selmon->pertag->curtag;
	    selmon->pertag->curtag = tmptag;
	}
	#endif // PERTAG_WITH_SEL_PATCH

	selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];
	#if FLEXTILE_DELUXE_LAYOUT
	selmon->nstack = selmon->pertag->nstacks[selmon->pertag->curtag];
	#endif // FLEXTILE_DELUXE_LAYOUT
	selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];
	selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
	selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
	selmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];

	#if PERTAG_VANITYGAPS_PATCH && VANITYGAPS_PATCH
	selmon->gappoh = (selmon->pertag->gaps[selmon->pertag->curtag] & 0xff) >> 0;
	selmon->gappov = (selmon->pertag->gaps[selmon->pertag->curtag] & 0xff00) >> 8;
	selmon->gappih = (selmon->pertag->gaps[selmon->pertag->curtag] & 0xff0000) >> 16;
	selmon->gappiv = (selmon->pertag->gaps[selmon->pertag->curtag] & 0xff000000) >> 24;
	#endif // PERTAG_VANITYGAPS_PATCH | VANITYGAPS_PATCH

	#if FLEXTILE_DELUXE_LAYOUT
	selmon->ltaxis[LAYOUT] = selmon->pertag->ltaxis[selmon->pertag->curtag][LAYOUT];
	selmon->ltaxis[MASTER] = selmon->pertag->ltaxis[selmon->pertag->curtag][MASTER];
	selmon->ltaxis[STACK]  = selmon->pertag->ltaxis[selmon->pertag->curtag][STACK];
	selmon->ltaxis[STACK2] = selmon->pertag->ltaxis[selmon->pertag->curtag][STACK2];
	#endif // FLEXTILE_DELUXE_LAYOUT

	#if PERTAGBAR_PATCH
	if (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])
		togglebar(NULL);
	#endif // PERTAGBAR_PATCH
}

void
pertagtoggleview(const Arg *arg)
{
	int i;
	unsigned int newtagset = selmon->tagset[selmon->seltags];

	#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
	if (newtagset == ~SPTAGMASK)
	#else
	if (newtagset == ~0)
	#endif // SCRATCHPADS_PATCH
	{
		selmon->pertag->curtag = 0;
	}
	/* test if the user did not select the same tag */
	if (!(newtagset & 1 << (selmon->pertag->curtag - 1))) {
		for (i = 0; !(newtagset & 1 << i); i++) ;
		selmon->pertag->curtag = i + 1;
	}

	/* apply settings for this view */
	selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];
	selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];
	selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
	selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
	selmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];

	#if PERTAGBAR_PATCH
	if (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])
		togglebar(NULL);
	#endif // PERTAGBAR_PATCH
}

