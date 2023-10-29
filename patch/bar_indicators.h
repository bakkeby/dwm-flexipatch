enum {
	INDICATOR_NONE,
	INDICATOR_TOP_LEFT_SQUARE,
	INDICATOR_TOP_LEFT_LARGER_SQUARE,
	INDICATOR_TOP_BAR,
	INDICATOR_TOP_BAR_SLIM,
	INDICATOR_BOTTOM_BAR,
	INDICATOR_BOTTOM_BAR_SLIM,
	INDICATOR_BOX,
	INDICATOR_BOX_WIDER,
	INDICATOR_BOX_FULL,
	INDICATOR_CLIENT_DOTS,
	INDICATOR_RIGHT_TAGS,
	INDICATOR_PLUS,
	INDICATOR_PLUS_AND_SQUARE,
	INDICATOR_PLUS_AND_LARGER_SQUARE,
};

static void drawindicator(Monitor *m, Client *c, unsigned int occ, int x, int y, int w, int h, unsigned int tag, int filled, int invert, int type);
static void drawstateindicator(Monitor *m, Client *c, unsigned int occ, int x, int y, int w, int h, unsigned int tag, int filled, int invert);

