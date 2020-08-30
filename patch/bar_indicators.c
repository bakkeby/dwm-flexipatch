/* Indicator properties, you can override these in your config.h if you want. */
#ifndef TAGSINDICATOR
#define TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#endif
#ifndef TAGSPX
#define TAGSPX 5        // # pixels for tag grid boxes
#endif
#ifndef TAGSROWS
#define TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
#endif

void
drawindicator(Monitor *m, Client *c, unsigned int occ, int x, int w, unsigned int tag, int filled, int invert, int type)
{
	int i, boxw, boxs, indn = 0;
	if (!(occ & 1 << tag) || type == INDICATOR_NONE)
		return;

	boxs = drw->fonts->h / 9;
	boxw = drw->fonts->h / 6 + 2;
	if (filled == -1)
		filled = m == selmon && m->sel && m->sel->tags & 1 << tag;

	switch (type) {
	default:
	case INDICATOR_TOP_LEFT_SQUARE:
		drw_rect(drw, x + boxs, boxs, boxw, boxw, filled, invert);
		break;
	case INDICATOR_TOP_LEFT_LARGER_SQUARE:
		drw_rect(drw, x + boxs + 2, boxs+1, boxw+1, boxw+1, filled, invert);
		break;
	case INDICATOR_TOP_BAR:
		drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw/2, filled, invert);
		break;
	case INDICATOR_TOP_BAR_SLIM:
		drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), 1, 0, invert);
		break;
	case INDICATOR_BOTTOM_BAR:
		drw_rect(drw, x + boxw, bh - boxw/2, w - ( 2 * boxw + 1), boxw/2, filled, invert);
		break;
	case INDICATOR_BOTTOM_BAR_SLIM:
		drw_rect(drw, x + boxw, bh - 1, w - ( 2 * boxw + 1), 1, 0, invert);
		break;
	case INDICATOR_BOX:
		drw_rect(drw, x + boxw, 0, w - 2 * boxw, bh, 0, invert);
		break;
	case INDICATOR_BOX_WIDER:
		drw_rect(drw, x + boxw/2, 0, w - boxw, bh, 0, invert);
		break;
	case INDICATOR_CLIENT_DOTS:
		for (c = m->clients; c; c = c->next) {
			if (c->tags & (1 << tag)) {
				drw_rect(drw, x, 1 + (indn * 2), m->sel == c ? 6 : 1, 1, 1, invert);
				indn++;
			}
			if (bh <= 1 + (indn * 2)) {
				indn = 0;
				x += 2;
			}
		}
		break;
	case INDICATOR_RIGHT_TAGS:
		if (!c)
			break;
		for (i = 0; i < NUMTAGS; i++) {
			drw_rect(drw,
				( x + w - 2 - ((NUMTAGS / TAGSROWS) * TAGSPX)
					- (i % (NUMTAGS/TAGSROWS)) + ((i % (NUMTAGS / TAGSROWS)) * TAGSPX)
				),
				( 2 + ((i / (NUMTAGS/TAGSROWS)) * TAGSPX)
					- ((i / (NUMTAGS/TAGSROWS)))
				),
				TAGSPX, TAGSPX, (c->tags >> i) & 1, 0
			);
		}
		break;
	}
}