int
width_taggrid(Bar *bar, BarWidthArg *a)
{
	return (bh / 2) * (LENGTH(tags) / tagrows + ((LENGTH(tags) % tagrows > 0) ? 1 : 0)) + lrpad;
}

int
draw_taggrid(Bar *bar, BarDrawArg *a)
{
	unsigned int x, y, h, max_x = 0, columns, occ = 0;
	int invert, i,j, k;
	Client *c;

	for (c = bar->mon->clients; c; c = c->next)
		occ |= c->tags;

	max_x = x = a->x + lrpad / 2;
	h = bh / tagrows;
	y = 0;
	columns = LENGTH(tags) / tagrows + ((LENGTH(tags) % tagrows > 0) ? 1 : 0);

	/* Firstly we will fill the borders of squares */
	XSetForeground(drw->dpy, drw->gc, scheme[SchemeTagsNorm][ColBg].pixel);
	XFillRectangle(dpy, drw->drawable, drw->gc, x, y, h*columns + 1, bh);

	/* We will draw LENGTH(tags) squares in tagraws raws. */
	for (j = 0, i = 0; j < tagrows; j++) {
		x = a->x + lrpad / 2;
		for (k = 0; k < columns; k++, i++) {
			if (i < LENGTH(tags)) {
				invert = bar->mon->tagset[bar->mon->seltags] & 1 << i ? 0 : 1;

				/* Select active color for current square */
				XSetForeground(drw->dpy, drw->gc, !invert ? scheme[SchemeTagsSel][ColBg].pixel :
									scheme[SchemeTagsNorm][ColFg].pixel);
				XFillRectangle(dpy, drw->drawable, drw->gc, x+1, y+1, h-1, h-1);

				/* Mark square if tag has client */
				if (occ & 1 << i) {
					XSetForeground(drw->dpy, drw->gc, !invert ? scheme[SchemeTagsSel][ColFg].pixel :
									scheme[SchemeTagsNorm][ColBg].pixel);
					XFillRectangle(dpy, drw->drawable, drw->gc, x + 1, y + 1,
								   h / 2, h / 2);
				}
			} else {
				XSetForeground(drw->dpy, drw->gc, scheme[SchemeTagsNorm][ColBg].pixel);
				XFillRectangle(dpy, drw->drawable, drw->gc, x+1, y+1, h-1, h);
			}
			x += h;
			if (x > max_x) {
				max_x = x;
			}
		}
		y += h;
	}
	return 1;
}

int
click_taggrid(Bar *bar, Arg *arg, BarClickArg *a)
{
	unsigned int i, columns;

	columns = LENGTH(tags) / tagrows + ((LENGTH(tags) % tagrows > 0) ? 1 : 0);
	i = (a->rel_x - lrpad / 2) / (bh / tagrows) + columns * (a->rel_y / (bh / tagrows));
	if (i >= LENGTH(tags)) {
		i = LENGTH(tags) - 1;
	}
	arg->ui = 1 << i;
	return ClkTagBar;
}

void
switchtag(const Arg *arg)
{
	unsigned int columns;
	unsigned int new_tagset = 0;
	unsigned int pos, i;
	int col, row;
	Arg new_arg;

	columns = LENGTH(tags) / tagrows + ((LENGTH(tags) % tagrows > 0) ? 1 : 0);

	for (i = 0; i < LENGTH(tags); ++i) {
		if (!(selmon->tagset[selmon->seltags] & 1 << i)) {
			continue;
		}
		pos = i;
		row = pos / columns;
		col = pos % columns;
		if (arg->ui & SWITCHTAG_UP) {     /* UP */
			row --;
			if (row < 0) {
				row = tagrows - 1;
			}
			do {
				pos = row * columns + col;
				row --;
			} while (pos >= LENGTH(tags));
		}
		if (arg->ui & SWITCHTAG_DOWN) {     /* DOWN */
			row ++;
			if (row >= tagrows) {
				row = 0;
			}
			pos = row * columns + col;
			if (pos >= LENGTH(tags)) {
				row = 0;
			}
			pos = row * columns + col;
		}
		if (arg->ui & SWITCHTAG_LEFT) {     /* LEFT */
			col --;
			if (col < 0) {
				col = columns - 1;
			}
			do {
				pos = row * columns + col;
				col --;
			} while (pos >= LENGTH(tags));
		}
		if (arg->ui & SWITCHTAG_RIGHT) {     /* RIGHT */
			col ++;
			if (col >= columns) {
				col = 0;
			}
			pos = row * columns + col;
			if (pos >= LENGTH(tags)) {
				col = 0;
				pos = row * columns + col;
			}
		}
		new_tagset |= 1 << pos;
	}
	new_arg.ui = new_tagset;
	if (arg->ui & SWITCHTAG_TOGGLETAG) {
		toggletag(&new_arg);
	}
	if (arg->ui & SWITCHTAG_TAG) {
		tag(&new_arg);
	}
	if (arg->ui & SWITCHTAG_VIEW) {
		view (&new_arg);
	}
	if (arg->ui & SWITCHTAG_TOGGLEVIEW) {
		toggleview (&new_arg);
	}
}