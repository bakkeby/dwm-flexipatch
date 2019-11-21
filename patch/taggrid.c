void drawtaggrid(Monitor *m, int *x_pos, unsigned int occ)
{
    unsigned int x, y, h, max_x, columns;
    int invert, i,j, k;

    h = bh / tagrows;
    x = max_x = *x_pos;
    y = 0;
    columns = LENGTH(tags) / tagrows + ((LENGTH(tags) % tagrows > 0) ? 1 : 0);

    /* Firstly we will fill the borders of squares */

    XSetForeground(drw->dpy, drw->gc, scheme[SchemeNorm][ColBorder].pixel);
    XFillRectangle(dpy, drw->drawable, drw->gc, x, y, h*columns + 1, bh);

    /* We will draw LENGTH(tags) squares in tagraws raws. */
    for (j = 0,  i= 0; j < tagrows; j++) {
        x = *x_pos;
        for (k = 0; k < columns && i < LENGTH(tags); k++, i++) {
            invert = m->tagset[m->seltags] & 1 << i ? 0 : 1;

            /* Select active color for current square */
            XSetForeground(drw->dpy, drw->gc, !invert ? scheme[SchemeSel][ColBg].pixel :
                                scheme[SchemeNorm][ColFg].pixel);
            XFillRectangle(dpy, drw->drawable, drw->gc, x+1, y+1, h-1, h-1);

            /* Mark square if tag has client */
            if (occ & 1 << i) {
                XSetForeground(drw->dpy, drw->gc, !invert ? scheme[SchemeSel][ColFg].pixel :
                                scheme[SchemeNorm][ColBg].pixel);
                XFillRectangle(dpy, drw->drawable, drw->gc, x + 1, y + 1,
                               h / 2, h / 2);
            }
            x += h;
            if (x > max_x) {
                max_x = x;
            }
        }
        y += h;
	}
    *x_pos = max_x + 1;
}

void switchtag(const Arg *arg)
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