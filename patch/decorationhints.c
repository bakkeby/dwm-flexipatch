static Atom motifatom;

void
updatemotifhints(Client *c)
{
	Atom real;
	int format;
	unsigned char *p = NULL;
	unsigned long n, extra;
	unsigned long *motif;
	int width, height;

	if (!decorhints)
		return;

	if (XGetWindowProperty(dpy, c->win, motifatom, 0L, 5L, False, motifatom,
		                   &real, &format, &n, &extra, &p) == Success && p != NULL) {
		motif = (unsigned long*)p;
		if (motif[MWM_HINTS_FLAGS_FIELD] & MWM_HINTS_DECORATIONS) {
			width = WIDTH(c);
			height = HEIGHT(c);

			if (motif[MWM_HINTS_DECORATIONS_FIELD] & MWM_DECOR_ALL ||
				motif[MWM_HINTS_DECORATIONS_FIELD] & MWM_DECOR_BORDER ||
				motif[MWM_HINTS_DECORATIONS_FIELD] & MWM_DECOR_TITLE)
				#if SETBORDERPX_PATCH
				c->bw = c->oldbw = c->mon->borderpx;
				#else
				c->bw = c->oldbw = borderpx;
				#endif // SETBORDERPX_PATCH
			else
				c->bw = c->oldbw = 0;

			resize(c, c->x, c->y, width - (2*c->bw), height - (2*c->bw), 0);
		}
		XFree(p);
	}
}

