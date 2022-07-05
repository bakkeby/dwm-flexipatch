#include <Imlib2.h>

void
showtagpreview(int tag, int x, int y)
{
	if (selmon->tagmap[tag]) {
		XSetWindowBackgroundPixmap(dpy, selmon->tagwin, selmon->tagmap[tag]);
		XCopyArea(dpy, selmon->tagmap[tag], selmon->tagwin, drw->gc, 0, 0, selmon->mw / scalepreview, selmon->mh / scalepreview, 0, 0);
		XMoveWindow(dpy, selmon->tagwin, x, y);
		XSync(dpy, False);
		XMapWindow(dpy, selmon->tagwin);
	} else
		XUnmapWindow(dpy, selmon->tagwin);
}

void
hidetagpreview(Monitor *m)
{
	m->previewshow = 0;
	XUnmapWindow(dpy, m->tagwin);
}

void
tagpreviewswitchtag(void)
{
	int i;
	unsigned int occ = 0;
	Client *c;
	Imlib_Image image;

	for (c = selmon->clients; c; c = c->next)
		occ |= c->tags;
	for (i = 0; i < NUMTAGS; i++) {
		if (selmon->tagset[selmon->seltags] & 1 << i) {
					if (selmon->tagmap[i] != 0) {
				XFreePixmap(dpy, selmon->tagmap[i]);
				selmon->tagmap[i] = 0;
			}
			if (occ & 1 << i) {
				image = imlib_create_image(sw, sh);
				imlib_context_set_image(image);
				imlib_context_set_display(dpy);
				#if BAR_ALPHA_PATCH
				imlib_image_set_has_alpha(1);
				imlib_context_set_blend(0);
				imlib_context_set_visual(visual);
				#else
				imlib_context_set_visual(DefaultVisual(dpy, screen));
				#endif // BAR_ALPHA_PATCH
				imlib_context_set_drawable(root);
				imlib_copy_drawable_to_image(0, selmon->mx, selmon->my, selmon->mw ,selmon->mh, 0, 0, 1);
				#if BAR_ALPHA_PATCH
				selmon->tagmap[i] = XCreatePixmap(dpy, selmon->tagwin, selmon->mw / scalepreview, selmon->mh / scalepreview, depth);
				#else
				selmon->tagmap[i] = XCreatePixmap(dpy, selmon->tagwin, selmon->mw / scalepreview, selmon->mh / scalepreview, DefaultDepth(dpy, screen));
				#endif // BAR_ALPHA_PATCH
				imlib_context_set_drawable(selmon->tagmap[i]);
				imlib_render_image_part_on_drawable_at_size(0, 0, selmon->mw, selmon->mh, 0, 0, selmon->mw / scalepreview, selmon->mh / scalepreview);
				imlib_free_image();
			}
		}
	}
}

void
updatepreview(void)
{
	Monitor *m;

	XSetWindowAttributes wa = {
		.override_redirect = True,
		#if BAR_ALPHA_PATCH
		.background_pixel = 0,
		.border_pixel = 0,
		.colormap = cmap,
		#else
		.background_pixmap = ParentRelative,
		#endif // BAR_ALPHA_PATCH
		.event_mask = ButtonPressMask|ExposureMask
	};
	for (m = mons; m; m = m->next) {
		m->tagwin = XCreateWindow(dpy, root, m->wx, m->bar->by + bh, m->mw / 4, m->mh / 4, 0,
				#if BAR_ALPHA_PATCH
				depth, CopyFromParent, visual,
				CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa
				#else
				DefaultDepth(dpy, screen), CopyFromParent, DefaultVisual(dpy, screen),
				CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa
				#endif // BAR_ALPHA_PATCH
				);
		XDefineCursor(dpy, m->tagwin, cursor[CurNormal]->cursor);
		XMapRaised(dpy, m->tagwin);
		XUnmapWindow(dpy, m->tagwin);
	}
}
