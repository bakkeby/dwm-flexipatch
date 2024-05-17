/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "patches.h"
#include "drw.h"
#include "util.h"

#if BIDI_PATCH
#include <fribidi.h>

static char fribidi_text[BUFSIZ] = "";

static void
apply_fribidi(const char *str)
{
	FriBidiStrIndex len = strlen(str);
	FriBidiChar logical[BUFSIZ];
	FriBidiChar visual[BUFSIZ];
	FriBidiParType base = FRIBIDI_PAR_ON;
	FriBidiCharSet charset;

	fribidi_text[0] = 0;
	if (len > 0) {
		charset = fribidi_parse_charset("UTF-8");
		len = fribidi_charset_to_unicode(charset, str, len, logical);
		fribidi_log2vis(logical, len, &base, visual, NULL, NULL, NULL);
		len = fribidi_unicode_to_charset(charset, visual, len, fribidi_text);
	}
}
#endif

#if !BAR_PANGO_PATCH
#define UTF_INVALID 0xFFFD
#define UTF_SIZ     4

static const unsigned char utfbyte[UTF_SIZ + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0};
static const unsigned char utfmask[UTF_SIZ + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
static const long utfmin[UTF_SIZ + 1] = {       0,    0,  0x80,  0x800,  0x10000};
static const long utfmax[UTF_SIZ + 1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};
#endif // BAR_PANGO_PATCH

#if BAR_POWERLINE_TAGS_PATCH || BAR_POWERLINE_STATUS_PATCH
Clr transcheme[3];
#endif // BAR_POWERLINE_TAGS_PATCH | BAR_POWERLINE_STATUS_PATCH

#if !BAR_PANGO_PATCH
static long
utf8decodebyte(const char c, size_t *i)
{
	for (*i = 0; *i < (UTF_SIZ + 1); ++(*i))
		if (((unsigned char)c & utfmask[*i]) == utfbyte[*i])
			return (unsigned char)c & ~utfmask[*i];
	return 0;
}

static size_t
utf8validate(long *u, size_t i)
{
	if (!BETWEEN(*u, utfmin[i], utfmax[i]) || BETWEEN(*u, 0xD800, 0xDFFF))
		*u = UTF_INVALID;
	for (i = 1; *u > utfmax[i]; ++i)
		;
	return i;
}

static size_t
utf8decode(const char *c, long *u, size_t clen)
{
	size_t i, j, len, type;
	long udecoded;

	*u = UTF_INVALID;
	if (!clen)
		return 0;
	udecoded = utf8decodebyte(c[0], &len);
	if (!BETWEEN(len, 1, UTF_SIZ))
		return 1;
	for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
		udecoded = (udecoded << 6) | utf8decodebyte(c[i], &type);
		if (type)
			return j;
	}
	if (j < len)
		return 0;
	*u = udecoded;
	utf8validate(u, len);

	return len;
}
#endif // BAR_PANGO_PATCH

Drw *
#if BAR_ALPHA_PATCH
drw_create(Display *dpy, int screen, Window root, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap)
#else
drw_create(Display *dpy, int screen, Window root, unsigned int w, unsigned int h)
#endif // BAR_ALPHA_PATCH
{
	Drw *drw = ecalloc(1, sizeof(Drw));

	drw->dpy = dpy;
	drw->screen = screen;
	drw->root = root;
	drw->w = w;
	drw->h = h;

	#if BAR_ALPHA_PATCH
	drw->visual = visual;
	drw->depth = depth;
	drw->cmap = cmap;
	drw->drawable = XCreatePixmap(dpy, root, w, h, depth);
	#if BAR_WINICON_PATCH
	drw->picture = XRenderCreatePicture(dpy, drw->drawable, XRenderFindVisualFormat(dpy, visual), 0, NULL);
	#endif // BAR_WINICON_PATCH
	drw->gc = XCreateGC(dpy, drw->drawable, 0, NULL);
	#else
	drw->drawable = XCreatePixmap(dpy, root, w, h, DefaultDepth(dpy, screen));
	#if BAR_WINICON_PATCH
	drw->picture = XRenderCreatePicture(dpy, drw->drawable, XRenderFindVisualFormat(dpy, DefaultVisual(dpy, screen)), 0, NULL);
	#endif // BAR_WINICON_PATCH
	drw->gc = XCreateGC(dpy, root, 0, NULL);
	#endif // BAR_ALPHA_PATCH
	XSetLineAttributes(dpy, drw->gc, 1, LineSolid, CapButt, JoinMiter);

	return drw;
}

