#include <Imlib2.h>

void
createpreview(Monitor *m)
{
	if (m->tagwin) {
		XMoveResizeWindow(
			dpy, m->tagwin,
			m->mx,
			m->bar->by + bh,
			m->mw / scalepreview,
			m->mh / scalepreview
		);
		return;
	}

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

	m->tagwin = XCreateWindow(dpy, root, m->wx, m->bar->by + bh, m->mw / scalepreview, m->mh / scalepreview, 0,
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

void
hidetagpreview(Monitor *m)
{
	m->previewshow = 0;
	XUnmapWindow(dpy, m->tagwin);
}

void
showtagpreview(int tag, int x, int y)
{
	Monitor *m = selmon;

	if (!m->tagwin)
		return;

	if (m->tagmap[tag]) {
		XSetWindowBackgroundPixmap(dpy, m->tagwin, m->tagmap[tag]);
		XCopyArea(dpy, m->tagmap[tag], m->tagwin, drw->gc, 0, 0, m->mw / scalepreview, m->mh / scalepreview, 0, 0);
		XMoveWindow(dpy, m->tagwin, x, y);
		XSync(dpy, False);
		XMapWindow(dpy, m->tagwin);
	} else
		XUnmapWindow(dpy, m->tagwin);
}

void
tagpreviewswitchtag(void)
{
	int i;
	unsigned int occ = 0;
	Client *c;
	Imlib_Image image;
	Monitor *m = selmon;

	if (!m->tagwin)
		createpreview(m);

	for (c = m->clients; c; c = c->next)
		occ |= c->tags;
	for (i = 0; i < NUMTAGS; i++) {
		if (m->tagset[m->seltags] & 1 << i) {
			if (m->tagmap[i] != 0) {
				XFreePixmap(dpy, m->tagmap[i]);
				m->tagmap[i] = 0;
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
				imlib_copy_drawable_to_image(0, m->mx, m->my, m->mw ,m->mh, 0, 0, 1);
				#if BAR_ALPHA_PATCH
				m->tagmap[i] = XCreatePixmap(dpy, m->tagwin, m->mw / scalepreview, m->mh / scalepreview, depth);
				#else
				m->tagmap[i] = XCreatePixmap(dpy, m->tagwin, m->mw / scalepreview, m->mh / scalepreview, DefaultDepth(dpy, screen));
				#endif // BAR_ALPHA_PATCH
				imlib_context_set_drawable(m->tagmap[i]);
				imlib_render_image_part_on_drawable_at_size(0, 0, m->mw, m->mh, 0, 0, m->mw / scalepreview, m->mh / scalepreview);
				imlib_free_image();
			}
		}
	}
}
