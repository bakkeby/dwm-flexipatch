#include <X11/extensions/shape.h>

void drawroundedcorners(Client *c)
{
    if (corner_radius <= 0 || !c || c->isfullscreen)
    	return;

    Window win;
    win = c->win;
    if (!win)
    	return;

    XWindowAttributes win_attr;
    if (!XGetWindowAttributes(dpy, win, &win_attr))
    	return;

    int dia = 2 * corner_radius;
    int w = c->w;
    int h = c->h;
    if (w < dia || h < dia)
    	return;

    Pixmap mask;
    mask = XCreatePixmap(dpy, win, w, h, 1);
    if (!mask)
    	return;

    XGCValues xgcv;
    GC shape_gc;
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
    XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, mask, ShapeSet);
    XFreePixmap(dpy, mask);
    XFreeGC(dpy, shape_gc);
}