void
drw_resize(Drw *drw, unsigned int w, unsigned int h)
{
	if (!drw)
		return;

	drw->w = w;
	drw->h = h;
	#if BAR_WINICON_PATCH
	if (drw->picture)
		XRenderFreePicture(drw->dpy, drw->picture);
	#endif // BAR_WINICON_PATCH
	if (drw->drawable)
		XFreePixmap(drw->dpy, drw->drawable);
	#if BAR_ALPHA_PATCH
	drw->drawable = XCreatePixmap(drw->dpy, drw->root, w, h, drw->depth);
	#if BAR_WINICON_PATCH
	drw->picture = XRenderCreatePicture(drw->dpy, drw->drawable, XRenderFindVisualFormat(drw->dpy, drw->visual), 0, NULL);
	#endif // BAR_WINICON_PATCH
	#else // !BAR_ALPHA_PATCH
	drw->drawable = XCreatePixmap(drw->dpy, drw->root, w, h, DefaultDepth(drw->dpy, drw->screen));
	#if BAR_WINICON_PATCH
	drw->picture = XRenderCreatePicture(drw->dpy, drw->drawable, XRenderFindVisualFormat(drw->dpy, DefaultVisual(drw->dpy, drw->screen)), 0, NULL);
	#endif // BAR_WINICON_PATCH
	#endif // BAR_ALPHA_PATCH
}

void
drw_free(Drw *drw)
{
	#if BAR_WINICON_PATCH
	XRenderFreePicture(drw->dpy, drw->picture);
	#endif // BAR_WINICON_PATCH
	XFreePixmap(drw->dpy, drw->drawable);
	XFreeGC(drw->dpy, drw->gc);
	drw_fontset_free(drw->fonts);
	free(drw);
}

#if BAR_PANGO_PATCH
/* This function is an implementation detail. Library users should use
 * drw_font_create instead.
 */
static Fnt *
xfont_create(Drw *drw, const char *fontname)
{
	Fnt *font;
	PangoFontMap *fontmap;
	PangoContext *context;
	PangoFontDescription *desc;
	PangoFontMetrics *metrics;

	if (!fontname) {
		die("no font specified.");
	}

	font = ecalloc(1, sizeof(Fnt));
	font->dpy = drw->dpy;

	fontmap = pango_xft_get_font_map(drw->dpy, drw->screen);
	context = pango_font_map_create_context(fontmap);
	desc = pango_font_description_from_string(fontname);
	font->layout = pango_layout_new(context);
	pango_layout_set_font_description(font->layout, desc);

	metrics = pango_context_get_metrics(context, desc, pango_language_from_string ("en-us"));
	font->h = pango_font_metrics_get_height(metrics) / PANGO_SCALE;

	pango_font_metrics_unref(metrics);
	g_object_unref(context);

	return font;
}
#else
/* This function is an implementation detail. Library users should use
 * drw_fontset_create instead.
 */
