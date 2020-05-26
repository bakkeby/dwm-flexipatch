/* See LICENSE file for copyright and license details. */

typedef struct {
	Cursor cursor;
} Cur;

typedef struct Fnt {
	Display *dpy;
	unsigned int h;
	XftFont *xfont;
	FcPattern *pattern;
	struct Fnt *next;
} Fnt;

#if FLOAT_BORDER_COLOR_PATCH
enum { ColFg, ColBg, ColBorder, ColFloat, ColCount }; /* Clr scheme index */
#else
enum { ColFg, ColBg, ColBorder, ColCount }; /* Clr scheme index */
#endif // FLOAT_BORDER_COLOR_PATCH
typedef XftColor Clr;

typedef struct {
	unsigned int w, h;
	Display *dpy;
	int screen;
	Window root;
	#if ALPHA_PATCH
	Visual *visual;
	unsigned int depth;
	Colormap cmap;
	#endif // ALPHA_PATCH
	Drawable drawable;
	GC gc;
	Clr *scheme;
	Fnt *fonts;
} Drw;

/* Drawable abstraction */
#if ALPHA_PATCH
Drw *drw_create(Display *dpy, int screen, Window win, unsigned int w, unsigned int h, Visual *visual, unsigned int depth, Colormap cmap);
#else
Drw *drw_create(Display *dpy, int screen, Window win, unsigned int w, unsigned int h);
#endif // ALPHA_PATCH
void drw_resize(Drw *drw, unsigned int w, unsigned int h);
void drw_free(Drw *drw);

/* Fnt abstraction */
Fnt *drw_fontset_create(Drw* drw, const char *fonts[], size_t fontcount);
void drw_fontset_free(Fnt* set);
unsigned int drw_fontset_getwidth(Drw *drw, const char *text);
void drw_font_getexts(Fnt *font, const char *text, unsigned int len, unsigned int *w, unsigned int *h);

/* Colorscheme abstraction */
void drw_clr_create(
	Drw *drw,
	Clr *dest,
	#if VTCOLORS_PATCH
	const char clrname[]
	#else
	const char *clrname
	#endif // VTCOLORS_PATCH
	#if ALPHA_PATCH
	, unsigned int alpha
	#endif // ALPHA_PATCH
);
Clr *drw_scm_create(
	Drw *drw,
	#if VTCOLORS_PATCH
	char clrnames[][8],
	#elif XRDB_PATCH
	char *clrnames[],
	#else
	const char *clrnames[],
	#endif // VTCOLORS_PATCH / XRDB_PATCH
	#if ALPHA_PATCH
	const unsigned int alphas[],
	#endif // ALPHA_PATCH
	size_t clrcount
);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *drw, int shape);
void drw_cur_free(Drw *drw, Cur *cursor);

/* Drawing context manipulation */
void drw_setfontset(Drw *drw, Fnt *set);
void drw_setscheme(Drw *drw, Clr *scm);

/* Drawing functions */
void drw_rect(Drw *drw, int x, int y, unsigned int w, unsigned int h, int filled, int invert);
int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert);

/* Map functions */
void drw_map(Drw *drw, Window win, int x, int y, unsigned int w, unsigned int h);
