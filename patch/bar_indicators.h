enum {
	INDICATOR_NONE,
	INDICATOR_TOP_LEFT_SQUARE,
	INDICATOR_TOP_LEFT_LARGER_SQUARE,
	INDICATOR_TOP_BAR,
	INDICATOR_BOTTOM_BAR,
	INDICATOR_CLIENT_DOTS,
	INDICATOR_RIGHT_TAGS
};

static void drawindicator(Monitor *m, Client *c, unsigned int occ, int x, int w, unsigned int tag, int filled, int invert, int type);