static Fnt *
xfont_create(Drw *drw, const char *fontname, FcPattern *fontpattern)
{
	Fnt *font;
	XftFont *xfont = NULL;
	FcPattern *pattern = NULL;

	if (fontname) {
		/* Using the pattern found at font->xfont->pattern does not yield the
		 * same substitution results as using the pattern returned by
		 * FcNameParse; using the latter results in the desired fallback
		 * behaviour whereas the former just results in missing-character
		 * rectangles being drawn, at least with some fonts. */
		if (!(xfont = XftFontOpenName(drw->dpy, drw->screen, fontname))) {
			fprintf(stderr, "error, cannot load font from name: '%s'\n", fontname);
			return NULL;
		}
		if (!(pattern = FcNameParse((FcChar8 *) fontname))) {
			fprintf(stderr, "error, cannot parse font name to pattern: '%s'\n", fontname);
			XftFontClose(drw->dpy, xfont);
			return NULL;
		}
	} else if (fontpattern) {
		if (!(xfont = XftFontOpenPattern(drw->dpy, fontpattern))) {
			fprintf(stderr, "error, cannot load font from pattern.\n");
			return NULL;
		}
	} else {
		die("no font specified.");
	}

	#if BAR_NO_COLOR_EMOJI_PATCH
	/* Do not allow using color fonts. This is a workaround for a BadLength
	 * error from Xft with color glyphs. Modelled on the Xterm workaround. See
	 * https://bugzilla.redhat.com/show_bug.cgi?id=1498269
	 * https://lists.suckless.org/dev/1701/30932.html
	 * https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=916349
	 * and lots more all over the internet.
	 */
	FcBool iscol;
	if (FcPatternGetBool(xfont->pattern, FC_COLOR, 0, &iscol) == FcResultMatch && iscol) {
		XftFontClose(drw->dpy, xfont);
		return NULL;
	}
	#endif // BAR_NO_COLOR_EMOJI_PATCH

	font = ecalloc(1, sizeof(Fnt));
	font->xfont = xfont;
	font->pattern = pattern;
	font->h = xfont->ascent + xfont->descent;
	font->dpy = drw->dpy;

	return font;
}
#endif // BAR_PANGO_PATCH

static void
xfont_free(Fnt *font)
{
	if (!font)
		return;
	#if BAR_PANGO_PATCH
	if (font->layout)
		g_object_unref(font->layout);
	#else
	if (font->pattern)
		FcPatternDestroy(font->pattern);
	XftFontClose(font->dpy, font->xfont);
	#endif // BAR_PANGO_PATCH
	free(font);
}

#if BAR_PANGO_PATCH
Fnt*
drw_font_create(Drw* drw, const char font[])
{
	Fnt *fnt = NULL;

	if (!drw || !font)
		return NULL;

	fnt = xfont_create(drw, font);

	return (drw->fonts = fnt);
}
#else
Fnt*
drw_fontset_create(Drw* drw, const char *fonts[], size_t fontcount)
{
	Fnt *cur, *ret = NULL;
	size_t i;

	if (!drw || !fonts)
		return NULL;

	for (i = 1; i <= fontcount; i++) {
		if ((cur = xfont_create(drw, fonts[fontcount - i], NULL))) {
			cur->next = ret;
			ret = cur;
		}
	}
	return (drw->fonts = ret);
}
#endif // BAR_PANGO_PATCH

void
drw_fontset_free(Fnt *font)
{
	if (font) {
		#if !BAR_PANGO_PATCH
		drw_fontset_free(font->next);
		#endif // BAR_PANGO_PATCH
		xfont_free(font);
	}
}

void
drw_clr_create(
	Drw *drw,
	Clr *dest,
	const char *clrname
	#if BAR_ALPHA_PATCH
	, unsigned int alpha
	#endif // BAR_ALPHA_PATCH
) {
	if (!drw || !dest || !clrname)
		return;

	#if BAR_ALPHA_PATCH
	if (!XftColorAllocName(drw->dpy, drw->visual, drw->cmap,
	                       clrname, dest))
		#if DO_NOT_DIE_ON_COLOR_ALLOCATION_FAILURE_PATCH
		fprintf(stderr, "warning, cannot allocate color '%s'", clrname);
		#else
		die("error, cannot allocate color '%s'", clrname);
		#endif // DO_NOT_DIE_ON_COLOR_ALLOCATION_FAILURE_PATCH

	dest->pixel = (dest->pixel & 0x00ffffffU) | (alpha << 24);
	#else
	if (!XftColorAllocName(drw->dpy, DefaultVisual(drw->dpy, drw->screen),
	                       DefaultColormap(drw->dpy, drw->screen),
	                       clrname, dest))
		#if DO_NOT_DIE_ON_COLOR_ALLOCATION_FAILURE_PATCH
		fprintf(stderr, "warning, cannot allocate color '%s'", clrname);
		#else
		die("error, cannot allocate color '%s'", clrname);
		#endif // DO_NOT_DIE_ON_COLOR_ALLOCATION_FAILURE_PATCH

	#if NO_TRANSPARENT_BORDERS_PATCH
	dest->pixel |= 0xff << 24;
	#endif // NO_TRANSPARENT_BORDERS_PATCH
	#endif // BAR_ALPHA_PATCH
}

