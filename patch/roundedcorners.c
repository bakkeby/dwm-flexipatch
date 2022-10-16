#include <X11/extensions/shape.h>

void drawroundedcorners(Client *c)
{
	XWindowAttributes win_attr;
	Pixmap mask;
	XGCValues xgcv;
	GC shape_gc;
	int dia, w, h;

	if (corner_radius <= 0 || !c)
		return;

	/* Clear window shape if fullscreen */
	if (c->w == c->mon->mw && c->h == c->mon->mh) {
		XRectangle rect = { .x = 0, .y = 0, .width = c->w, .height = c->h };
		XShapeCombineRectangles(dpy, c->win, ShapeBounding, 0, 0, &rect, 1, ShapeSet, 1);
		return;
	}

	if (!XGetWindowAttributes(dpy, c->win, &win_attr))
		return;

	dia = 2 * corner_radius;
	w = c->w + 2 * c->bw;
	h = c->h + 2 * c->bw;
	if (w < dia || h < dia)
		return;

	mask = XCreatePixmap(dpy, c->win, w, h, 1);
	if (!mask)
		return;

	shape_gc = XCreateGC(dpy, mask, 0, &xgcv);
	if (!shape_gc) {
		XFreePixmap(dpy, mask);
		free(shape_gc);
		return;
	}

	XSetForeground(dpy, shape_gc, 0);
	XFillRectangle(dpy, mask, shape_gc, 0, 0, w, h);
	XSetForeground(dpy, shape_gc, 1);
	XFillArc(dpy, mask, shape_gc, 0, 0, dia, dia, 0, 23040);
	XFillArc(dpy, mask, shape_gc, w-dia-1, 0, dia, dia, 0, 23040);
	XFillArc(dpy, mask, shape_gc, 0, h-dia-1, dia, dia, 0, 23040);
	XFillArc(dpy, mask, shape_gc, w-dia-1, h-dia-1, dia, dia, 0, 23040);
	XFillRectangle(dpy, mask, shape_gc, corner_radius, 0, w-dia, h);
	XFillRectangle(dpy, mask, shape_gc, 0, corner_radius, w, h-dia);
	XShapeCombineMask(dpy, c->win, ShapeBounding, -c->bw, -c->bw, mask, ShapeSet);
	XFreePixmap(dpy, mask);
	XFreeGC(dpy, shape_gc);
}
