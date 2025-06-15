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
drawindicator(Monitor *m, Client *c, unsigned int occ, int x, int y, int w, int h, unsigned int tag, int filled, int invert, int type)
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
		drw_rect(drw, x + boxs, y + boxs, boxw, boxw, filled, invert);
		break;
	case INDICATOR_TOP_LEFT_LARGER_SQUARE:
		drw_rect(drw, x + boxs + 2, y + boxs+1, boxw+1, boxw+1, filled, invert);
		break;
	case INDICATOR_TOP_BAR:
		drw_rect(drw, x + boxw, y, w - ( 2 * boxw + 1), boxw/2, filled, invert);
		break;
	case INDICATOR_TOP_BAR_SLIM:
		drw_rect(drw, x + boxw, y, w - ( 2 * boxw + 1), 1, 0, invert);
		break;
	case INDICATOR_BOTTOM_BAR:
		drw_rect(drw, x + boxw, y + h - boxw/2, w - ( 2 * boxw + 1), boxw/2, filled, invert);
		break;
	case INDICATOR_BOTTOM_BAR_SLIM:
		drw_rect(drw, x + boxw, y + h - 1, w - ( 2 * boxw + 1), 1, 0, invert);
		break;
	case INDICATOR_BOX:
		drw_rect(drw, x + boxw, y, w - 2 * boxw, h, 0, invert);
		break;
	case INDICATOR_BOX_WIDER:
		drw_rect(drw, x + boxw/2, y, w - boxw, h, 0, invert);
		break;
	case INDICATOR_BOX_FULL:
		drw_rect(drw, x, y, w - 2, h, 0, invert);
		break;
	case INDICATOR_CLIENT_DOTS:
		for (c = m->clients; c; c = c->next) {
			if (c->tags & (1 << tag)) {
				drw_rect(drw, x, 1 + (indn * 2), m->sel == c ? 6 : 1, 1, 1, invert);
				indn++;
			}
			if (h <= 1 + (indn * 2)) {
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
				( y + 2 + ((i / (NUMTAGS/TAGSROWS)) * TAGSPX)
					- ((i / (NUMTAGS/TAGSROWS)))
				),
				TAGSPX, TAGSPX, (c->tags >> i) & 1, 0
			);
		}
		break;
	case INDICATOR_PLUS_AND_LARGER_SQUARE:
		boxs += 2;
		boxw += 2;
		/* falls through */
	case INDICATOR_PLUS_AND_SQUARE:
		drw_rect(drw, x + boxs, y + boxs, boxw % 2 ? boxw : boxw + 1, boxw % 2 ? boxw : boxw + 1, filled, invert);
		/* falls through */
	case INDICATOR_PLUS:
		if (!(boxw % 2))
			boxw += 1;
		drw_rect(drw, x + boxs + boxw / 2, y + boxs, 1, boxw, filled, invert); // |
		drw_rect(drw, x + boxs, y + boxs + boxw / 2, boxw + 1, 1, filled, invert); // ‒
		break;
	}
}

void
drawstateindicator(Monitor *m, Client *c, unsigned int occ, int x, int y, int w, int h, unsigned int tag, int filled, int invert)
{
	#if FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
	if (c->fakefullscreen && c->isfloating)
		drawindicator(m, c, occ, x, y, w, h, tag, filled, invert, floatfakefsindicatortype);
	else if (c->fakefullscreen)
		drawindicator(m, c, occ, x, y, w, h, tag, filled, invert, fakefsindicatortype);
	else
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfloating)
		drawindicator(m, c, occ, x, y, w, h, tag, filled, invert, floatindicatortype);
	else
		drawindicator(m, c, occ, x, y, w, h, tag, filled, invert, tiledindicatortype);
	#if ALWAYSONTOP_PATCH
	if (c->isfloating && c->alwaysontop)
		drawindicator(m, c, occ, x, y, w, h, tag, filled, invert, aotindicatortype);
	#endif // ALWAYSONTOP_PATCH
}