/* Wrapper to create color schemes. The caller has to call free(3) on the
 * returned color scheme when done using it. */
Clr *
drw_scm_create(
	Drw *drw,
	char *clrnames[],
	#if BAR_ALPHA_PATCH
	const unsigned int alphas[],
	#endif // BAR_ALPHA_PATCH
	size_t clrcount
) {
	size_t i;
	Clr *ret;

	/* need at least two colors for a scheme */
	if (!drw || !clrnames || clrcount < 2 || !(ret = ecalloc(clrcount, sizeof(XftColor))))
		return NULL;

	for (i = 0; i < clrcount; i++)
		#if BAR_ALPHA_PATCH
		drw_clr_create(drw, &ret[i], clrnames[i], alphas[i]);
		#else
		drw_clr_create(drw, &ret[i], clrnames[i]);
		#endif // BAR_ALPHA_PATCH
	return ret;
}

#if !BAR_PANGO_PATCH
void
drw_setfontset(Drw *drw, Fnt *set)
{
	if (drw)
		drw->fonts = set;
}
#endif // BAR_PANGO_PATCH

void
drw_setscheme(Drw *drw, Clr *scm)
{
	if (drw)
		drw->scheme = scm;
}

#if BAR_POWERLINE_TAGS_PATCH || BAR_POWERLINE_STATUS_PATCH
void
drw_settrans(Drw *drw, Clr *psc, Clr *nsc)
{
	if (drw) {
		transcheme[0] = psc[ColBg]; transcheme[1] = nsc[ColBg]; transcheme[2] = psc[ColBorder];
		drw->scheme = transcheme;
	}
}
#endif // BAR_POWERLINE_TAGS_PATCH | BAR_POWERLINE_STATUS_PATCH

void
drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert)
{
	if (!drw || !drw->scheme)
		return;
	XSetForeground(drw->dpy, drw->gc, invert ? drw->scheme[ColBg].pixel : drw->scheme[ColFg].pixel);
	if (filled)
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
	else
		XDrawRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w - 1, h - 1);
}

