static uint32_t tmpicon[ICONSIZE * ICONSIZE];

static uint32_t prealpha(uint32_t p) {
	uint8_t a = p >> 24u;
	uint32_t rb = (a * (p & 0xFF00FFu)) >> 8u;
	uint32_t g = (a * (p & 0x00FF00u)) >> 8u;
	return (rb & 0xFF00FFu) | (g & 0x00FF00u) | ((~a) << 24u);
}

#if BAR_ALPHA_PATCH
static uint8_t div255(uint16_t x) { return (x*0x8081u) >> 23u; }
static uint32_t blend(uint32_t p1rb, uint32_t p1g, uint8_t p1a, uint32_t p2) {
	uint8_t a = p2 >> 24u;
	uint32_t rb = (p2 & 0xFF00FFu) + ( (a * p1rb) >> 8u );
	uint32_t g = (p2 & 0x00FF00u) + ( (a * p1g) >> 8u );
	return (rb & 0xFF00FFu) | (g & 0x00FF00u) | div255(~a * 255u + a * p1a) << 24u;
}

void
drw_img(Drw *drw, int x, int y, XImage *img, uint32_t *tmp)
{
	if (!drw || !drw->scheme)
		return;
	uint32_t *data = (uint32_t *)img->data, p = drw->scheme[ColBg].pixel,
			prb = p & 0xFF00FFu, pg = p & 0x00FF00u;
	uint8_t pa = p >> 24u;
	int icsz = img->width * img->height, i;
	for (i = 0; i < icsz; ++i) tmp[i] = blend(prb, pg, pa, data[i]);

	img->data = (char *) tmp;
	XPutImage(drw->dpy, drw->drawable, drw->gc, img, 0, 0, x, y, img->width, img->height);
	img->data = (char *) data;
}
#else
static uint32_t blend(uint32_t p1rb, uint32_t p1g, uint32_t p2) {
	uint8_t a = p2 >> 24u;
	uint32_t rb = (p2 & 0xFF00FFu) + ( (a * p1rb) >> 8u );
	uint32_t g = (p2 & 0x00FF00u) + ( (a * p1g) >> 8u );
	return (rb & 0xFF00FFu) | (g & 0x00FF00u) | ((~a) << 24u);
}

void
drw_img(Drw *drw, int x, int y, XImage *img, uint32_t *tmp)
{
	if (!drw || !drw->scheme)
		return;
	uint32_t *data = (uint32_t *)img->data, p = drw->scheme[ColBg].pixel, prb = p & 0xFF00FFu, pg = p & 0x00FF00u;
	int icsz = img->width * img->height, i;
	for (i = 0; i < icsz; ++i) tmp[i] = blend(prb, pg, data[i]);
	img->data = (char *) tmp;
	XPutImage(drw->dpy, drw->drawable, drw->gc, img, 0, 0, x, y, img->width, img->height);
	img->data = (char *) data;
}
#endif // BAR_ALPHA_PATCH

XImage *
geticonprop(Window win)
{
	int format;
	unsigned long n, extra, *p = NULL;
	Atom real;

	if (XGetWindowProperty(dpy, win, netatom[NetWMIcon], 0L, LONG_MAX, False, AnyPropertyType,
						   &real, &format, &n, &extra, (unsigned char **)&p) != Success)
		return NULL;
	if (n == 0 || format != 32) { XFree(p); return NULL; }

	unsigned long *bstp = NULL;
	uint32_t w, h, sz;

	{
		const unsigned long *end = p + n;
		unsigned long *i;
		uint32_t bstd = UINT32_MAX, d, m;
		for (i = p; i < end - 1; i += sz) {
			if ((w = *i++) > UINT16_MAX || (h = *i++) > UINT16_MAX) { XFree(p); return NULL; }
			if ((sz = w * h) > end - i) break;
			if ((m = w > h ? w : h) >= ICONSIZE && (d = m - ICONSIZE) < bstd) { bstd = d; bstp = i; }
		}
		if (!bstp) {
			for (i = p; i < end - 1; i += sz) {
				if ((w = *i++) > UINT16_MAX || (h = *i++) > UINT16_MAX) { XFree(p); return NULL; }
				if ((sz = w * h) > end - i) break;
				if ((d = ICONSIZE - (w > h ? w : h)) < bstd) { bstd = d; bstp = i; }
			}
		}
		if (!bstp) { XFree(p); return NULL; }
	}

	if ((w = *(bstp - 2)) == 0 || (h = *(bstp - 1)) == 0) { XFree(p); return NULL; }

	uint32_t icw, ich, icsz;
	if (w <= h) {
		ich = ICONSIZE; icw = w * ICONSIZE / h;
		if (icw == 0) icw = 1;
	}
	else {
		icw = ICONSIZE; ich = h * ICONSIZE / w;
		if (ich == 0) ich = 1;
	}
	icsz = icw * ich;

	uint32_t i;
#if ULONG_MAX > UINT32_MAX
	uint32_t *bstp32 = (uint32_t *)bstp;
	for (sz = w * h, i = 0; i < sz; ++i) bstp32[i] = bstp[i];
#endif
	uint32_t *icbuf = malloc(icsz << 2); if(!icbuf) { XFree(p); return NULL; }
	if (w == icw && h == ich) memcpy(icbuf, bstp, icsz << 2);
	else {
		Imlib_Image origin = imlib_create_image_using_data(w, h, (DATA32 *)bstp);
		if (!origin) { XFree(p); free(icbuf); return NULL; }
		imlib_context_set_image(origin);
		imlib_image_set_has_alpha(1);
		Imlib_Image scaled = imlib_create_cropped_scaled_image(0, 0, w, h, icw, ich);
		imlib_free_image_and_decache();
		if (!scaled) { XFree(p); free(icbuf); return NULL; }
		imlib_context_set_image(scaled);
		imlib_image_set_has_alpha(1);
		memcpy(icbuf, imlib_image_get_data_for_reading_only(), icsz << 2);
		imlib_free_image_and_decache();
	}
	XFree(p);
	for (i = 0; i < icsz; ++i) icbuf[i] = prealpha(icbuf[i]);
	#if BAR_ALPHA_PATCH
	return XCreateImage(dpy, drw->visual, drw->depth, ZPixmap, 0, (char *)icbuf, icw, ich, 32, 0);
	#else
	return XCreateImage(dpy, DefaultVisual(dpy, screen), DefaultDepth(dpy, screen), ZPixmap, 0, (char *)icbuf, icw, ich, 32, 0);
	#endif // BAR_ALPHA_PATCH
}

void
freeicon(Client *c)
{
	if (c->icon) {
		XDestroyImage(c->icon);
		c->icon = NULL;
	}
}

void
updateicon(Client *c)
{
	freeicon(c);
	c->icon = geticonprop(c->win);
}