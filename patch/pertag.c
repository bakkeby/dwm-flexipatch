struct Pertag {
	unsigned int curtag, prevtag; /* current and previous tag */
	int nmasters[LENGTH(tags) + 1]; /* number of windows in master area */
	#if FLEXTILE_LAYOUT
	int ltaxes[LENGTH(tags) + 1][3];
	#endif // FLEXTILE_LAYOUT
	float mfacts[LENGTH(tags) + 1]; /* mfacts per tag */
	unsigned int sellts[LENGTH(tags) + 1]; /* selected layouts */
	const Layout *ltidxs[LENGTH(tags) + 1][2]; /* matrix of tags and layouts indexes  */
	#if PERTAGBAR_PATCH
	Bool showbars[LENGTH(tags) + 1]; /* display bar for the current tag */
	#endif // PERTAGBAR_PATCH
	#if ZOOMSWAP_PATCH
	Client *prevzooms[LENGTH(tags) + 1]; /* store zoom information */
	#endif // ZOOMSWAP_PATCH
};