#if BIDI_PATCH
int
_drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup)
#else
int
drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup)
#endif // BIDI_PATCH
{
#if BAR_PANGO_PATCH
	char buf[1024];
	int i, ty, th;
	unsigned int ew, eh;
	XftDraw *d = NULL;
	size_t len;
	int render = x || y || w || h;

	if (!drw || (render && !drw->scheme) || !text || !drw->fonts)
		return 0;

	if (!render) {
		w = ~w;
	} else {
		XSetForeground(drw->dpy, drw->gc, drw->scheme[invert ? ColFg : ColBg].pixel);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
		#if BAR_ALPHA_PATCH
		d = XftDrawCreate(drw->dpy, drw->drawable, drw->visual, drw->cmap);
		#else
		d = XftDrawCreate(drw->dpy, drw->drawable,
		                  DefaultVisual(drw->dpy, drw->screen),
		                  DefaultColormap(drw->dpy, drw->screen));
		#endif // BAR_ALPHA_PATCH
		x += lpad;
		w -= lpad;
	}

	len = strlen(text);

	if (len) {
		drw_font_getexts(drw->fonts, text, len, &ew, &eh, markup);
		th = eh;
		/* shorten text if necessary */
		for (len = MIN(len, sizeof(buf) - 1); len && ew > w; len--) {
			drw_font_getexts(drw->fonts, text, len, &ew, &eh, markup);
			if (eh > th)
				th = eh;
		}

		if (len) {
			memcpy(buf, text, len);
			buf[len] = '\0';
			if (len < strlen(text))
				for (i = len; i && i > len - 3; buf[--i] = '.')
					; /* NOP */

			if (render) {
				ty = y + (h - th) / 2;
				if (markup)
					pango_layout_set_markup(drw->fonts->layout, buf, len);
				else
					pango_layout_set_text(drw->fonts->layout, buf, len);
				pango_xft_render_layout(d, &drw->scheme[invert ? ColBg : ColFg],
					drw->fonts->layout, x * PANGO_SCALE, ty * PANGO_SCALE);
				if (markup) /* clear markup attributes */
					pango_layout_set_attributes(drw->fonts->layout, NULL);
			}
			x += ew;
			w -= ew;
		}
	}
	if (d)
		XftDrawDestroy(d);

	return x + (render ? w : 0);
#else
	char buf[1024];
	int ty;
	unsigned int ew;
	XftDraw *d = NULL;
	Fnt *usedfont, *curfont, *nextfont;
	size_t i, len;
	int utf8strlen, utf8charlen, render = x || y || w || h;
	long utf8codepoint = 0;
	const char *utf8str;
	FcCharSet *fccharset;
	FcPattern *fcpattern;
	FcPattern *match;
	XftResult result;
	int charexists = 0;

	if (!drw || (render && !drw->scheme) || !text || !drw->fonts)
		return 0;

	if (!render) {
		w = ~w;
	} else {
		XSetForeground(drw->dpy, drw->gc, drw->scheme[invert ? ColFg : ColBg].pixel);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
		#if BAR_ALPHA_PATCH
		d = XftDrawCreate(drw->dpy, drw->drawable, drw->visual, drw->cmap);
		#else
		d = XftDrawCreate(drw->dpy, drw->drawable,
		                  DefaultVisual(drw->dpy, drw->screen),
		                  DefaultColormap(drw->dpy, drw->screen));
		#endif // BAR_ALPHA_PATCH
		x += lpad;
		w -= lpad;
	}

	usedfont = drw->fonts;
	while (1) {
		utf8strlen = 0;
		utf8str = text;
		nextfont = NULL;
		while (*text) {
			utf8charlen = utf8decode(text, &utf8codepoint, UTF_SIZ);
			for (curfont = drw->fonts; curfont; curfont = curfont->next) {
				charexists = charexists || XftCharExists(drw->dpy, curfont->xfont, utf8codepoint);
				if (charexists) {
					if (curfont == usedfont) {
						utf8strlen += utf8charlen;
						text += utf8charlen;
					} else {
						nextfont = curfont;
					}
					break;
				}
			}

			if (!charexists || nextfont)
				break;
			else
				charexists = 0;
		}

		if (utf8strlen) {
			drw_font_getexts(usedfont, utf8str, utf8strlen, &ew, NULL);
			/* shorten text if necessary */
			for (len = MIN(utf8strlen, sizeof(buf) - 1); len && ew > w; drw_font_getexts(usedfont, utf8str, len, &ew, NULL))
				len--;

			if (len) {
				memcpy(buf, utf8str, len);
				buf[len] = '\0';
				if (len < utf8strlen)
					for (i = len; i && i > len - 3; buf[--i] = '.')
						; /* NOP */

				if (render) {
					ty = y + (h - usedfont->h) / 2 + usedfont->xfont->ascent;
					XftDrawStringUtf8(d, &drw->scheme[invert ? ColBg : ColFg],
					                  usedfont->xfont, x, ty, (XftChar8 *)buf, len);
				}
				x += ew;
				w -= ew;
			}
		}

		if (!*text) {
			break;
		} else if (nextfont) {
			charexists = 0;
			usedfont = nextfont;
		} else {
			/* Regardless of whether or not a fallback font is found, the
			 * character must be drawn. */
			charexists = 1;

			fccharset = FcCharSetCreate();
			FcCharSetAddChar(fccharset, utf8codepoint);

			if (!drw->fonts->pattern) {
				/* Refer to the comment in xfont_create for more information. */
				die("the first font in the cache must be loaded from a font string.");
			}

			fcpattern = FcPatternDuplicate(drw->fonts->pattern);
			FcPatternAddCharSet(fcpattern, FC_CHARSET, fccharset);
			FcPatternAddBool(fcpattern, FC_SCALABLE, FcTrue);
			#if BAR_NO_COLOR_EMOJI_PATCH
			FcPatternAddBool(fcpattern, FC_COLOR, FcFalse);
			#endif // BAR_NO_COLOR_EMOJI_PATCH

			FcConfigSubstitute(NULL, fcpattern, FcMatchPattern);
			FcDefaultSubstitute(fcpattern);
			match = XftFontMatch(drw->dpy, drw->screen, fcpattern, &result);

			FcCharSetDestroy(fccharset);
			FcPatternDestroy(fcpattern);

			if (match) {
				usedfont = xfont_create(drw, NULL, match);
				if (usedfont && XftCharExists(drw->dpy, usedfont->xfont, utf8codepoint)) {
					for (curfont = drw->fonts; curfont->next; curfont = curfont->next)
						; /* NOP */
					curfont->next = usedfont;
				} else {
					xfont_free(usedfont);
					usedfont = drw->fonts;
				}
			}
		}
	}
	if (d)
		XftDrawDestroy(d);

	return x + (render ? w : 0);
#endif // BAR_PANGO_PATCH
}

