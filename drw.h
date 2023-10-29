/* See LICENSE file for copyright and license details. */

#if BAR_PANGO_PATCH
#include <pango/pango.h>
#include <pango/pangoxft.h>
#endif // BAR_PANGO_PATCH

typedef struct {
	Cursor cursor;
} Cur;

typedef struct Fnt {
	Display *dpy;
	unsigned int h;
	#if BAR_PANGO_PATCH
	PangoLayout *layout;
	#else
	XftFont *xfont;
	FcPattern *pattern;
	struct Fnt *next;
	#endif // BAR_PANGO_PATCH
} Fnt;

enum { ColFg, ColBg, ColBorder, ColFloat, ColCount }; /* Clr scheme index */
typedef XftColor Clr;

typedef struct {
	unsigned int w, h;
	Display *dpy;
	int screen;
	Window root;
	#if BAR_ALPHA_PATCH
	Visual *visual;
	unsigned int depth;
	Colormap cmap;
	#endif // BAR_ALPHA_PATCH
	Drawable drawable;
	#if BAR_WINICON_PATCH
	Picture picture;
	#endif // BAR_WINICON_PATCH
	GC gc;
	Clr *scheme;
	Fnt *fonts;
} Drw;

/* Drawable abstraction */
#if BAR_ALPHA_PATCH
Drw *drw_create(Display *dpy, int screen, Window win, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap);
#else
Drw *drw_create(Display *dpy, int screen, Window win, unsigned int w, unsigned int h);
#endif // BAR_ALPHA_PATCH
void drw_resize(Drw *drw, unsigned int w, unsigned int h);
void drw_free(Drw *drw);

/* Fnt abstraction */
#if BAR_PANGO_PATCH
Fnt *drw_font_create(Drw* drw, const char font[]);
void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h, Bool markup);
#else
Fnt *drw_fontset_create(Drw* drw, const char *fonts[], size_t fontcount);
void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h);
#endif // BAR_PANGO_PATCH
void drw_fontset_free(Fnt* set);
unsigned int drw_fontset_getwidth(Drw *drw, const char *text, Bool markup);

/* Colorscheme abstraction */
void drw_clr_create(
	Drw *drw,
	Clr *dest,
	const char *clrname
	#if BAR_ALPHA_PATCH
	, unsigned int alpha
	#endif // BAR_ALPHA_PATCH
);
Clr *drw_scm_create(
	Drw *drw,
	char *clrnames[],
	#if BAR_ALPHA_PATCH
	const unsigned int alphas[],
	#endif // BAR_ALPHA_PATCH
	size_t clrcount
);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *drw, int shape);
void drw_cur_free(Drw *drw, Cur *cursor);

/* Drawing context manipulation */
#if !BAR_PANGO_PATCH
void drw_setfontset(Drw *drw, Fnt *set);
#endif // BAR_PANGO_PATCH
void drw_setscheme(Drw *drw, Clr *scm);
#if BAR_POWERLINE_TAGS_PATCH || BAR_POWERLINE_STATUS_PATCH
void drw_settrans(Drw *drw, Clr *psc, Clr *nsc);
#endif // BAR_POWERLINE_TAGS_PATCH | BAR_POWERLINE_STATUS_PATCH

/* Drawing functions */
void drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert);
int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert, Bool markup);
#if BAR_POWERLINE_TAGS_PATCH || BAR_POWERLINE_STATUS_PATCH
void drw_arrow(Drw *drw, int x, int y, unsigned int w, unsigned int h, int direction, int slash);
#endif // BAR_POWERLINE_TAGS_PATCH | BAR_POWERLINE_STATUS_PATCH

/* Map functions */
void drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h);