#if BIDI_PATCH
int
drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup)
{
	apply_fribidi(text);
	return _drw_text(drw, x, y, w, h, lpad, fribidi_text, invert, markup);
}
#endif // BIDI_PATCH

#if BAR_POWERLINE_TAGS_PATCH || BAR_POWERLINE_STATUS_PATCH
void
drw_arrow(Drw *drw, int x, int y, unsigned int w, unsigned int h, int direction, int slash)
{
	if (!drw || !drw->scheme)
		return;

	/* direction=1 draws right arrow */
	x = direction ? x : x + w;
	w = direction ? w : -w;
	/* slash=1 draws slash instead of arrow */
	unsigned int hh = slash ? (direction ? 0 : h) : h/2;

	XPoint points[] = {
		{x    , y      },
		{x + w, y + hh },
		{x    , y + h  },
	};

	XPoint bg[] = {
		{x    , y    },
		{x + w, y    },
		{x + w, y + h},
		{x    , y + h},
	};

	XSetForeground(drw->dpy, drw->gc, drw->scheme[ColBg].pixel);
	XFillPolygon(drw->dpy, drw->drawable, drw->gc, bg, 4, Convex, CoordModeOrigin);
	XSetForeground(drw->dpy, drw->gc, drw->scheme[ColFg].pixel);
	XFillPolygon(drw->dpy, drw->drawable, drw->gc, points, 3, Nonconvex, CoordModeOrigin);
}
#endif // BAR_POWERLINE_TAGS_PATCH | BAR_POWERLINE_STATUS_PATCH

void
drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h)
{
	if (!drw)
		return;

	XCopyArea(drw->dpy, drw->drawable, win, drw->gc, x, y, w, h, x, y);
	XSync(drw->dpy, False);
}

unsigned int
drw_fontset_getwidth(Drw *drw, const char *text, Bool markup)
{
	if (!drw || !drw->fonts || !text)
		return 0;
	return drw_text(drw, 0, 0, 0, 0, 0, text, 0, markup);
}

#if BAR_PANGO_PATCH
void
drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup)
{
	if (!font || !text)
		return;

	PangoRectangle r;
	if (markup)
		pango_layout_set_markup(font->layout, text, len);
	else
		pango_layout_set_text(font->layout, text, len);
	pango_layout_get_extents(font->layout, 0, &r);
	if (markup) /* clear markup attributes */
		pango_layout_set_attributes(font->layout, NULL);
	if (w)
		*w = r.width / PANGO_SCALE;
	if (h)
		*h = r.height / PANGO_SCALE;
}
#else
void
drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h)
{
	XGlyphInfo ext;

	if (!font || !text)
		return;

	XftTextExtentsUtf8(font->dpy, font->xfont, (XftChar8 *)text, len, &ext);
	if (w)
		*w = ext.xOff;
	if (h)
		*h = font->h;
}
#endif // BAR_PANGO_PATCH

Cur *
drw_cur_create(Drw *drw, int shape)
{
	Cur *cur;

	if (!drw || !(cur = ecalloc(1, sizeof(Cur))))
		return NULL;

	cur->cursor = XCreateFontCursor(drw->dpy, shape);

	return cur;
}

void
drw_cur_free(Drw *drw, Cur *cursor)
{
	if (!cursor)
		return;

	XFreeCursor(drw->dpy, cursor->cursor);
	free(cursor);
}

