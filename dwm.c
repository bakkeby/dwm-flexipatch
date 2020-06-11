/* See LICENSE file for copyright and license details.
 *
 * dynamic window manager is designed like any other X client as well. It is
 * driven through handling X events. In contrast to other X clients, a window
 * manager selects for SubstructureRedirectMask on the root window, to receive
 * events about window (dis-)appearance. Only one X connection at a time is
 * allowed to select for this event mask.
 *
 * The event handlers of dwm are organized in an array which is accessed
 * whenever a new event has been fetched. This allows event dispatching
 * in O(1) time.
 *
 * Each child of the root window is called a client, except windows which have
 * set the override_redirect flag. Clients are organized in a linked client
 * list on each monitor, the focus history is remembered through a stack list
 * on each monitor. Each client contains a bit array to indicate the tags of a
 * client.
 *
 * Keys and tagging rules are organized as arrays and defined in config.h.
 *
 * To understand everything else, start reading main().
 */
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */
#include <X11/Xft/Xft.h>

#include "patches.h"
#include "drw.h"
#include "util.h"

#if PANGO_PATCH
#include <pango/pango.h>
#endif // PANGO_PATCH

#if SPAWNCMD_PATCH
#include <assert.h>
#include <libgen.h>
#include <sys/stat.h>
#define SPAWN_CWD_DELIM " []{}()<>\"':"
#endif // SPAWNCMD_PATCH

/* macros */
#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->wx+(m)->ww) - MAX((x),(m)->wx)) \
                               * MAX(0, MIN((y)+(h),(m)->wy+(m)->wh) - MAX((y),(m)->wy)))
#if ATTACHASIDE_PATCH && STICKY_PATCH
#define ISVISIBLEONTAG(C, T)    ((C->tags & T) || C->issticky)
#define ISVISIBLE(C)            ISVISIBLEONTAG(C, C->mon->tagset[C->mon->seltags])
#elif ATTACHASIDE_PATCH
#define ISVISIBLEONTAG(C, T)    ((C->tags & T))
#define ISVISIBLE(C)            ISVISIBLEONTAG(C, C->mon->tagset[C->mon->seltags])
#elif STICKY_PATCH
#define ISVISIBLE(C)            ((C->tags & C->mon->tagset[C->mon->seltags]) || C->issticky)
#else
#define ISVISIBLE(C)            ((C->tags & C->mon->tagset[C->mon->seltags]))
#endif // ATTACHASIDE_PATCH
#if AWESOMEBAR_PATCH
#define HIDDEN(C)               ((getstate(C->win) == IconicState))
#endif // AWESOMEBAR_PATCH
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MOUSEMASK               (BUTTONMASK|PointerMotionMask)
#define WIDTH(X)                ((X)->w + 2 * (X)->bw)
#define HEIGHT(X)               ((X)->h + 2 * (X)->bw)
#if SCRATCHPADS_PATCH
#define NUMTAGS                 (LENGTH(tags) + LENGTH(scratchpads))
#define TAGMASK                 ((1 << NUMTAGS) - 1)
#define SPTAG(i)                ((1 << LENGTH(tags)) << (i))
#define SPTAGMASK               (((1 << LENGTH(scratchpads))-1) << LENGTH(tags))
#else
#define TAGMASK                 ((1 << LENGTH(tags)) - 1)
#endif // SCRATCHPADS_PATCH
#if PANGO_PATCH
#define TEXTW(X)                (drw_font_getwidth(drw, (X), False) + lrpad)
#define TEXTWM(X)               (drw_font_getwidth(drw, (X), True) + lrpad)
#else
#define TEXTW(X)                (drw_fontset_getwidth(drw, (X)) + lrpad)
#endif // PANGO_PATCH

/* enums */
enum { CurNormal, CurResize, CurMove, CurLast }; /* cursor */

enum {
	SchemeNorm
	,SchemeSel
	#if STATUSCOLORS_PATCH
	,SchemeWarn
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	,SchemeUrg
	#endif // URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	,SchemeHid
	#endif // AWESOMEBAR_PATCH
	#if VTCOLORS_PATCH
	,SchemeTagsNorm
	,SchemeTagsSel
	,SchemeTitleNorm
	,SchemeTitleSel
	,SchemeStatus
	#elif TITLECOLOR_PATCH
	,SchemeTitle
	#endif // VTCOLORS_PATCH
}; /* color schemes */

enum {
	NetSupported, NetWMName, NetWMState, NetWMCheck,
	NetWMFullscreen, NetActiveWindow, NetWMWindowType,
	#if SYSTRAY_PATCH
	NetSystemTray, NetSystemTrayOP, NetSystemTrayOrientation,
	NetSystemTrayVisual, NetWMWindowTypeDock, NetSystemTrayOrientationHorz,
	#endif // SYSTRAY_PATCH
	#if EWMHTAGS_PATCH
	NetDesktopNames, NetDesktopViewport, NetNumberOfDesktops, NetCurrentDesktop,
	#endif // EWMHTAGS_PATCH
	#if EWMH_WINDOWS_FLOAT_PATCH
	NetWMModal, NetWMWindowTypeUtility, NetWMWindowTypeToolbar, NetWMWindowTypeSplash,
	#endif // EWMH_WINDOWS_FLOAT_PATCH
	NetWMWindowTypeDialog, NetClientList, NetLast
}; /* EWMH atoms */

#if WINDOWROLERULE_PATCH
enum { WMProtocols, WMDelete, WMState, WMTakeFocus, WMWindowRole, WMLast }; /* default atoms */
#else
enum { WMProtocols, WMDelete, WMState, WMTakeFocus, WMLast }; /* default atoms */
#endif // WINDOWROLERULE_PATCH
enum { ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
       ClkClientWin, ClkRootWin, ClkLast }; /* clicks */

typedef union {
	int i;
	unsigned int ui;
	float f;
	const void *v;
} Arg;

typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg *arg);
	const Arg arg;
} Button;

typedef struct Monitor Monitor;
typedef struct Client Client;
struct Client {
	char name[256];
	float mina, maxa;
	#if CFACTS_PATCH
	float cfact;
	#endif // CFACTS_PATCH
	int x, y, w, h;
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	int sfx, sfy, sfw, sfh; /* stored float geometry, used on mode revert */
	#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
	int oldx, oldy, oldw, oldh;
	int basew, baseh, incw, inch, maxw, maxh, minw, minh;
	int bw, oldbw;
	unsigned int tags;
	#if SWITCHTAG_PATCH
	unsigned int switchtag;
	#endif // SWITCHTAG_PATCH
	int isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	int fakefullscreen;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#if EXRESIZE_PATCH
	unsigned char expandmask;
	int expandx1, expandy1, expandx2, expandy2;
	#if !MAXIMIZE_PATCH
	int wasfloating;
	#endif // MAXIMIZE_PATCH
	#endif // EXRESIZE_PATCH
	#if MAXIMIZE_PATCH
	int ismax, wasfloating;
	#endif // MAXIMIZE_PATCH
	#if AUTORESIZE_PATCH
	int needresize;
	#endif // AUTORESIZE_PATCH
	#if CENTER_PATCH
	int iscentered;
	#endif // CENTER_PATCH
	#if ISPERMANENT_PATCH
	int ispermanent;
	#endif // ISPERMANENT_PATCH
	#if SWALLOW_PATCH
	int isterminal, noswallow;
	pid_t pid;
	#endif // SWALLOW_PATCH
	#if STICKY_PATCH
	int issticky;
	#endif // STICKY_PATCH
	Client *next;
	Client *snext;
	#if SWALLOW_PATCH
	Client *swallowing;
	#endif // SWALLOW_PATCH
	Monitor *mon;
	Window win;
};

typedef struct {
	unsigned int mod;
	KeySym keysym;
	void (*func)(const Arg *);
	const Arg arg;
} Key;

#if FLEXTILE_DELUXE_LAYOUT
typedef struct {
	int nmaster;
	int nstack;
	int layout;
	int masteraxis; // master stack area
	int stack1axis; // primary stack area
	int stack2axis; // secondary stack area, e.g. centered master
	void (*symbolfunc)(Monitor *, unsigned int);
} LayoutPreset;
#endif // FLEXTILE_DELUXE_LAYOUT

typedef struct {
	const char *symbol;
	void (*arrange)(Monitor *);
	#if FLEXTILE_DELUXE_LAYOUT
	LayoutPreset preset;
	#endif // FLEXTILE_DELUXE_LAYOUT
} Layout;

#if PERTAG_PATCH
typedef struct Pertag Pertag;
#endif // PERTAG_PATCH
struct Monitor {
	char ltsymbol[16];
	float mfact;
	#if FLEXTILE_DELUXE_LAYOUT
	int ltaxis[4];
	int nstack;
	#endif // FLEXTILE_DELUXE_LAYOUT
	int nmaster;
	int num;
	int by;               /* bar geometry */
	int tw;               /* bar text width */
	#if EXTRABAR_PATCH
	int eby;	          /* extra bar geometry */
	#endif // EXTRABAR_PATCH
	#if AWESOMEBAR_PATCH
	int btw;              /* width of tasks portion of bar */
	int bt;               /* number of tasks */
	#endif // AWESOMEBAR_PATCH
	int mx, my, mw, mh;   /* screen size */
	int wx, wy, ww, wh;   /* window area  */
	#if VANITYGAPS_PATCH
	int gappih;           /* horizontal gap between windows */
	int gappiv;           /* vertical gap between windows */
	int gappoh;           /* horizontal outer gaps */
	int gappov;           /* vertical outer gaps */
	#endif // VANITYGAPS_PATCH
	#if SETBORDERPX_PATCH
	unsigned int borderpx;
	#endif // SETBORDERPX_PATCH
	unsigned int seltags;
	unsigned int sellt;
	unsigned int tagset[2];
	int showbar;
	int topbar;
	Client *clients;
	Client *sel;
	Client *stack;
	Monitor *next;
	Window barwin;
	#if EXTRABAR_PATCH
	Window extrabarwin;
	#endif // EXTRABAR_PATCH
	const Layout *lt[2];
	#if ALTERNATIVE_TAGS_PATCH
	unsigned int alttag;
	#endif // ALTERNATIVE_TAGS_PATCH
	#if PERTAG_PATCH
	Pertag *pertag;
	#endif // PERTAG_PATCH
};

typedef struct {
	const char *class;
	#if WINDOWROLERULE_PATCH
	const char *role;
	#endif // WINDOWROLERULE_PATCH
	const char *instance;
	const char *title;
	unsigned int tags;
	#if SWITCHTAG_PATCH
	int switchtag;
	#endif // SWITCHTAG_PATCH
	#if CENTER_PATCH
	int iscentered;
	#endif // CENTER_PATCH
	int isfloating;
	#if ISPERMANENT_PATCH
	int ispermanent;
	#endif // ISPERMANENT_PATCH
	#if SWALLOW_PATCH
	int isterminal;
	int noswallow;
	#endif // SWALLOW_PATCH
	int monitor;
} Rule;

#if MONITOR_RULES_PATCH
typedef struct {
	int monitor;
	#if PERTAG_PATCH
	int tag;
	#endif // PERTAG_PATCH
	int layout;
	float mfact;
	int nmaster;
	int showbar;
	int topbar;
} MonitorRule;
#endif // MONITOR_RULES_PATCH

/* function declarations */
static void applyrules(Client *c);
static int applysizehints(Client *c, int *x, int *y, int *w, int *h, int interact);
static void arrange(Monitor *m);
static void arrangemon(Monitor *m);
static void attach(Client *c);
static void attachstack(Client *c);
static void buttonpress(XEvent *e);
static void checkotherwm(void);
static void cleanup(void);
static void cleanupmon(Monitor *mon);
static void clientmessage(XEvent *e);
static void configure(Client *c);
static void configurenotify(XEvent *e);
static void configurerequest(XEvent *e);
static Monitor *createmon(void);
static void destroynotify(XEvent *e);
static void detach(Client *c);
static void detachstack(Client *c);
static Monitor *dirtomon(int dir);
static void drawbar(Monitor *m);
static void drawbars(void);
#if !FOCUSONCLICK_PATCH
static void enternotify(XEvent *e);
#endif // FOCUSONCLICK_PATCH
static void expose(XEvent *e);
static void focus(Client *c);
static void focusin(XEvent *e);
static void focusmon(const Arg *arg);
#if !STACKER_PATCH
static void focusstack(const Arg *arg);
#endif // STACKER_PATCH
static Atom getatomprop(Client *c, Atom prop);
static int getrootptr(int *x, int *y);
static long getstate(Window w);
static int gettextprop(Window w, Atom atom, char *text, unsigned int size);
static void grabbuttons(Client *c, int focused);
#if KEYMODES_PATCH
static void grabdefkeys(void);
#else
static void grabkeys(void);
#endif // KEYMODES_PATCH
static void incnmaster(const Arg *arg);
#if KEYMODES_PATCH
static void keydefpress(XEvent *e);
#else
static void keypress(XEvent *e);
#endif // KEYMODES_PATCH
static void killclient(const Arg *arg);
static void manage(Window w, XWindowAttributes *wa);
static void mappingnotify(XEvent *e);
static void maprequest(XEvent *e);
#if !FOCUSONCLICK_PATCH
static void motionnotify(XEvent *e);
#endif // FOCUSONCLICK_PATCH
static void movemouse(const Arg *arg);
static Client *nexttiled(Client *c);
#if !ZOOMSWAP_PATCH || TAGINTOSTACK_ALLMASTER_PATCH || TAGINTOSTACK_ONEMASTER_PATCH
static void pop(Client *);
#endif // !ZOOMSWAP_PATCH / TAGINTOSTACK_ALLMASTER_PATCH / TAGINTOSTACK_ONEMASTER_PATCH
static void propertynotify(XEvent *e);
static void quit(const Arg *arg);
static Monitor *recttomon(int x, int y, int w, int h);
static void resize(Client *c, int x, int y, int w, int h, int interact);
static void resizeclient(Client *c, int x, int y, int w, int h);
static void resizemouse(const Arg *arg);
static void restack(Monitor *m);
static void run(void);
static void scan(void);
#if SYSTRAY_PATCH
static int sendevent(Window w, Atom proto, int m, long d0, long d1, long d2, long d3, long d4);
#else
static int sendevent(Client *c, Atom proto);
#endif // SYSTRAY_PATCH
static void sendmon(Client *c, Monitor *m);
static void setclientstate(Client *c, long state);
static void setfocus(Client *c);
static void setfullscreen(Client *c, int fullscreen);
static void setlayout(const Arg *arg);
static void setmfact(const Arg *arg);
static void setup(void);
static void seturgent(Client *c, int urg);
static void showhide(Client *c);
static void sigchld(int unused);
static void spawn(const Arg *arg);
static void tag(const Arg *arg);
static void tagmon(const Arg *arg);
static void togglebar(const Arg *arg);
static void togglefloating(const Arg *arg);
static void toggletag(const Arg *arg);
static void toggleview(const Arg *arg);
static void unfocus(Client *c, int setfocus);
static void unmanage(Client *c, int destroyed);
static void unmapnotify(XEvent *e);
static void updatebarpos(Monitor *m);
static void updatebars(void);
static void updateclientlist(void);
static int updategeom(void);
static void updatenumlockmask(void);
static void updatesizehints(Client *c);
static void updatestatus(void);
static void updatetitle(Client *c);
static void updatewindowtype(Client *c);
static void updatewmhints(Client *c);
static void view(const Arg *arg);
static Client *wintoclient(Window w);
static Monitor *wintomon(Window w);
static int xerror(Display *dpy, XErrorEvent *ee);
static int xerrordummy(Display *dpy, XErrorEvent *ee);
static int xerrorstart(Display *dpy, XErrorEvent *ee);
static void zoom(const Arg *arg);

#include "patch/include.h"

/* variables */
static const char broken[] = "broken";
#if PANGO_PATCH || STATUS2D_PATCH && !STATUSCOLORS_PATCH
static char stext[1024];
#else
static char stext[256];
#endif // STATUS2D_PATCH
#if EXTRABAR_PATCH
#if STATUS2D_PATCH && !STATUSCOLORS_PATCH
static char estext[1024];
#else
static char estext[256];
#endif // STATUS2D_PATCH
#endif // EXTRABAR_PATCH
static int screen;
static int sw, sh;           /* X display screen geometry width, height */
static int bh, blw = 0;      /* bar geometry */
static int lrpad;            /* sum of left and right padding for text */
#if BARPADDING_PATCH
static int vp;               /* vertical padding for bar */
static int sp;               /* side padding for bar */
#endif // BARPADDING_PATCH
static int (*xerrorxlib)(Display *, XErrorEvent *);
static unsigned int numlockmask = 0;
static void (*handler[LASTEvent]) (XEvent *) = {
	[ButtonPress] = buttonpress,
	#if COMBO_PATCH || HOLDBAR_PATCH
	[ButtonRelease] = keyrelease,
	#endif // COMBO_PATCH / HOLDBAR_PATCH
	[ClientMessage] = clientmessage,
	[ConfigureRequest] = configurerequest,
	[ConfigureNotify] = configurenotify,
	[DestroyNotify] = destroynotify,
	#if !FOCUSONCLICK_PATCH
	[EnterNotify] = enternotify,
	#endif // FOCUSONCLICK_PATCH
	[Expose] = expose,
	[FocusIn] = focusin,
	[KeyPress] = keypress,
	#if COMBO_PATCH || HOLDBAR_PATCH
	[KeyRelease] = keyrelease,
	#endif // COMBO_PATCH / HOLDBAR_PATCH
	[MappingNotify] = mappingnotify,
	[MapRequest] = maprequest,
	#if !FOCUSONCLICK_PATCH
	[MotionNotify] = motionnotify,
	#endif // FOCUSONCLICK_PATCH
	[PropertyNotify] = propertynotify,
	#if SYSTRAY_PATCH
	[ResizeRequest] = resizerequest,
	#endif // SYSTRAY_PATCH
	[UnmapNotify] = unmapnotify
};
#if SYSTRAY_PATCH
static Atom wmatom[WMLast], netatom[NetLast], xatom[XLast];
#else
static Atom wmatom[WMLast], netatom[NetLast];
#endif // SYSTRAY_PATCH
static int running = 1;
static Cur *cursor[CurLast];
static Clr **scheme;
static Display *dpy;
static Drw *drw;
static Monitor *mons, *selmon;
#if STATICSTATUS_PATCH && !STATUSALLMONS_PATCH
static Monitor *statmon;
#endif // STATICSTATUS_PATCH
static Window root, wmcheckwin;

/* configuration, allows nested code to access above variables */
#include "config.h"

#include "patch/include.c"

/* compile-time check if all tags fit into an unsigned int bit array. */
#if SCRATCHPAD_ALT_1_PATCH
struct NumTags { char limitexceeded[LENGTH(tags) > 30 ? -1 : 1]; };
#else
struct NumTags { char limitexceeded[LENGTH(tags) > 31 ? -1 : 1]; };
#endif // SCRATCHPAD_ALT_1_PATCH

/* function implementations */
void
applyrules(Client *c)
{
	const char *class, *instance;
	#if WINDOWROLERULE_PATCH
	char role[64];
	#endif // WINDOWROLERULE_PATCH
	unsigned int i;
	const Rule *r;
	Monitor *m;
	XClassHint ch = { NULL, NULL };

	/* rule matching */
	c->isfloating = 0;
	c->tags = 0;
	XGetClassHint(dpy, c->win, &ch);
	class    = ch.res_class ? ch.res_class : broken;
	instance = ch.res_name  ? ch.res_name  : broken;
	#if WINDOWROLERULE_PATCH
	gettextprop(c->win, wmatom[WMWindowRole], role, sizeof(role));
	#endif // WINDOWROLERULE_PATCH

	for (i = 0; i < LENGTH(rules); i++) {
		r = &rules[i];
		if ((!r->title || strstr(c->name, r->title))
		&& (!r->class || strstr(class, r->class))
		#if WINDOWROLERULE_PATCH
		&& (!r->role || strstr(role, r->role))
		#endif // WINDOWROLERULE_PATCH
		&& (!r->instance || strstr(instance, r->instance)))
		{
			#if CENTER_PATCH
			c->iscentered = r->iscentered;
			#endif // CENTER_PATCH
			#if ISPERMANENT_PATCH
			c->ispermanent = r->ispermanent;
			#endif // ISPERMANENT_PATCH
			#if SWALLOW_PATCH
			c->isterminal = r->isterminal;
			c->noswallow = r->noswallow;
			#endif // SWALLOW_PATCH
			c->isfloating = r->isfloating;
			c->tags |= r->tags;
			#if SCRATCHPADS_PATCH && !SCRATCHPAD_KEEP_POSITION_AND_SIZE_PATCH
			if ((r->tags & SPTAGMASK) && r->isfloating) {
				c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
				c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
			}
			#endif // SCRATCHPADS_PATCH | SCRATCHPAD_KEEP_POSITION_AND_SIZE_PATCH
			for (m = mons; m && m->num != r->monitor; m = m->next);
			if (m)
				c->mon = m;

			#if SWITCHTAG_PATCH
			#if SWALLOW_PATCH
			if (r->switchtag && (c->noswallow || !termforwin(c)))
			#else
			if (r->switchtag)
			#endif // SWALLOW_PATCH
			{
				unsigned int newtagset;
				if (r->switchtag == 2)
					newtagset = c->mon->tagset[c->mon->seltags] ^ c->tags;
				else
					newtagset = c->tags;

				if (newtagset) {
					c->switchtag = selmon->tagset[selmon->seltags];
					c->mon->tagset[c->mon->seltags] = newtagset;
					if (r->switchtag == 1)
						#if PERTAG_PATCH
						pertagview(&((Arg) { .ui = newtagset }));
						#else
						view(&((Arg) { .ui = newtagset }));
						#endif // PERTAG_PATCH
					arrange(c->mon);
				}
			}
			#endif // SWITCHTAG_PATCH
		}
	}
	if (ch.res_class)
		XFree(ch.res_class);
	if (ch.res_name)
		XFree(ch.res_name);
	#if EMPTYVIEW_PATCH
	if (c->tags & TAGMASK)                    c->tags = c->tags & TAGMASK;
	#if SCRATCHPADS_PATCH
	else if (c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags] & ~SPTAGMASK;
	#elif SCRATCHPAD_ALT_1_PATCH
	else if (c->tags != SCRATCHPAD_MASK && c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags];
	#else
	else if (c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags];
	#endif // SCRATCHPADS_PATCH
	else                                     c->tags = 1;
	#elif SCRATCHPADS_PATCH
	c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : (c->mon->tagset[c->mon->seltags] & ~SPTAGMASK);
	#elif SCRATCHPAD_ALT_1_PATCH
	if (c->tags != SCRATCHPAD_MASK)
		c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[c->mon->seltags];
	#else
	c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[c->mon->seltags];
	#endif // EMPTYVIEW_PATCH
}

int
applysizehints(Client *c, int *x, int *y, int *w, int *h, int interact)
{
	int baseismin;
	Monitor *m = c->mon;

	/* set minimum possible */
	*w = MAX(1, *w);
	*h = MAX(1, *h);
	if (interact) {
		if (*x > sw)
			*x = sw - WIDTH(c);
		if (*y > sh)
			*y = sh - HEIGHT(c);
		if (*x + *w + 2 * c->bw < 0)
			*x = 0;
		if (*y + *h + 2 * c->bw < 0)
			*y = 0;
	} else {
		if (*x >= m->wx + m->ww)
			*x = m->wx + m->ww - WIDTH(c);
		if (*y >= m->wy + m->wh)
			*y = m->wy + m->wh - HEIGHT(c);
		if (*x + *w + 2 * c->bw <= m->wx)
			*x = m->wx;
		if (*y + *h + 2 * c->bw <= m->wy)
			*y = m->wy;
	}
	if (*h < bh)
		*h = bh;
	if (*w < bh)
		*w = bh;
	if (resizehints || c->isfloating || !c->mon->lt[c->mon->sellt]->arrange) {
		/* see last two sentences in ICCCM 4.1.2.3 */
		baseismin = c->basew == c->minw && c->baseh == c->minh;
		if (!baseismin) { /* temporarily remove base dimensions */
			*w -= c->basew;
			*h -= c->baseh;
		}
		/* adjust for aspect limits */
		if (c->mina > 0 && c->maxa > 0) {
			if (c->maxa < (float)*w / *h)
				*w = *h * c->maxa + 0.5;
			else if (c->mina < (float)*h / *w)
				*h = *w * c->mina + 0.5;
		}
		if (baseismin) { /* increment calculation requires this */
			*w -= c->basew;
			*h -= c->baseh;
		}
		/* adjust for increment value */
		if (c->incw)
			*w -= *w % c->incw;
		if (c->inch)
			*h -= *h % c->inch;
		/* restore base dimensions */
		*w = MAX(*w + c->basew, c->minw);
		*h = MAX(*h + c->baseh, c->minh);
		if (c->maxw)
			*w = MIN(*w, c->maxw);
		if (c->maxh)
			*h = MIN(*h, c->maxh);
	}
	return *x != c->x || *y != c->y || *w != c->w || *h != c->h;
}

void
arrange(Monitor *m)
{
	if (m)
		showhide(m->stack);
	else for (m = mons; m; m = m->next)
		showhide(m->stack);
	if (m) {
		arrangemon(m);
		restack(m);
	} else for (m = mons; m; m = m->next)
		arrangemon(m);
}

void
arrangemon(Monitor *m)
{
	strncpy(m->ltsymbol, m->lt[m->sellt]->symbol, sizeof m->ltsymbol);
	if (m->lt[m->sellt]->arrange)
		m->lt[m->sellt]->arrange(m);
	#if ROUNDED_CORNERS_PATCH
	Client *c;
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		drawroundedcorners(c);
	#endif // ROUNDED_CORNERS_PATCH
}

void
attach(Client *c)
{
	c->next = c->mon->clients;
	c->mon->clients = c;
}

void
attachstack(Client *c)
{
	c->snext = c->mon->stack;
	c->mon->stack = c;
}

void
buttonpress(XEvent *e)
{
	unsigned int i, x, click;
	int padding = 0;
	#if STATUSCMD_PATCH
	unsigned int xc;
	#endif // STATUSCMD_PATCH
	#if TAGGRID_PATCH
	unsigned int columns;
	#endif // TAGGRID_PATCH
	#if HIDEVACANTTAGS_PATCH
	unsigned int occ = 0;
	#endif // HIDEVACANTTAGS_PATCH
	Arg arg = {0};
	Client *c;
	Monitor *m;
	XButtonPressedEvent *ev = &e->xbutton;
	#if STATUSCMD_PATCH && !DWMBLOCKS_PATCH
	lastbutton = ev->button;
	#endif // STATUSCMD_PATCH | DWMBLOCKS_PATCH
	#if BARPADDING_PATCH
	padding -= sp * 2;
	#endif // BARPADDING_PATCH
	#if SYSTRAY_PATCH
	padding -= getsystraywidth();
	#endif // SYSTRAY_PATCH
	#if TAGGRID_PATCH
	columns = LENGTH(tags) / tagrows + ((LENGTH(tags) % tagrows > 0) ? 1 : 0);
	#endif // TAGGRID_PATCH
	click = ClkRootWin;
	/* focus monitor if necessary */
	if ((m = wintomon(ev->window)) && m != selmon
		#if FOCUSONCLICK_PATCH
		&& (focusonwheel || (ev->button != Button4 && ev->button != Button5))
		#endif // FOCUSONCLICK_PATCH
	) {
		unfocus(selmon->sel, 1);
		selmon = m;
		focus(NULL);
	}
	if (ev->window == selmon->barwin) {
		i = x = 0;
		#if LEFTLAYOUT_PATCH
		x += blw;
		if (ev->x < x) {
			click = ClkLtSymbol;
		} else {
		#endif // LEFTLAYOUT_PATCH
		#if HIDEVACANTTAGS_PATCH
		for (c = m->clients; c; c = c->next)
			occ |= c->tags == 255 ? 0 : c->tags;
		#endif // HIDEVACANTTAGS_PATCH
		#if TAGGRID_PATCH
		if (drawtagmask & DRAWCLASSICTAGS)
		#endif // TAGGRID_PATCH
		do {
			#if HIDEVACANTTAGS_PATCH
			/* do not reserve space for vacant tags */
			if (!(occ & 1 << i || m->tagset[m->seltags] & 1 << i))
				continue;
			#endif // HIDEVACANTTAGS_PATCH
			x += TEXTW(tags[i]);
		} while (ev->x >= x && ++i < LENGTH(tags));
		if (i < LENGTH(tags)
		#if TAGGRID_PATCH
			&& (drawtagmask & DRAWCLASSICTAGS)
		#endif
		) {
			click = ClkTagBar;
			arg.ui = 1 << i;
		#if TAGGRID_PATCH
		} else if (ev->x < x + columns * bh / tagrows && (drawtagmask & DRAWTAGGRID)) {
			click = ClkTagBar;
			i = (ev->x - x) / (bh / tagrows);
			i = i + columns * (ev->y / (bh / tagrows));
			if (i >= LENGTH(tags)) {
				i = LENGTH(tags) - 1;
			}
			arg.ui = 1 << i;
		}
		else if (ev->x < x + blw + columns * bh / tagrows)
			click = ClkLtSymbol;
		#else // TAGGRID_PATCH
		} else if (ev->x < x + blw)
			click = ClkLtSymbol;
		#endif // TAGGRID_PATCH
		else if (ev->x > selmon->ww - selmon->tw + padding)
		#if !STATUSCMD_PATCH
			click = ClkStatusText;
		#else
		{
			click = ClkStatusText;
			xc = selmon->ww - selmon->tw + padding;
			#if STATUSPADDING_PATCH
			xc += lrpad / 2;
			#endif // STATUSPADDING_PATCH
			char *text = rawstext;
			int i = -1;
			char ch;
			#if DWMBLOCKS_PATCH
			dwmblockssig = -1;
			#else
			statuscmdn = 0;
			#endif // DWMBLOCKS_PATCH
			while (text[++i]) {
				if ((unsigned char)text[i] < ' ') {
					ch = text[i];
					text[i] = '\0';
					#if STATUS2D_PATCH && !STATUSCOLORS_PATCH
					xc += status2dtextlength(text);
					#elif PANGO_PATCH
					xc += TEXTWM(text) - lrpad;
					#else
					xc += TEXTW(text) - lrpad;
					#endif // STATUS2D_PATCH
					text[i] = ch;
					text += i+1;
					i = -1;
					#if DWMBLOCKS_PATCH
					if (xc >= ev->x && dwmblockssig != -1)
						break;
					dwmblockssig = ch;
					#else
					if (xc >= ev->x)
						break;
					if (ch <= LENGTH(statuscmds))
						statuscmdn = ch - 1;
					#endif // DWMBLOCKS_PATCH
				}
			}
			#if DWMBLOCKS_PATCH
			if (dwmblockssig == -1)
				dwmblockssig = 0;
			#endif // DWMBLOCKS_PATCH
		}
		#endif // STATUSCMD_PATCH
		#if AWESOMEBAR_PATCH
		else {
			x += blw;
			c = m->clients;

			do {
				if (!c || !ISVISIBLE(c))
					continue;
				else
					x += (1.0 / (double)m->bt) * m->btw;
			} while (c && ev->x > x && (c = c->next));

			if (c) {
				click = ClkWinTitle;
				arg.v = c;
			}
		}
		#else
		else
			click = ClkWinTitle;
		#endif // AWESOMEBAR_PATCH
		#if LEFTLAYOUT_PATCH
		}
		#endif // LEFTLAYOUT_PATCH
	} else if ((c = wintoclient(ev->window))) {
		#if FOCUSONCLICK_PATCH
		if (focusonwheel || (ev->button != Button4 && ev->button != Button5))
			focus(c);
		#else
		focus(c);
		restack(selmon);
		#endif // FOCUSONCLICK_PATCH
		XAllowEvents(dpy, ReplayPointer, CurrentTime);
		click = ClkClientWin;
	}
	for (i = 0; i < LENGTH(buttons); i++)
		if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
		&& CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state))
			#if AWESOMEBAR_PATCH
			buttons[i].func((click == ClkTagBar || click == ClkWinTitle) && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg);
			#else
			buttons[i].func(click == ClkTagBar && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg);
			#endif
}

void
checkotherwm(void)
{
	xerrorxlib = XSetErrorHandler(xerrorstart);
	/* this causes an error if some other window manager is running */
	XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureRedirectMask);
	XSync(dpy, False);
	XSetErrorHandler(xerror);
	XSync(dpy, False);
}

void
cleanup(void)
{
	Arg a = {.ui = ~0};
	Layout foo = { "", NULL };
	Monitor *m;
	size_t i;

	view(&a);
	selmon->lt[selmon->sellt] = &foo;
	for (m = mons; m; m = m->next)
		while (m->stack)
			unmanage(m->stack, 0);
	XUngrabKey(dpy, AnyKey, AnyModifier, root);
	while (mons)
		cleanupmon(mons);
	#if SYSTRAY_PATCH
	if (showsystray) {
		XUnmapWindow(dpy, systray->win);
		XDestroyWindow(dpy, systray->win);
		free(systray);
	}
	#endif // SYSTRAY_PATCH
	for (i = 0; i < CurLast; i++)
		drw_cur_free(drw, cursor[i]);
	#if STATUS2D_PATCH && !STATUSCOLORS_PATCH
	for (i = 0; i < LENGTH(colors) + 1; i++)
	#else
	for (i = 0; i < LENGTH(colors); i++)
	#endif // STATUS2D_PATCH
		free(scheme[i]);
	free(scheme);
	XDestroyWindow(dpy, wmcheckwin);
	#if PANGO_PATCH
	drw_font_free(drw->font);
	#else
	drw_fontset_free(drw->fonts);
	#endif // PANGO_PATCH
	drw_free(drw);
	XSync(dpy, False);
	XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
	XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
}

void
cleanupmon(Monitor *mon)
{
	Monitor *m;

	if (mon == mons)
		mons = mons->next;
	else {
		for (m = mons; m && m->next != mon; m = m->next);
		m->next = mon->next;
	}
	XUnmapWindow(dpy, mon->barwin);
	#if EXTRABAR_PATCH
	XUnmapWindow(dpy, mon->extrabarwin);
	#endif // EXTRABAR_PATCH
	XDestroyWindow(dpy, mon->barwin);
	#if EXTRABAR_PATCH
	XDestroyWindow(dpy, mon->extrabarwin);
	#endif // EXTRABAR_PATCH
	free(mon);
}

void
clientmessage(XEvent *e)
{
	#if SYSTRAY_PATCH
	XWindowAttributes wa;
	XSetWindowAttributes swa;
	#endif // SYSTRAY_PATCH
	XClientMessageEvent *cme = &e->xclient;
	Client *c = wintoclient(cme->window);
	#if FOCUSONNETACTIVE_PATCH
	unsigned int i;
	#endif // FOCUSONNETACTIVE_PATCH

	#if SYSTRAY_PATCH
	if (showsystray && cme->window == systray->win && cme->message_type == netatom[NetSystemTrayOP]) {
		/* add systray icons */
		if (cme->data.l[1] == SYSTEM_TRAY_REQUEST_DOCK) {
			if (!(c = (Client *)calloc(1, sizeof(Client))))
				die("fatal: could not malloc() %u bytes\n", sizeof(Client));
			if (!(c->win = cme->data.l[2])) {
				free(c);
				return;
			}

			/* Clear status bar to avoid artifacts beneath systray icons */
			drw_rect(drw, 0, 0, selmon->ww, bh, 1, 1);
			drw_map(drw, selmon->barwin, 0, 0, selmon->ww, bh);

			c->mon = selmon;
			c->next = systray->icons;
			systray->icons = c;
			XGetWindowAttributes(dpy, c->win, &wa);
			c->x = c->oldx = c->y = c->oldy = 0;
			c->w = c->oldw = wa.width;
			c->h = c->oldh = wa.height;
			c->oldbw = wa.border_width;
			c->bw = 0;
			c->isfloating = True;
			/* reuse tags field as mapped status */
			c->tags = 1;
			updatesizehints(c);
			updatesystrayicongeom(c, wa.width, wa.height);
			XAddToSaveSet(dpy, c->win);
			XSelectInput(dpy, c->win, StructureNotifyMask | PropertyChangeMask | ResizeRedirectMask);
			XReparentWindow(dpy, c->win, systray->win, 0, 0);
			/* use parents background color */
			swa.background_pixel  = scheme[SchemeNorm][ColBg].pixel;
			XChangeWindowAttributes(dpy, c->win, CWBackPixel, &swa);
			sendevent(c->win, netatom[Xembed], StructureNotifyMask, CurrentTime, XEMBED_EMBEDDED_NOTIFY, 0 , systray->win, XEMBED_EMBEDDED_VERSION);
			XSync(dpy, False);
			setclientstate(c, NormalState);
			drawbar(selmon);
			updatesystray();
		}
		return;
	}
	#endif // SYSTRAY_PATCH

	if (!c)
		return;
	if (cme->message_type == netatom[NetWMState]) {
		if (cme->data.l[1] == netatom[NetWMFullscreen]
		|| cme->data.l[2] == netatom[NetWMFullscreen]) {
			#if FAKEFULLSCREEN_CLIENT_PATCH
			if (c->fakefullscreen)
				resizeclient(c, c->x, c->y, c->w, c->h);
			else
				setfullscreen(c, (cme->data.l[0] == 1 /* _NET_WM_STATE_ADD    */
					|| (cme->data.l[0] == 2 /* _NET_WM_STATE_TOGGLE */
					#if !FAKEFULLSCREEN_PATCH
					&& !c->isfullscreen
					#endif // !FAKEFULLSCREEN_PATCH
				)));
			#else
			setfullscreen(c, (cme->data.l[0] == 1 /* _NET_WM_STATE_ADD    */
				|| (cme->data.l[0] == 2 /* _NET_WM_STATE_TOGGLE */
				#if !FAKEFULLSCREEN_PATCH
				&& !c->isfullscreen
				#endif // !FAKEFULLSCREEN_PATCH
			)));
			#endif // FAKEFULLSCREEN_CLIENT_PATCH
		}
	} else if (cme->message_type == netatom[NetActiveWindow]) {
		#if FOCUSONNETACTIVE_PATCH
		for (i = 0; i < LENGTH(tags) && !((1 << i) & c->tags); i++);
		if (i < LENGTH(tags)) {
			const Arg a = {.ui = 1 << i};
			selmon = c->mon;
			view(&a);
			focus(c);
			restack(selmon);
		}
		#else
		if (c != selmon->sel && !c->isurgent)
			seturgent(c, 1);
		#endif // FOCUSONNETACTIVE_PATCH
	}
}

void
configure(Client *c)
{
	XConfigureEvent ce;

	ce.type = ConfigureNotify;
	ce.display = dpy;
	ce.event = c->win;
	ce.window = c->win;
	ce.x = c->x;
	ce.y = c->y;
	ce.width = c->w;
	ce.height = c->h;
	ce.border_width = c->bw;
	ce.above = None;
	ce.override_redirect = False;
	XSendEvent(dpy, c->win, False, StructureNotifyMask, (XEvent *)&ce);
}

void
configurenotify(XEvent *e)
{
	Monitor *m;
	#if !FAKEFULLSCREEN_PATCH
	Client *c;
	#endif // !FAKEFULLSCREEN_PATCH
	XConfigureEvent *ev = &e->xconfigure;
	int dirty;

	/* TODO: updategeom handling sucks, needs to be simplified */
	if (ev->window == root) {
		dirty = (sw != ev->width || sh != ev->height);
		sw = ev->width;
		sh = ev->height;
		if (updategeom() || dirty) {
			drw_resize(drw, sw, bh);
			updatebars();
			for (m = mons; m; m = m->next) {
				#if !FAKEFULLSCREEN_PATCH
				for (c = m->clients; c; c = c->next)
					#if FAKEFULLSCREEN_CLIENT_PATCH
					if (c->isfullscreen && !c->fakefullscreen)
					#else
					if (c->isfullscreen)
					#endif // FAKEFULLSCREEN_CLIENT_PATCH
						resizeclient(c, m->mx, m->my, m->mw, m->mh);
				#endif // !FAKEFULLSCREEN_PATCH
				#if BARPADDING_PATCH
				XMoveResizeWindow(dpy, m->barwin, m->wx + sp, m->by + vp, m->ww -  2 * sp, bh);
				#else
				XMoveResizeWindow(dpy, m->barwin, m->wx, m->by, m->ww, bh);
				#endif // BARPADDING_PATCH
				#if EXTRABAR_PATCH
				#if BARPADDING_PATCH
				XMoveResizeWindow(dpy, m->extrabarwin, m->wx + sp, m->eby - vp, m->ww - 2 * sp, bh);
				#else
				XMoveResizeWindow(dpy, m->extrabarwin, m->wx, m->eby, m->ww, bh);
				#endif // BARPADDING_PATCH
				#endif // EXTRABAR_PATCH
			}
			focus(NULL);
			arrange(NULL);
		}
	}
}

void
configurerequest(XEvent *e)
{
	Client *c;
	Monitor *m;
	XConfigureRequestEvent *ev = &e->xconfigurerequest;
	XWindowChanges wc;

	if ((c = wintoclient(ev->window))) {
		if (ev->value_mask & CWBorderWidth)
			c->bw = ev->border_width;
		else if (c->isfloating || !selmon->lt[selmon->sellt]->arrange) {
			m = c->mon;
			if (ev->value_mask & CWX) {
				c->oldx = c->x;
				c->x = m->mx + ev->x;
			}
			if (ev->value_mask & CWY) {
				c->oldy = c->y;
				c->y = m->my + ev->y;
			}
			if (ev->value_mask & CWWidth) {
				c->oldw = c->w;
				c->w = ev->width;
			}
			if (ev->value_mask & CWHeight) {
				c->oldh = c->h;
				c->h = ev->height;
			}
			if ((c->x + c->w) > m->mx + m->mw && c->isfloating)
				c->x = m->mx + (m->mw / 2 - WIDTH(c) / 2); /* center in x direction */
			if ((c->y + c->h) > m->my + m->mh && c->isfloating)
				c->y = m->my + (m->mh / 2 - HEIGHT(c) / 2); /* center in y direction */
			if ((ev->value_mask & (CWX|CWY)) && !(ev->value_mask & (CWWidth|CWHeight)))
				configure(c);
			if (ISVISIBLE(c))
				XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
			#if AUTORESIZE_PATCH
			else
				c->needresize = 1;
			#endif // AUTORESIZE_PATCH
		} else
			configure(c);
	} else {
		wc.x = ev->x;
		wc.y = ev->y;
		wc.width = ev->width;
		wc.height = ev->height;
		wc.border_width = ev->border_width;
		wc.sibling = ev->above;
		wc.stack_mode = ev->detail;
		XConfigureWindow(dpy, ev->window, ev->value_mask, &wc);
	}
	XSync(dpy, False);
}

Monitor *
createmon(void)
{
	Monitor *m;
	#if PERTAG_PATCH
	int i;
	#endif // PERTAG_PATCH
	#if MONITOR_RULES_PATCH
	int mc, j;
	Monitor *mi;
	const MonitorRule *mr;
	#endif // MONITOR_RULES_PATCH

	m = ecalloc(1, sizeof(Monitor));
	#if EMPTYVIEW_PATCH
	m->tagset[0] = m->tagset[1] = 0;
	#else
	m->tagset[0] = m->tagset[1] = 1;
	#endif // EMPTYVIEW_PATCH
	m->mfact = mfact;
	m->nmaster = nmaster;
	#if FLEXTILE_DELUXE_LAYOUT
	m->nstack = nstack;
	#endif // FLEXTILE_DELUXE_LAYOUT
	m->showbar = showbar;
	m->topbar = topbar;
	#if SETBORDERPX_PATCH
	m->borderpx = borderpx;
	#endif // SETBORDERPX_PATCH
	#if VANITYGAPS_PATCH
	m->gappih = gappih;
	m->gappiv = gappiv;
	m->gappoh = gappoh;
	m->gappov = gappov;
	#endif // VANITYGAPS_PATCH
	#if MONITOR_RULES_PATCH
	for (mc = 0, mi = mons; mi; mi = mi->next, mc++);
	for (j = 0; j < LENGTH(monrules); j++) {
		mr = &monrules[j];
		if ((mr->monitor == -1 || mr->monitor == mc)
		#if PERTAG_PATCH
				&& (mr->tag == -1 || mr->tag == 0)
		#endif // PERTAG_PATCH
		) {
			m->lt[0] = &layouts[mr->layout];
			m->lt[1] = &layouts[1 % LENGTH(layouts)];
			strncpy(m->ltsymbol, layouts[mr->layout].symbol, sizeof m->ltsymbol);

			if (mr->mfact > -1)
				m->mfact = mr->mfact;
			if (mr->nmaster > -1)
				m->nmaster = mr->nmaster;
			if (mr->showbar > -1)
				m->showbar = mr->showbar;
			if (mr->topbar > -1)
				m->topbar = mr->topbar;
			break;
		}
	}
	#else
	m->lt[0] = &layouts[0];
	m->lt[1] = &layouts[1 % LENGTH(layouts)];
	strncpy(m->ltsymbol, layouts[0].symbol, sizeof m->ltsymbol);
	#endif // MONITOR_RULES_PATCH

	#if FLEXTILE_DELUXE_LAYOUT
	m->ltaxis[LAYOUT] = m->lt[0]->preset.layout;
	m->ltaxis[MASTER] = m->lt[0]->preset.masteraxis;
	m->ltaxis[STACK]  = m->lt[0]->preset.stack1axis;
	m->ltaxis[STACK2] = m->lt[0]->preset.stack2axis;
	#endif // FLEXTILE_DELUXE_LAYOUT

	#if PERTAG_PATCH
	if (!(m->pertag = (Pertag *)calloc(1, sizeof(Pertag))))
		die("fatal: could not malloc() %u bytes\n", sizeof(Pertag));
	m->pertag->curtag = m->pertag->prevtag = 1;
	for (i = 0; i <= LENGTH(tags); i++) {
		#if FLEXTILE_DELUXE_LAYOUT
		m->pertag->nstacks[i] = m->nstack;
		#endif // FLEXTILE_DELUXE_LAYOUT

		#if !MONITOR_RULES_PATCH
		/* init nmaster */
		m->pertag->nmasters[i] = m->nmaster;

		/* init mfacts */
		m->pertag->mfacts[i] = m->mfact;

		#if PERTAGBAR_PATCH
		/* init showbar */
		m->pertag->showbars[i] = m->showbar;
		#endif // PERTAGBAR_PATCH
		#endif // MONITOR_RULES_PATCH

		#if ZOOMSWAP_PATCH
		m->pertag->prevzooms[i] = NULL;
		#endif // ZOOMSWAP_PATCH

		/* init layouts */
		#if MONITOR_RULES_PATCH
		for (j = 0; j < LENGTH(monrules); j++) {
			mr = &monrules[j];
			if ((mr->monitor == -1 || mr->monitor == mc) && (mr->tag == -1 || mr->tag == i)) {
				m->pertag->ltidxs[i][0] = &layouts[mr->layout];
				m->pertag->ltidxs[i][1] = m->lt[0];
				m->pertag->nmasters[i] = (mr->nmaster > -1 ? mr->nmaster : m->nmaster);
				m->pertag->mfacts[i] = (mr->mfact > -1 ? mr->mfact : m->mfact);
				#if PERTAGBAR_PATCH
				m->pertag->showbars[i] = (mr->showbar > -1 ? mr->showbar : m->showbar);
				#endif // PERTAGBAR_PATCH
				#if FLEXTILE_DELUXE_LAYOUT
				m->pertag->ltaxis[i][LAYOUT] = m->pertag->ltidxs[i][0]->preset.layout;
				m->pertag->ltaxis[i][MASTER] = m->pertag->ltidxs[i][0]->preset.masteraxis;
				m->pertag->ltaxis[i][STACK]  = m->pertag->ltidxs[i][0]->preset.stack1axis;
				m->pertag->ltaxis[i][STACK2] = m->pertag->ltidxs[i][0]->preset.stack2axis;
				#endif // FLEXTILE_DELUXE_LAYOUT
				break;
			}
		}
		#else
		m->pertag->ltidxs[i][0] = m->lt[0];
		m->pertag->ltidxs[i][1] = m->lt[1];
		#if FLEXTILE_DELUXE_LAYOUT
		/* init flextile axes */
		m->pertag->ltaxis[i][LAYOUT] = m->ltaxis[LAYOUT];
		m->pertag->ltaxis[i][MASTER] = m->ltaxis[MASTER];
		m->pertag->ltaxis[i][STACK]  = m->ltaxis[STACK];
		m->pertag->ltaxis[i][STACK2] = m->ltaxis[STACK2];
		#endif // FLEXTILE_DELUXE_LAYOUT
		#endif // MONITOR_RULES_PATCH
		m->pertag->sellts[i] = m->sellt;

		#if VANITYGAPS_PATCH
		m->pertag->enablegaps[i] = 1;
		#endif // VANITYGAPS_PATCH
	}
	#endif // PERTAG_PATCH
	return m;
}

void
destroynotify(XEvent *e)
{
	Client *c;
	XDestroyWindowEvent *ev = &e->xdestroywindow;

	if ((c = wintoclient(ev->window)))
		unmanage(c, 1);
	#if SWALLOW_PATCH
	else if ((c = swallowingclient(ev->window)))
		unmanage(c->swallowing, 1);
	#endif // SWALLOW_PATCH
	#if SYSTRAY_PATCH
	else if (showsystray && (c = wintosystrayicon(ev->window))) {
		removesystrayicon(c);
		updatesystray();
	}
	#endif // SYSTRAY_PATCH
}

void
detach(Client *c)
{
	Client **tc;

	for (tc = &c->mon->clients; *tc && *tc != c; tc = &(*tc)->next);
	*tc = c->next;
}

void
detachstack(Client *c)
{
	Client **tc, *t;

	for (tc = &c->mon->stack; *tc && *tc != c; tc = &(*tc)->snext);
	*tc = c->snext;

	if (c == c->mon->sel) {
		for (t = c->mon->stack; t && !ISVISIBLE(t); t = t->snext);
		c->mon->sel = t;
	}
}

Monitor *
dirtomon(int dir)
{
	Monitor *m = NULL;

	if (dir > 0) {
		if (!(m = selmon->next))
			m = mons;
	} else if (selmon == mons)
		for (m = mons; m->next; m = m->next);
	else
		for (m = mons; m->next != selmon; m = m->next);
	return m;
}

void
drawbar(Monitor *m)
{
	int x, w, stw = 0, stp = 0, invert;
	#if ALTERNATIVE_TAGS_PATCH
	int wdelta;
	#endif // ALTERNATIVE_TAGS_PATCH
	#if AWESOMEBAR_PATCH
	int n = 0, scm, remainder, tabw;
	#elif FANCYBAR_PATCH
	int ftw, mw, ew = 0;
	int n = 0;
	#endif // FANCYBAR_PATCH, AWESOMEBAR_PATCH
	#if BARPADDING_PATCH
	stw = 2 * sp;
	#endif // BARPADDING_PATCH
	#if STATUSPADDING_PATCH
	stp = lrpad / 2;
	#endif // STATUSPADDING_PATCH
	#if !HIDEVACANTTAGS_PATCH
	#if !ACTIVETAGINDICATORBAR_PATCH
	#if PANGO_PATCH
	int boxs = drw->font->h / 9;
	#else
	int boxs = drw->fonts->h / 9;
	#endif // PANGO_PATCH
	#endif // ACTIVETAGINDICATORBAR_PATCH
	#if PANGO_PATCH
	int boxw = drw->font->h / 6 + 2;
	#else
	int boxw = drw->fonts->h / 6 + 2;
	#endif // PANGO_PATCH
	#endif // HIDEVACANTTAGS_PATCH
	unsigned int i, occ = 0, urg = 0;
	#if STATUSCOLORS_PATCH
	char *ts = stext;
	char *tp = stext;
	int tx = 0;
	char ctmp;
	#endif // STATUSCOLORS_PATCH
	Client *c;

	#if SYSTRAY_PATCH
	if (showsystray && m == systraytomon(m))
		stw += getsystraywidth();
	#endif // SYSTRAY_PATCH

	/* draw status first so it can be overdrawn by tags later */
	#if !STATUSALLMONS_PATCH
	#if STATICSTATUS_PATCH
	if (m == statmon)
	#else
	if (m == selmon)
	#endif // STATICSTATUS_PATCH
	{ /* status is only drawn on selected monitor */
	#endif // STATUSALLMONS_PATCH
		#if VTCOLORS_PATCH
		drw_setscheme(drw, scheme[SchemeStatus]);
		#else
		drw_setscheme(drw, scheme[SchemeNorm]);
		#endif // VTCOLORS_PATCH
		#if STATUSCOLORS_PATCH
		#if STATUSPADDING_PATCH
		m->tw = textw_wosc(stext) + lrpad + 2;
		#else
		m->tw = textw_wosc(stext) + 2;
		#endif // STATUSPADDING_PATCH
		while (1) {
			if ((unsigned int)*ts > LENGTH(colors)) {
				ts++;
				continue;
			}
			ctmp = *ts;
			*ts = '\0';
			#if PANGO_PATCH
			drw_text(drw, m->ww - m->tw - stw + tx, 0, m->tw - tx, bh, stp, tp, 0, True);
			tx += TEXTWM(tp) - lrpad;
			#else
			drw_text(drw, m->ww - m->tw - stw + tx, 0, m->tw - tx, bh, stp, tp, 0);
			tx += TEXTW(tp) - lrpad;
			#endif // PANGO_PATCH
			if (ctmp == '\0')
				break;
			drw_setscheme(drw, scheme[(unsigned int)(ctmp-1)]);
			*ts = ctmp;
			tp = ++ts;
		}
		#elif STATUS2D_PATCH
		m->tw = m->ww - drawstatusbar(m, bh, stext, stw, stp);
		#else // STATUSCOLORS_PATCH
		#if STATUSPADDING_PATCH && PANGO_PATCH
		m->tw = TEXTWM(stext);
		#elif STATUSPADDING_PATCH
		m->tw = TEXTW(stext);
		#elif PANGO_PATCH
		m->tw = TEXTWM(stext) - lrpad + 2; /* 2px right padding */
		#else
		m->tw = TEXTW(stext) - lrpad + 2; /* 2px right padding */
		#endif // STATUSPADDING_PATCH | PANGO_PATCH
		#if PANGO_PATCH
		drw_text(drw, m->ww - m->tw - stw, 0, m->tw, bh, stp, stext, 0, True);
		#else
		drw_text(drw, m->ww - m->tw - stw, 0, m->tw, bh, stp, stext, 0);
		#endif // PANGO_PATCH
		#endif // STATUSCOLORS_PATCH
	#if !STATUSALLMONS_PATCH
	}
	#endif // STATUSALLMONS_PATCH

	for (c = m->clients; c; c = c->next) {
		#if AWESOMEBAR_PATCH || FANCYBAR_PATCH
		if (ISVISIBLE(c))
			n++;
		#endif // FANCYBAR_PATCH
		#if HIDEVACANTTAGS_PATCH
		occ |= c->tags == 255 ? 0 : c->tags;
		#else
		occ |= c->tags;
		#endif // HIDEVACANTTAGS_PATCH
		if (c->isurgent)
			urg |= c->tags;
	}
	x = 0;
	#if LEFTLAYOUT_PATCH
	w = blw = TEXTW(m->ltsymbol);
	#if VTCOLORS_PATCH
	drw_setscheme(drw, scheme[SchemeTagsNorm]);
	#else
	drw_setscheme(drw, scheme[SchemeNorm]);
	#endif // VTCOLORS_PATCH
	#if PANGO_PATCH
	x = drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0, False);
	#else
	x = drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0);
	#endif // PANGO_PATCH
	#endif // LEFTLAYOUT_PATCH
	#if TAGGRID_PATCH
	if (drawtagmask & DRAWCLASSICTAGS)
	#endif // TAGGRID_PATCH
	for (i = 0; i < LENGTH(tags); i++) {
		#if URGENTBORDER_PATCH
		invert = 0;
		#else
		invert = urg & 1 << i;
		#endif // URGENTBORDER_PATCH
		#if HIDEVACANTTAGS_PATCH
		/* do not draw vacant tags */
		if (!(occ & 1 << i || m->tagset[m->seltags] & 1 << i))
		continue;
		#endif // HIDEVACANTTAGS_PATCH
		w = TEXTW(tags[i]);
		#if ALTERNATIVE_TAGS_PATCH
		wdelta = selmon->alttag ? abs(TEXTW(tags[i]) - TEXTW(tagsalt[i])) / 2 : 0;
		#endif // ALTERNATIVE_TAGS_PATCH
		#if URGENTBORDER_PATCH
		if (m->tagset[m->seltags] & 1 << i)
			#if VTCOLORS_PATCH
			drw_setscheme(drw, scheme[SchemeTagsSel]);
			#else
			drw_setscheme(drw, scheme[SchemeSel]);
			#endif // VTCOLORS_PATCH
		else
			#if VTCOLORS_PATCH
			drw_setscheme(drw, scheme[urg & 1 << i ? SchemeUrg : SchemeTagsNorm]);
			#else
			drw_setscheme(drw, scheme[urg & 1 << i ? SchemeUrg : SchemeNorm]);
			#endif // VTCOLORS_PATCH
		#elif VTCOLORS_PATCH
		drw_setscheme(drw, scheme[m->tagset[m->seltags] & 1 << i ? SchemeTagsSel : SchemeTagsNorm]);
		#else // URGENTBORDER_PATCH
		drw_setscheme(drw, scheme[m->tagset[m->seltags] & 1 << i ? SchemeSel : SchemeNorm]);
		#endif // URGENTBORDER_PATCH
		#if ALTERNATIVE_TAGS_PATCH && PANGO_PATCH
		drw_text(drw, x, 0, w, bh, wdelta + lrpad / 2, (selmon->alttag ? tagsalt[i] : tags[i]), invert, False);
		#elif ALTERNATIVE_TAGS_PATCH
		drw_text(drw, x, 0, w, bh, wdelta + lrpad / 2, (selmon->alttag ? tagsalt[i] : tags[i]), invert);
		#elif PANGO_PATCH
		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], invert, False);
		#else
		drw_text(drw, x, 0, w, bh, lrpad / 2, tags[i], invert);
		#endif // ALTERNATIVE_TAGS_PATCH
		#if !HIDEVACANTTAGS_PATCH
		if (occ & 1 << i)
			#if ACTIVETAGINDICATORBAR_PATCH
			drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw,
			#else
			drw_rect(drw, x + boxs, boxs, boxw, boxw,
			#endif // ACTIVETAGINDICATORBAR_PATCH
				m == selmon && selmon->sel && selmon->sel->tags & 1 << i, invert);
		#endif // HIDEVACANTTAGS_PATCH
		x += w;
	}
	#if TAGGRID_PATCH
	if (drawtagmask & DRAWTAGGRID) {
		drawtaggrid(m,&x,occ);
	}
	#endif // TAGGRID_PATCH
	#if !LEFTLAYOUT_PATCH
	w = blw = TEXTW(m->ltsymbol);
	#if VTCOLORS_PATCH
	drw_setscheme(drw, scheme[SchemeTagsNorm]);
	#else
	drw_setscheme(drw, scheme[SchemeNorm]);
	#endif // VTCOLORS_PATCH
	#if PANGO_PATCH
	x = drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0, False);
	#else
	x = drw_text(drw, x, 0, w, bh, lrpad / 2, m->ltsymbol, 0);
	#endif // PANGO_PATCH
	#endif // LEFTLAYOUT_PATCH

	if ((w = m->ww - m->tw - stw - x) > bh)
	{
		#if AWESOMEBAR_PATCH
		if (n > 0) {
			remainder = w % n;
			tabw = (1.0 / (double)n) * w + 1;
			for (c = m->clients; c; c = c->next) {
				if (!ISVISIBLE(c))
					continue;
				if (m->sel == c)
					#if VTCOLORS_PATCH
					scm = SchemeTitleSel;
					#elif TITLECOLOR_PATCH
					scm = SchemeTitle;
					#else
					scm = SchemeSel;
					#endif // VTCOLORS_PATCH / TITLECOLOR_PATCH
				else if (HIDDEN(c))
					scm = SchemeHid;
				else
					#if VTCOLORS_PATCH
					scm = SchemeTitleNorm;
					#else
					scm = SchemeNorm;
					#endif // VTCOLORS_PATCH

				drw_setscheme(drw, scheme[scm]);
				if (remainder >= 0) {
					if (remainder == 0)
						tabw--;
					remainder--;
				}
				#if PANGO_PATCH
				drw_text(drw, x, 0, (1.0 / (double)n) * w, bh, lrpad / 2, c->name, 0, False);
				#else
				drw_text(drw, x, 0, (1.0 / (double)n) * w, bh, lrpad / 2, c->name, 0);
				#endif // PANGO_PATCH
				x += tabw;
			}
		} else {
			#if VTCOLORS_PATCH
			drw_setscheme(drw, scheme[SchemeTitleNorm]);
			#else
			drw_setscheme(drw, scheme[SchemeNorm]);
			#endif // VTCOLORS_PATCH
			drw_rect(drw, x, 0, w, bh, 1, 1);
		}
		#elif FANCYBAR_PATCH
		if (n > 0) {
			ftw = TEXTW(m->sel->name) + lrpad;
			mw = (ftw >= w || n == 1) ? 0 : (w - ftw) / (n - 1);

			i = 0;
			for (c = m->clients; c; c = c->next) {
				if (!ISVISIBLE(c) || c == m->sel)
					continue;
				ftw = TEXTW(c->name);
				if (ftw < mw)
					ew += (mw - ftw);
				else
					i++;
			}
			if (i > 0)
				mw += ew / i;

			for (c = m->clients; c; c = c->next) {
				if (!ISVISIBLE(c))
					continue;
				ftw = MIN(m->sel == c ? w : mw, TEXTW(c->name));

				#if VTCOLORS_PATCH
				drw_setscheme(drw, scheme[m->sel == c ? SchemeTitleSel : SchemeTitleNorm]);
				#elif TITLECOLOR_PATCH
				drw_setscheme(drw, scheme[m->sel == c ? SchemeTitle : SchemeNorm]);
				#else
				drw_setscheme(drw, scheme[m->sel == c ? SchemeSel : SchemeNorm]);
				#endif // VTCOLORS_PATCH / TITLECOLOR_PATCH
				if (ftw > 0) /* trap special handling of 0 in drw_text */
					#if PANGO_PATCH
					drw_text(drw, x, 0, ftw, bh, lrpad / 2, c->name, 0, False);
					#else
					drw_text(drw, x, 0, ftw, bh, lrpad / 2, c->name, 0);
					#endif // PANGO_PATCH
				#if !HIDEVACANTTAGS_PATCH
				if (c->isfloating)
					#if ACTIVETAGINDICATORBAR_PATCH
					drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw, c->isfixed, 0);
					#else
					drw_rect(drw, x + boxs, boxs, boxw, boxw, c->isfixed, 0);
					#endif // ACTIVETAGINDICATORBAR_PATCH
				#endif // HIDEVACANTTAGS_PATCH
				x += ftw;
				w -= ftw;
			}
		}
		#if VTCOLORS_PATCH
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		#else
		drw_setscheme(drw, scheme[SchemeNorm]);
		#endif // VTCOLORS_PATCH
		drw_rect(drw, x, 0, w, bh, 1, 1);
		#else
		if (m->sel) {
			#if VTCOLORS_PATCH
			drw_setscheme(drw, scheme[m == selmon ? SchemeTitleSel : SchemeTitleNorm]);
			#elif TITLECOLOR_PATCH
			drw_setscheme(drw, scheme[m == selmon ? SchemeTitle : SchemeNorm]);
			#else
			drw_setscheme(drw, scheme[m == selmon ? SchemeSel : SchemeNorm]);
			#endif // VTCOLORS_PATCH / TITLECOLOR_PATCH
			#if IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
			XSetErrorHandler(xerrordummy);
			#endif // IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
			#if CENTEREDWINDOWNAME_PATCH
			int mid = (m->ww - TEXTW(m->sel->name)) / 2 - x;
			#if BARPADDING_PATCH && PANGO_PATCH
			drw_text(drw, x, 0, w - 2*sp, bh, mid, m->sel->name, 0, False);
			#elif BARPADDING_PATCH
			drw_text(drw, x, 0, w - 2*sp, bh, mid, m->sel->name, 0);
			#elif PANGO_PATCH
			drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0, False);
			#else
			drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0);
			#endif // BARPADDING_PATCH
			#else
			#if BARPADDING_PATCH && PANGO_PATCH
			drw_text(drw, x, 0, w - 2*sp, bh, lrpad / 2, m->sel->name, 0, False);
			#elif BARPADDING_PATCH
			drw_text(drw, x, 0, w - 2*sp, bh, lrpad / 2, m->sel->name, 0);
			#elif PANGO_PATCH
			drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0, False);
			#else
			drw_text(drw, x, 0, w, bh, lrpad / 2, m->sel->name, 0);
			#endif // BARPADDING_PATCH
			#endif // CENTEREDWINDOWNAME_PATCH
			#if IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
			XSync(dpy, False);
			XSetErrorHandler(xerror);
			#endif // IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
			#if !HIDEVACANTTAGS_PATCH
			if (m->sel->isfloating)
				#if ACTIVETAGINDICATORBAR_PATCH
				drw_rect(drw, x + boxw, 0, w - ( 2 * boxw + 1), boxw, m->sel->isfixed, 0);
				#else
				drw_rect(drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0);
				#endif // ACTIVETAGINDICATORBAR_PATCH
			#endif // HIDEVACANTTAGS_PATCH
		} else {
			#if VTCOLORS_PATCH
			drw_setscheme(drw, scheme[SchemeTitleNorm]);
			#else
			drw_setscheme(drw, scheme[SchemeNorm]);
			#endif // VTCOLORS_PATCH
			#if BARPADDING_PATCH
			drw_rect(drw, x, 0, w - 2 * sp, bh, 1, 1);
			#else
			drw_rect(drw, x, 0, w, bh, 1, 1);
			#endif // BARPADDING_PATCH
		}
		#endif // FANCYBAR_PATCH, AWESOMEBAR_PATCH
	}

	#if AWESOMEBAR_PATCH
	m->bt = n;
	m->btw = w;
	#endif // AWESOMEBAR_PATCH
	drw_map(drw, m->barwin, 0, 0, m->ww - stw, bh);
	#if EXTRABAR_PATCH
	if (m == selmon) { /* extra status is only drawn on selected monitor */
		#if VTCOLORS_PATCH
		drw_setscheme(drw, scheme[SchemeTitleNorm]);
		#else
		drw_setscheme(drw, scheme[SchemeNorm]);
		#endif // VTCOLORS_PATCH
		#if PANGO_PATCH
		drw_text(drw, 0, 0, mons->ww, bh, 0, estext, 0, True);
		#else
		drw_text(drw, 0, 0, mons->ww, bh, 0, estext, 0);
		#endif // PANGO_PATCH
		drw_map(drw, m->extrabarwin, 0, 0, m->ww, bh);
	}
	#endif // EXTRABAR_PATCH
}

void
drawbars(void)
{
	Monitor *m;

	#if SYSTRAY_PATCH
	if (showsystray && systraypinning == 0) {
		/* Clear status bar to avoid artifacts beneath systray icons */
		drw_setscheme(drw, scheme[SchemeNorm]);
		drw_rect(drw, 0, 0, selmon->ww, bh, 1, 1);
		drw_map(drw, selmon->barwin, 0, 0, selmon->ww, bh);
	}
	#endif // SYSTRAY_PATCH

	for (m = mons; m; m = m->next)
		drawbar(m);

	#if SYSTRAY_PATCH
	if (showsystray)
		updatesystray();
	#endif // SYSTRAY_PATCH
}

#if !FOCUSONCLICK_PATCH
void
enternotify(XEvent *e)
{
	Client *c;
	Monitor *m;
	XCrossingEvent *ev = &e->xcrossing;

	if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != root)
		return;
	c = wintoclient(ev->window);
	m = c ? c->mon : wintomon(ev->window);
	if (m != selmon) {
		unfocus(selmon->sel, 1);
		selmon = m;
	} else if (!c || c == selmon->sel)
		return;
	focus(c);
}
#endif // FOCUSONCLICK_PATCH

void
expose(XEvent *e)
{
	Monitor *m;
	XExposeEvent *ev = &e->xexpose;

	if (ev->count == 0 && (m = wintomon(ev->window))) {
		drawbar(m);
	#if SYSTRAY_PATCH
		if (showsystray && m == selmon)
			updatesystray();
	#endif // SYSTRAY_PATCH
	}
}

void
focus(Client *c)
{
	#if AWESOMEBAR_PATCH
	if (!c || !ISVISIBLE(c) || HIDDEN(c))
		for (c = selmon->stack; c && (!ISVISIBLE(c) || HIDDEN(c)); c = c->snext);
	#else
	if (!c || !ISVISIBLE(c))
		for (c = selmon->stack; c && !ISVISIBLE(c); c = c->snext);
	#endif // AWESOMEBAR_PATCH
	if (selmon->sel && selmon->sel != c)
		unfocus(selmon->sel, 0);
	if (c) {
		if (c->mon != selmon)
			selmon = c->mon;
		if (c->isurgent)
			seturgent(c, 0);
		detachstack(c);
		attachstack(c);
		grabbuttons(c, 1);
		#if FLOAT_BORDER_COLOR_PATCH
		if (c->isfloating)
			XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColFloat].pixel);
		else
			XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColBorder].pixel);
		#else
		XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColBorder].pixel);
		#endif // FLOAT_BORDER_COLOR_PATCH
		setfocus(c);
	} else {
		XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
		XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
	}
	selmon->sel = c;
	drawbars();
}

/* there are some broken focus acquiring clients needing extra handling */
void
focusin(XEvent *e)
{
	XFocusChangeEvent *ev = &e->xfocus;

	if (selmon->sel && ev->window != selmon->sel->win)
		setfocus(selmon->sel);
}

void
focusmon(const Arg *arg)
{
	Monitor *m;

	if (!mons->next)
		return;
	if ((m = dirtomon(arg->i)) == selmon)
		return;
	unfocus(selmon->sel, 0);
	selmon = m;
	focus(NULL);
	#if WARP_PATCH
	warp(selmon->sel);
	#endif // WARP_PATCH
}

#if !STACKER_PATCH
void
focusstack(const Arg *arg)
{
	Client *c = NULL, *i;

	if (!selmon->sel)
		return;
	#if ALWAYSFULLSCREEN_PATCH
	if (selmon->sel->isfullscreen)
		return;
	#endif // ALWAYSFULLSCREEN_PATCH
	if (arg->i > 0) {
		for (c = selmon->sel->next; c && !ISVISIBLE(c); c = c->next);
		if (!c)
			for (c = selmon->clients; c && !ISVISIBLE(c); c = c->next);
	} else {
		for (i = selmon->clients; i != selmon->sel; i = i->next)
			if (ISVISIBLE(i))
				c = i;
		if (!c)
			for (; i; i = i->next)
				if (ISVISIBLE(i))
					c = i;
	}
	if (c) {
		focus(c);
		restack(selmon);
	}
}
#endif // STACKER_PATCH

Atom
getatomprop(Client *c, Atom prop)
{
	int di;
	unsigned long dl;
	unsigned char *p = NULL;
	Atom da, atom = None;

	#if SYSTRAY_PATCH
	/* FIXME getatomprop should return the number of items and a pointer to
	 * the stored data instead of this workaround */
	Atom req = XA_ATOM;
	if (prop == xatom[XembedInfo])
		req = xatom[XembedInfo];

	if (XGetWindowProperty(dpy, c->win, prop, 0L, sizeof atom, False, req,
		&da, &di, &dl, &dl, &p) == Success && p) {
		atom = *(Atom *)p;
		if (da == xatom[XembedInfo] && dl == 2)
			atom = ((Atom *)p)[1];
		XFree(p);
	}
	#else
	if (XGetWindowProperty(dpy, c->win, prop, 0L, sizeof atom, False, XA_ATOM,
		&da, &di, &dl, &dl, &p) == Success && p) {
		atom = *(Atom *)p;
		XFree(p);
	}
	#endif // SYSTRAY_PATCH
	return atom;
}

int
getrootptr(int *x, int *y)
{
	int di;
	unsigned int dui;
	Window dummy;

	return XQueryPointer(dpy, root, &dummy, &dummy, x, y, &di, &di, &dui);
}

long
getstate(Window w)
{
	int format;
	long result = -1;
	unsigned char *p = NULL;
	unsigned long n, extra;
	Atom real;

	if (XGetWindowProperty(dpy, w, wmatom[WMState], 0L, 2L, False, wmatom[WMState],
		&real, &format, &n, &extra, (unsigned char **)&p) != Success)
		return -1;
	if (n != 0)
		result = *p;
	XFree(p);
	return result;
}

int
gettextprop(Window w, Atom atom, char *text, unsigned int size)
{
	char **list = NULL;
	int n;
	XTextProperty name;

	if (!text || size == 0)
		return 0;
	text[0] = '\0';
	if (!XGetTextProperty(dpy, w, &name, atom) || !name.nitems)
		return 0;
	if (name.encoding == XA_STRING)
		strncpy(text, (char *)name.value, size - 1);
	else {
		if (XmbTextPropertyToTextList(dpy, &name, &list, &n) >= Success && n > 0 && *list) {
			strncpy(text, *list, size - 1);
			XFreeStringList(list);
		}
	}
	text[size - 1] = '\0';
	XFree(name.value);
	return 1;
}

void
grabbuttons(Client *c, int focused)
{
	updatenumlockmask();
	{
		unsigned int i, j;
		unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
		XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
		if (!focused)
			XGrabButton(dpy, AnyButton, AnyModifier, c->win, False,
				BUTTONMASK, GrabModeSync, GrabModeSync, None, None);
		for (i = 0; i < LENGTH(buttons); i++)
			if (buttons[i].click == ClkClientWin)
				for (j = 0; j < LENGTH(modifiers); j++)
					XGrabButton(dpy, buttons[i].button,
						buttons[i].mask | modifiers[j],
						c->win, False, BUTTONMASK,
						GrabModeAsync, GrabModeSync, None, None);
	}
}

void
#if KEYMODES_PATCH
grabdefkeys(void)
#else
grabkeys(void)
#endif // KEYMODES_PATCH
{
	updatenumlockmask();
	{
		unsigned int i, j;
		unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
		KeyCode code;

		XUngrabKey(dpy, AnyKey, AnyModifier, root);
		for (i = 0; i < LENGTH(keys); i++)
			if ((code = XKeysymToKeycode(dpy, keys[i].keysym)))
				for (j = 0; j < LENGTH(modifiers); j++)
					XGrabKey(dpy, code, keys[i].mod | modifiers[j], root,
						True, GrabModeAsync, GrabModeAsync);
	}
}

void
incnmaster(const Arg *arg)
{
	#if PERTAG_PATCH
	selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag] = MAX(selmon->nmaster + arg->i, 0);
	#else
	selmon->nmaster = MAX(selmon->nmaster + arg->i, 0);
	#endif // PERTAG_PATCH
	arrange(selmon);
}

#ifdef XINERAMA
static int
isuniquegeom(XineramaScreenInfo *unique, size_t n, XineramaScreenInfo *info)
{
	while (n--)
		if (unique[n].x_org == info->x_org && unique[n].y_org == info->y_org
		&& unique[n].width == info->width && unique[n].height == info->height)
			return 0;
	return 1;
}
#endif /* XINERAMA */

void
#if KEYMODES_PATCH
keydefpress(XEvent *e)
#else
keypress(XEvent *e)
#endif // KEYMODES_PATCH
{
	unsigned int i;
	KeySym keysym;
	XKeyEvent *ev;

	ev = &e->xkey;
	keysym = XKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0);
	for (i = 0; i < LENGTH(keys); i++)
		if (keysym == keys[i].keysym
		&& CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
		&& keys[i].func)
			keys[i].func(&(keys[i].arg));
}

void
killclient(const Arg *arg)
{
	#if ISPERMANENT_PATCH
	if (!selmon->sel || selmon->sel->ispermanent)
	#else
	if (!selmon->sel)
	#endif // ISPERMANENT_PATCH
		return;
	#if SYSTRAY_PATCH
	if (!sendevent(selmon->sel->win, wmatom[WMDelete], NoEventMask, wmatom[WMDelete], CurrentTime, 0, 0, 0)) {
	#else
	if (!sendevent(selmon->sel, wmatom[WMDelete])) {
	#endif // SYSTRAY_PATCH
		XGrabServer(dpy);
		XSetErrorHandler(xerrordummy);
		XSetCloseDownMode(dpy, DestroyAll);
		XKillClient(dpy, selmon->sel->win);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	selmon->pertag->prevclient[selmon->pertag->curtag] = NULL;
	#endif // SWAPFOCUS_PATCH
}

void
manage(Window w, XWindowAttributes *wa)
{
	Client *c, *t = NULL;
	#if SWALLOW_PATCH
	Client *term = NULL;
	#endif // SWALLOW_PATCH
	Window trans = None;
	XWindowChanges wc;

	c = ecalloc(1, sizeof(Client));
	c->win = w;
	#if SWALLOW_PATCH
	c->pid = winpid(w);
	#endif // SWALLOW_PATCH
	/* geometry */
	c->x = c->oldx = wa->x;
	c->y = c->oldy = wa->y;
	c->w = c->oldw = wa->width;
	c->h = c->oldh = wa->height;
	c->oldbw = wa->border_width;
	#if CFACTS_PATCH
	c->cfact = 1.0;
	#endif // CFACTS_PATCH

	updatetitle(c);
	if (XGetTransientForHint(dpy, w, &trans) && (t = wintoclient(trans))) {
		c->mon = t->mon;
		c->tags = t->tags;
	} else {
		c->mon = selmon;
		applyrules(c);
		#if SWALLOW_PATCH
		term = termforwin(c);
		#endif // SWALLOW_PATCH
	}

	if (c->x + WIDTH(c) > c->mon->mx + c->mon->mw)
		c->x = c->mon->mx + c->mon->mw - WIDTH(c);
	if (c->y + HEIGHT(c) > c->mon->my + c->mon->mh)
		c->y = c->mon->my + c->mon->mh - HEIGHT(c);
	c->x = MAX(c->x, c->mon->mx);
	/* only fix client y-offset, if the client center might cover the bar */
	c->y = MAX(c->y, ((c->mon->by == c->mon->my) && (c->x + (c->w / 2) >= c->mon->wx)
		&& (c->x + (c->w / 2) < c->mon->wx + c->mon->ww)) ? bh : c->mon->my);
	#if SETBORDERPX_PATCH
	c->bw = c->mon->borderpx;
	#else
	c->bw = borderpx;
	#endif // SETBORDERPX_PATCH

	wc.border_width = c->bw;
	XConfigureWindow(dpy, w, CWBorderWidth, &wc);
	#if FLOAT_BORDER_COLOR_PATCH
	if (c->isfloating)
		XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColBorder].pixel);
	#else
	XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColBorder].pixel);
	#endif // FLOAT_BORDER_COLOR_PATCH
	configure(c); /* propagates border_width, if size doesn't change */
	updatewindowtype(c);
	updatesizehints(c);
	updatewmhints(c);
	#if CENTER_PATCH
	if (c->iscentered) {
		c->x = c->mon->mx + (c->mon->mw - WIDTH(c)) / 2;
		c->y = c->mon->my + (c->mon->mh - HEIGHT(c)) / 2;
	}
	#endif // CENTER_PATCH
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	c->sfx = -9999;
	c->sfy = -9999;
	c->sfw = c->w;
	c->sfh = c->h;
	#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH

	XSelectInput(dpy, w, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
	grabbuttons(c, 0);
	#if MAXIMIZE_PATCH
	c->wasfloating = 0;
	c->ismax = 0;
	#elif EXRESIZE_PATCH
	c->wasfloating = 0;
	#endif // MAXIMIZE_PATCH / EXRESIZE_PATCH

	if (!c->isfloating)
		c->isfloating = c->oldstate = trans != None || c->isfixed;
	if (c->isfloating)
		XRaiseWindow(dpy, c->win);
	#if FLOAT_BORDER_COLOR_PATCH
	if (c->isfloating)
		XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColFloat].pixel);
	#endif // FLOAT_BORDER_COLOR_PATCH
	#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH
	attachx(c);
	#else
	attach(c);
	#endif
	attachstack(c);
	XChangeProperty(dpy, root, netatom[NetClientList], XA_WINDOW, 32, PropModeAppend,
		(unsigned char *) &(c->win), 1);
	XMoveResizeWindow(dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h); /* some windows require this */

	#if AWESOMEBAR_PATCH
	if (!HIDDEN(c))
		setclientstate(c, NormalState);
	#else
	setclientstate(c, NormalState);
	#endif // AWESOMEBAR_PATCH
	if (c->mon == selmon)
		unfocus(selmon->sel, 0);
	c->mon->sel = c;
	arrange(c->mon);
	#if AWESOMEBAR_PATCH
	if (!HIDDEN(c))
		XMapWindow(dpy, c->win);
	#else
	XMapWindow(dpy, c->win);
	#endif // AWESOMEBAR_PATCH
	#if SWALLOW_PATCH
	if (term)
		swallow(term, c);
	#endif // SWALLOW_PATCH
	focus(NULL);
}

void
mappingnotify(XEvent *e)
{
	XMappingEvent *ev = &e->xmapping;

	XRefreshKeyboardMapping(ev);
	if (ev->request == MappingKeyboard)
		grabkeys();
}

void
maprequest(XEvent *e)
{
	static XWindowAttributes wa;
	XMapRequestEvent *ev = &e->xmaprequest;

	#if SYSTRAY_PATCH
	Client *i;
	if (showsystray && (i = wintosystrayicon(ev->window))) {
		sendevent(i->win, netatom[Xembed], StructureNotifyMask, CurrentTime, XEMBED_WINDOW_ACTIVATE, 0, systray->win, XEMBED_EMBEDDED_VERSION);
		updatesystray();
	}
	#endif // SYSTRAY_PATCH

	if (!XGetWindowAttributes(dpy, ev->window, &wa))
		return;
	if (wa.override_redirect)
		return;
	if (!wintoclient(ev->window))
		manage(ev->window, &wa);
}

#if !FOCUSONCLICK_PATCH
void
motionnotify(XEvent *e)
{
	static Monitor *mon = NULL;
	Monitor *m;
	XMotionEvent *ev = &e->xmotion;

	if (ev->window != root)
		return;
	if ((m = recttomon(ev->x_root, ev->y_root, 1, 1)) != mon && mon) {
		unfocus(selmon->sel, 1);
		selmon = m;
		focus(NULL);
	}
	mon = m;
}
#endif // FOCUSONCLICK_PATCH

void
movemouse(const Arg *arg)
{
	int x, y, ocx, ocy, nx, ny;
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel))
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfullscreen && !c->fakefullscreen) /* no support moving fullscreen windows by mouse */
		return;
	#else
	if (c->isfullscreen) /* no support moving fullscreen windows by mouse */
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // FAKEFULLSCREEN_PATCH
	restack(selmon);
	ocx = c->x;
	ocy = c->y;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurMove]->cursor, CurrentTime) != GrabSuccess)
		return;
	if (!getrootptr(&x, &y))
		return;
	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 60))
				continue;
			lasttime = ev.xmotion.time;

			nx = ocx + (ev.xmotion.x - x);
			ny = ocy + (ev.xmotion.y - y);
			if (abs(selmon->wx - nx) < snap)
				nx = selmon->wx;
			else if (abs((selmon->wx + selmon->ww) - (nx + WIDTH(c))) < snap)
				nx = selmon->wx + selmon->ww - WIDTH(c);
			if (abs(selmon->wy - ny) < snap)
				ny = selmon->wy;
			else if (abs((selmon->wy + selmon->wh) - (ny + HEIGHT(c))) < snap)
				ny = selmon->wy + selmon->wh - HEIGHT(c);
			if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
			&& (abs(nx - c->x) > snap || abs(ny - c->y) > snap))
				togglefloating(NULL);
			if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
			#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
				resize(c, nx, ny, c->w, c->h, 1);
				/* save last known float coordinates */
				c->sfx = nx;
				c->sfy = ny;
			#else
				resize(c, nx, ny, c->w, c->h, 1);
			#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
			}
			#if ROUNDED_CORNERS_PATCH
			drawroundedcorners(c);
			#endif // ROUNDED_CORNERS_PATCH
			break;
		}
	} while (ev.type != ButtonRelease);
	XUngrabPointer(dpy, CurrentTime);
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
	#if ROUNDED_CORNERS_PATCH
	drawroundedcorners(c);
	#endif // ROUNDED_CORNERS_PATCH
}

Client *
nexttiled(Client *c)
{
	#if AWESOMEBAR_PATCH
	for (; c && (c->isfloating || !ISVISIBLE(c) || HIDDEN(c)); c = c->next);
	#else
	for (; c && (c->isfloating || !ISVISIBLE(c)); c = c->next);
	#endif // AWESOMEBAR_PATCH
	return c;
}

#if !ZOOMSWAP_PATCH || TAGINTOSTACK_ALLMASTER_PATCH || TAGINTOSTACK_ONEMASTER_PATCH
void
pop(Client *c)
{
	detach(c);
	attach(c);
	focus(c);
	arrange(c->mon);
}
#endif // !ZOOMSWAP_PATCH / TAGINTOSTACK_ALLMASTER_PATCH / TAGINTOSTACK_ONEMASTER_PATCH

void
propertynotify(XEvent *e)
{
	Client *c;
	Window trans;
	XPropertyEvent *ev = &e->xproperty;

	#if SYSTRAY_PATCH
	if (showsystray && (c = wintosystrayicon(ev->window))) {
		if (ev->atom == XA_WM_NORMAL_HINTS) {
			updatesizehints(c);
			updatesystrayicongeom(c, c->w, c->h);
		}
		else
			updatesystrayiconstate(c, ev);
		updatesystray();
	}
	#endif // SYSTRAY_PATCH

	if ((ev->window == root) && (ev->atom == XA_WM_NAME)) {
		#if DWMC_PATCH || FSIGNAL_PATCH
		if (!fake_signal())
			updatestatus();
		#else
		updatestatus();
		#endif // DWMC_PATCH / FSIGNAL_PATCH
	} else if (ev->state == PropertyDelete) {
		return; /* ignore */
	} else if ((c = wintoclient(ev->window))) {
		switch(ev->atom) {
		default: break;
		case XA_WM_TRANSIENT_FOR:
			if (!c->isfloating && (XGetTransientForHint(dpy, c->win, &trans)) &&
				(c->isfloating = (wintoclient(trans)) != NULL))
				arrange(c->mon);
			break;
		case XA_WM_NORMAL_HINTS:
			updatesizehints(c);
			break;
		case XA_WM_HINTS:
			updatewmhints(c);
			drawbars();
			#if URGENTBORDER_PATCH
			if (c->isurgent)
				XSetWindowBorder(dpy, c->win, scheme[SchemeUrg][ColBorder].pixel);
			#endif // URGENTBORDER_PATCH
			break;
		}
		if (ev->atom == XA_WM_NAME || ev->atom == netatom[NetWMName]) {
			updatetitle(c);
			if (c == c->mon->sel)
				drawbar(c->mon);
		}
		if (ev->atom == netatom[NetWMWindowType])
			updatewindowtype(c);
	}
}

void
quit(const Arg *arg)
{
	#if ONLYQUITONEMPTY_PATCH
	unsigned int n;
	Window *junk = malloc(1);

	XQueryTree(dpy, root, junk, junk, &junk, &n);

	if (n <= quit_empty_window_count) {
		#if RESTARTSIG_PATCH
		if (arg->i)
			restart = 1;
		#endif // RESTARTSIG_PATCH
		running = 0;
	}
	else
		printf("[dwm] not exiting (n=%d)\n", n);

	free(junk);
	#else
	#if RESTARTSIG_PATCH
	if (arg->i)
		restart = 1;
	#endif // RESTARTSIG_PATCH
	running = 0;
	#endif // ONLYQUITONEMPTY_PATCH
}

Monitor *
recttomon(int x, int y, int w, int h)
{
	Monitor *m, *r = selmon;
	int a, area = 0;

	for (m = mons; m; m = m->next)
		if ((a = INTERSECT(x, y, w, h, m)) > area) {
			area = a;
			r = m;
		}
	return r;
}

void
resize(Client *c, int x, int y, int w, int h, int interact)
{
	if (applysizehints(c, &x, &y, &w, &h, interact))
		resizeclient(c, x, y, w, h);
}

void
resizeclient(Client *c, int x, int y, int w, int h)
{
	XWindowChanges wc;

	c->oldx = c->x; c->x = wc.x = x;
	c->oldy = c->y; c->y = wc.y = y;
	c->oldw = c->w; c->w = wc.width = w;
	c->oldh = c->h; c->h = wc.height = h;
	#if EXRESIZE_PATCH
	c->expandmask = 0;
	#endif // EXRESIZE_PATCH
	wc.border_width = c->bw;
	#if NOBORDER_PATCH
	if (((nexttiled(c->mon->clients) == c && !nexttiled(c->next))
		#if MONOCLE_LAYOUT
	    || &monocle == c->mon->lt[c->mon->sellt]->arrange
	    #endif // MONOCLE_LAYOUT
	    ) && !c->isfullscreen && !c->isfloating
	    && c->mon->lt[c->mon->sellt]->arrange) {
		c->w = wc.width += c->bw * 2;
		c->h = wc.height += c->bw * 2;
		wc.border_width = 0;
	}
	#endif // NOBORDER_PATCH
	XConfigureWindow(dpy, c->win, CWX|CWY|CWWidth|CWHeight|CWBorderWidth, &wc);
	configure(c);
	XSync(dpy, False);
}

void
resizemouse(const Arg *arg)
{
	int ocx, ocy, nw, nh;
	#if RESIZECORNERS_PATCH
	int ocx2, ocy2, nx, ny;
	int horizcorner, vertcorner;
	int di;
	unsigned int dui;
	Window dummy;
	#endif // RESIZECORNERS_PATCH
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel))
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfullscreen && !c->fakefullscreen) /* no support resizing fullscreen windows by mouse */
		return;
	#else
	if (c->isfullscreen) /* no support resizing fullscreen windows by mouse */
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // !FAKEFULLSCREEN_PATCH
	restack(selmon);
	ocx = c->x;
	ocy = c->y;
	#if RESIZECORNERS_PATCH
	ocx2 = c->x + c->w;
	ocy2 = c->y + c->h;
	#endif // RESIZECORNERS_PATCH
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurResize]->cursor, CurrentTime) != GrabSuccess)
		return;
	#if RESIZECORNERS_PATCH
	if (!XQueryPointer (dpy, c->win, &dummy, &dummy, &di, &di, &nx, &ny, &dui))
		return;
	horizcorner = nx < c->w / 2;
	vertcorner = ny < c->h / 2;
	XWarpPointer (dpy, None, c->win, 0, 0, 0, 0,
			horizcorner ? (-c->bw) : (c->w + c->bw - 1),
			vertcorner ? (-c->bw) : (c->h + c->bw - 1));
	#else
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
	#endif // RESIZECORNERS_PATCH
	do {
		XMaskEvent(dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
		switch(ev.type) {
		case ConfigureRequest:
		case Expose:
		case MapRequest:
			handler[ev.type](&ev);
			break;
		case MotionNotify:
			if ((ev.xmotion.time - lasttime) <= (1000 / 60))
				continue;
			lasttime = ev.xmotion.time;

			nw = MAX(ev.xmotion.x - ocx - 2 * c->bw + 1, 1);
			nh = MAX(ev.xmotion.y - ocy - 2 * c->bw + 1, 1);
			#if RESIZECORNERS_PATCH
			nx = horizcorner ? ev.xmotion.x : c->x;
			ny = vertcorner ? ev.xmotion.y : c->y;
			nw = MAX(horizcorner ? (ocx2 - nx) : (ev.xmotion.x - ocx - 2 * c->bw + 1), 1);
			nh = MAX(vertcorner ? (ocy2 - ny) : (ev.xmotion.y - ocy - 2 * c->bw + 1), 1);
			#endif // RESIZECORNERS_PATCH
			if (c->mon->wx + nw >= selmon->wx && c->mon->wx + nw <= selmon->wx + selmon->ww
			&& c->mon->wy + nh >= selmon->wy && c->mon->wy + nh <= selmon->wy + selmon->wh)
			{
				if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
				&& (abs(nw - c->w) > snap || abs(nh - c->h) > snap))
					togglefloating(NULL);
			}
			if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
				#if RESIZECORNERS_PATCH
				resize(c, nx, ny, nw, nh, 1);
				#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
				/* save last known float dimensions */
				c->sfx = nx;
				c->sfy = ny;
				c->sfw = nw;
				c->sfh = nh;
				#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
				#else
				resize(c, c->x, c->y, nw, nh, 1);
				#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
				c->sfx = c->x;
				c->sfy = c->y;
				c->sfw = nw;
				c->sfh = nh;
				#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
				#endif // RESIZECORNERS_PATCH
				#if ROUNDED_CORNERS_PATCH
				drawroundedcorners(c);
				#endif // ROUNDED_CORNERS_PATCH
			}
			break;
		}
	} while (ev.type != ButtonRelease);
	#if RESIZECORNERS_PATCH
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0,
			horizcorner ? (-c->bw) : (c->w + c->bw - 1),
			vertcorner ? (-c->bw) : (c->h + c->bw - 1));
	#else
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
	#endif // RESIZECORNERS_PATCH
	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
}

void
restack(Monitor *m)
{
	Client *c;
	XEvent ev;
	XWindowChanges wc;

	drawbar(m);
	if (!m->sel)
		return;
	if (m->sel->isfloating || !m->lt[m->sellt]->arrange)
		XRaiseWindow(dpy, m->sel->win);
	if (m->lt[m->sellt]->arrange) {
		wc.stack_mode = Below;
		wc.sibling = m->barwin;
		for (c = m->stack; c; c = c->snext)
			if (!c->isfloating && ISVISIBLE(c)) {
				XConfigureWindow(dpy, c->win, CWSibling|CWStackMode, &wc);
				wc.sibling = c->win;
			}
	}
	XSync(dpy, False);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
	#if WARP_PATCH
	if (m == selmon && (m->tagset[m->seltags] & m->sel->tags) && selmon->lt[selmon->sellt] != &layouts[MONOCLE_LAYOUT_POS])
		warp(m->sel);
	#endif // WARP_PATCH
}

void
run(void)
{
	XEvent ev;
	/* main event loop */
	XSync(dpy, False);
	while (running && !XNextEvent(dpy, &ev))
		if (handler[ev.type])
			handler[ev.type](&ev); /* call handler */
}

void
scan(void)
{
	unsigned int i, num;
	Window d1, d2, *wins = NULL;
	XWindowAttributes wa;

	if (XQueryTree(dpy, root, &d1, &d2, &wins, &num)) {
		for (i = 0; i < num; i++) {
			if (!XGetWindowAttributes(dpy, wins[i], &wa)
			|| wa.override_redirect || XGetTransientForHint(dpy, wins[i], &d1))
				continue;
			if (wa.map_state == IsViewable || getstate(wins[i]) == IconicState)
				manage(wins[i], &wa);
		}
		for (i = 0; i < num; i++) { /* now the transients */
			if (!XGetWindowAttributes(dpy, wins[i], &wa))
				continue;
			if (XGetTransientForHint(dpy, wins[i], &d1)
			&& (wa.map_state == IsViewable || getstate(wins[i]) == IconicState))
				manage(wins[i], &wa);
		}
		XFree(wins);
	}
}

void
sendmon(Client *c, Monitor *m)
{
	#if EXRESIZE_PATCH
	Monitor *oldm = selmon;
	#endif // EXRESIZE_PATCH
	if (c->mon == m)
		return;
	unfocus(c, 1);
	detach(c);
	detachstack(c);
	c->mon = m;
	#if EMPTYVIEW_PATCH
	c->tags = (m->tagset[m->seltags] ? m->tagset[m->seltags] : 1);
	#else
	c->tags = m->tagset[m->seltags]; /* assign tags of target monitor */
	#endif // EMPTYVIEW_PATCH
	#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH
	attachx(c);
	#else
	attach(c);
	#endif
	attachstack(c);
	#if EXRESIZE_PATCH
	if (oldm != m)
		arrange(oldm);
	arrange(m);
	focus(c);
	restack(m);
	#else
	focus(NULL);
	arrange(NULL);
	#endif // EXRESIZE_PATCH
	#if SWITCHTAG_PATCH
	if (c->switchtag)
		c->switchtag = 0;
	#endif // SWITCHTAG_PATCH
}

void
setclientstate(Client *c, long state)
{
	long data[] = { state, None };

	XChangeProperty(dpy, c->win, wmatom[WMState], wmatom[WMState], 32,
		PropModeReplace, (unsigned char *)data, 2);
}

int
#if SYSTRAY_PATCH
sendevent(Window w, Atom proto, int mask, long d0, long d1, long d2, long d3, long d4)
#else
sendevent(Client *c, Atom proto)
#endif // SYSTRAY_PATCH
{
	int n;
	Atom *protocols;
	#if SYSTRAY_PATCH
	Atom mt;
	#endif // SYSTRAY_PATCH
	int exists = 0;
	XEvent ev;

	#if SYSTRAY_PATCH
	if (proto == wmatom[WMTakeFocus] || proto == wmatom[WMDelete]) {
		mt = wmatom[WMProtocols];
		if (XGetWMProtocols(dpy, w, &protocols, &n)) {
			while (!exists && n--)
				exists = protocols[n] == proto;
			XFree(protocols);
		}
	} else {
		exists = True;
		mt = proto;
	}
	#else
	if (XGetWMProtocols(dpy, c->win, &protocols, &n)) {
		while (!exists && n--)
			exists = protocols[n] == proto;
		XFree(protocols);
	}
	#endif // SYSTRAY_PATCH

	if (exists) {
		#if SYSTRAY_PATCH
		ev.type = ClientMessage;
		ev.xclient.window = w;
		ev.xclient.message_type = mt;
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = d0;
		ev.xclient.data.l[1] = d1;
		ev.xclient.data.l[2] = d2;
		ev.xclient.data.l[3] = d3;
		ev.xclient.data.l[4] = d4;
		XSendEvent(dpy, w, False, mask, &ev);
		#else
		ev.type = ClientMessage;
		ev.xclient.window = c->win;
		ev.xclient.message_type = wmatom[WMProtocols];
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = proto;
		ev.xclient.data.l[1] = CurrentTime;
		XSendEvent(dpy, c->win, False, NoEventMask, &ev);
		#endif // SYSTRAY_PATCH
	}
	return exists;
}

void
setfocus(Client *c)
{
	if (!c->neverfocus) {
		XSetInputFocus(dpy, c->win, RevertToPointerRoot, CurrentTime);
		XChangeProperty(dpy, root, netatom[NetActiveWindow],
			XA_WINDOW, 32, PropModeReplace,
			(unsigned char *) &(c->win), 1);
	}
	#if SYSTRAY_PATCH
	sendevent(c->win, wmatom[WMTakeFocus], NoEventMask, wmatom[WMTakeFocus], CurrentTime, 0, 0, 0);
	#else
	sendevent(c, wmatom[WMTakeFocus]);
	#endif // SYSTRAY_PATCH
}

void
setfullscreen(Client *c, int fullscreen)
{
	if (fullscreen && !c->isfullscreen) {
		XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
			PropModeReplace, (unsigned char*)&netatom[NetWMFullscreen], 1);
		c->isfullscreen = 1;
		#if !FAKEFULLSCREEN_PATCH
		#if FAKEFULLSCREEN_CLIENT_PATCH
		if (!c->fakefullscreen) {
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		c->oldstate = c->isfloating;
		c->oldbw = c->bw;
		c->bw = 0;
		c->isfloating = 1;
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
		#if FAKEFULLSCREEN_CLIENT_PATCH
		}
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		#endif // !FAKEFULLSCREEN_PATCH
	} else if (!fullscreen && c->isfullscreen){
		XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
			PropModeReplace, (unsigned char*)0, 0);
		c->isfullscreen = 0;
		#if !FAKEFULLSCREEN_PATCH
		#if FAKEFULLSCREEN_CLIENT_PATCH
		if (!c->fakefullscreen) {
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		c->isfloating = c->oldstate;
		c->bw = c->oldbw;
		c->x = c->oldx;
		c->y = c->oldy;
		c->w = c->oldw;
		c->h = c->oldh;
		resizeclient(c, c->x, c->y, c->w, c->h);
		arrange(c->mon);
		#if FAKEFULLSCREEN_CLIENT_PATCH
		}
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		#endif // !FAKEFULLSCREEN_PATCH
	}
}

void
setlayout(const Arg *arg)
{
	if (!arg || !arg->v || arg->v != selmon->lt[selmon->sellt]) {
		#if PERTAG_PATCH
		selmon->pertag->sellts[selmon->pertag->curtag] ^= 1;
		selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
		#else
		selmon->sellt ^= 1;
		#endif // PERTAG_PATCH
		#if EXRESIZE_PATCH
		if (!selmon->lt[selmon->sellt]->arrange) {
			for (Client *c = selmon->clients ; c ; c = c->next) {
				if (!c->isfloating) {
					/*restore last known float dimensions*/
					resize(c, selmon->mx + c->sfx, selmon->my + c->sfy,
					       c->sfw, c->sfh, False);
				}
			}
		}
		#endif // EXRESIZE_PATCH
	}
	if (arg && arg->v)
	#if PERTAG_PATCH
		selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt] = (Layout *)arg->v;
	selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
	#else
		selmon->lt[selmon->sellt] = (Layout *)arg->v;
	#endif // PERTAG_PATCH

	#if FLEXTILE_DELUXE_LAYOUT
	if (selmon->lt[selmon->sellt]->preset.nmaster && selmon->lt[selmon->sellt]->preset.nmaster != -1)
		selmon->nmaster = selmon->lt[selmon->sellt]->preset.nmaster;
	if (selmon->lt[selmon->sellt]->preset.nstack && selmon->lt[selmon->sellt]->preset.nstack != -1)
		selmon->nstack = selmon->lt[selmon->sellt]->preset.nstack;

	selmon->ltaxis[LAYOUT] = selmon->lt[selmon->sellt]->preset.layout;
	selmon->ltaxis[MASTER] = selmon->lt[selmon->sellt]->preset.masteraxis;
	selmon->ltaxis[STACK]  = selmon->lt[selmon->sellt]->preset.stack1axis;
	selmon->ltaxis[STACK2] = selmon->lt[selmon->sellt]->preset.stack2axis;

	#if PERTAG_PATCH
	selmon->pertag->ltaxis[selmon->pertag->curtag][LAYOUT] = selmon->ltaxis[LAYOUT];
	selmon->pertag->ltaxis[selmon->pertag->curtag][MASTER] = selmon->ltaxis[MASTER];
	selmon->pertag->ltaxis[selmon->pertag->curtag][STACK]  = selmon->ltaxis[STACK];
	selmon->pertag->ltaxis[selmon->pertag->curtag][STACK2] = selmon->ltaxis[STACK2];
	#endif // PERTAG_PATCH
	#endif // FLEXTILE_DELUXE_LAYOUT
	strncpy(selmon->ltsymbol, selmon->lt[selmon->sellt]->symbol, sizeof selmon->ltsymbol);
	if (selmon->sel)
		arrange(selmon);
	else
		drawbar(selmon);
}

/* arg > 1.0 will set mfact absolutely */
void
setmfact(const Arg *arg)
{
	float f;

	if (!arg || !selmon->lt[selmon->sellt]->arrange)
		return;
	f = arg->f < 1.0 ? arg->f + selmon->mfact : arg->f - 1.0;
	if (f < 0.05 || f > 0.95)
		return;
	#if PERTAG_PATCH
	selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag] = f;
	#else
	selmon->mfact = f;
	#endif // PERTAG_PATCH
	arrange(selmon);
}

void
setup(void)
{
	int i;
	XSetWindowAttributes wa;
	Atom utf8string;

	/* clean up any zombies immediately */
	sigchld(0);

	#if RESTARTSIG_PATCH
	signal(SIGHUP, sighup);
	signal(SIGTERM, sigterm);
	#endif // RESTARTSIG_PATCH

	/* init screen */
	screen = DefaultScreen(dpy);
	sw = DisplayWidth(dpy, screen);
	sh = DisplayHeight(dpy, screen);
	root = RootWindow(dpy, screen);
	#if ALPHA_PATCH
	xinitvisual();
	drw = drw_create(dpy, screen, root, sw, sh, visual, depth, cmap);
	#else
	drw = drw_create(dpy, screen, root, sw, sh);
	#endif // ALPHA_PATCH
	#if PANGO_PATCH
	if (!drw_font_create(drw, font))
	#else
	if (!drw_fontset_create(drw, fonts, LENGTH(fonts)))
	#endif // PANGO_PATCH
		die("no fonts could be loaded.");
	#if STATUSPADDING_PATCH && PANGO_PATCH
	lrpad = drw->font->h + horizpadbar;
	bh = drw->font->h + vertpadbar;
	#elif STATUSPADDING_PATCH
	lrpad = drw->fonts->h + horizpadbar;
	bh = drw->fonts->h + vertpadbar;
	#elif PANGO_PATCH
	lrpad = drw->font->h;
	bh = drw->font->h + 2;
	#else
	lrpad = drw->fonts->h;
	bh = drw->fonts->h + 2;
	#endif // STATUSPADDING_PATCH
	updategeom();
	#if BARPADDING_PATCH
	sp = sidepad;
	vp = (topbar == 1) ? vertpad : - vertpad;
	#endif // BARPADDING_PATCH
	/* init atoms */
	utf8string = XInternAtom(dpy, "UTF8_STRING", False);
	wmatom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
	wmatom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	wmatom[WMState] = XInternAtom(dpy, "WM_STATE", False);
	wmatom[WMTakeFocus] = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
	#if WINDOWROLERULE_PATCH
	wmatom[WMWindowRole] = XInternAtom(dpy, "WM_WINDOW_ROLE", False);
	#endif // WINDOWROLERULE_PATCH
	netatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
	netatom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
	#if SYSTRAY_PATCH
	netatom[NetSystemTray] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_S0", False);
	netatom[NetSystemTrayOP] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_OPCODE", False);
	netatom[NetSystemTrayOrientation] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_ORIENTATION", False);
	netatom[NetSystemTrayOrientationHorz] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_ORIENTATION_HORZ", False);
	netatom[NetSystemTrayVisual] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_VISUAL", False);
	netatom[NetWMWindowTypeDock] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
	xatom[Manager] = XInternAtom(dpy, "MANAGER", False);
	xatom[Xembed] = XInternAtom(dpy, "_XEMBED", False);
	xatom[XembedInfo] = XInternAtom(dpy, "_XEMBED_INFO", False);
	#endif // SYSTRAY_PATCH
	#if EWMHTAGS_PATCH
	netatom[NetDesktopViewport] = XInternAtom(dpy, "_NET_DESKTOP_VIEWPORT", False);
	netatom[NetNumberOfDesktops] = XInternAtom(dpy, "_NET_NUMBER_OF_DESKTOPS", False);
	netatom[NetCurrentDesktop] = XInternAtom(dpy, "_NET_CURRENT_DESKTOP", False);
	netatom[NetDesktopNames] = XInternAtom(dpy, "_NET_DESKTOP_NAMES", False);
	#endif // EWMHTAGS_PATCH
	netatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
	netatom[NetWMState] = XInternAtom(dpy, "_NET_WM_STATE", False);
	netatom[NetWMCheck] = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);
	netatom[NetWMFullscreen] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
	#if EWMH_WINDOWS_FLOAT_PATCH
	netatom[NetWMModal] = XInternAtom(dpy, "_NET_WM_STATE_MODAL", False);
	#endif // EWMH_WINDOWS_FLOAT_PATCH
	netatom[NetWMWindowType] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	netatom[NetWMWindowTypeDialog] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
	#if EWMH_WINDOWS_FLOAT_PATCH
	netatom[NetWMWindowTypeUtility] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_UTILITY", False);
	netatom[NetWMWindowTypeToolbar] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
	netatom[NetWMWindowTypeSplash] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_SPLASH", False);
	#endif // EWMH_WINDOWS_FLOAT_PATCH
	netatom[NetClientList] = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
	/* init cursors */
	cursor[CurNormal] = drw_cur_create(drw, XC_left_ptr);
	cursor[CurResize] = drw_cur_create(drw, XC_sizing);
	cursor[CurMove] = drw_cur_create(drw, XC_fleur);
	/* init appearance */
	#if VTCOLORS_PATCH
	get_vt_colors();
	if (get_luminance(colors[SchemeTagsNorm][ColBg]) > 50) {
		strcpy(colors[SchemeTitleNorm][ColBg], title_bg_light);
		strcpy(colors[SchemeTitleSel][ColBg], title_bg_light);
	} else {
		strcpy(colors[SchemeTitleNorm][ColBg], title_bg_dark);
		strcpy(colors[SchemeTitleSel][ColBg], title_bg_dark);
	}
	#endif // VTCOLORS_PATCH
	#if STATUS2D_PATCH && !STATUSCOLORS_PATCH
	scheme = ecalloc(LENGTH(colors) + 1, sizeof(Clr *));
	#if ALPHA_PATCH
	scheme[LENGTH(colors)] = drw_scm_create(drw, colors[0], alphas[0], ColCount);
	#else
	scheme[LENGTH(colors)] = drw_scm_create(drw, colors[0], ColCount);
	#endif // ALPHA_PATCH | FLOAT_BORDER_COLOR_PATCH
	#else
	scheme = ecalloc(LENGTH(colors), sizeof(Clr *));
	#endif // STATUS2D_PATCH
	for (i = 0; i < LENGTH(colors); i++)
		#if ALPHA_PATCH
		scheme[i] = drw_scm_create(drw, colors[i], alphas[i], ColCount);
		#else
		scheme[i] = drw_scm_create(drw, colors[i], ColCount);
		#endif // ALPHA_PATCH
	#if SYSTRAY_PATCH
	/* init system tray */
	if (showsystray)
		updatesystray();
	#endif // SYSTRAY_PATCH
	/* init bars */
	updatebars();
	updatestatus();
	#if BARPADDING_PATCH
	updatebarpos(selmon);
	#endif // BARPADDING_PATCH
	/* supporting window for NetWMCheck */
	wmcheckwin = XCreateSimpleWindow(dpy, root, 0, 0, 1, 1, 0, 0, 0);
	XChangeProperty(dpy, wmcheckwin, netatom[NetWMCheck], XA_WINDOW, 32,
		PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	XChangeProperty(dpy, wmcheckwin, netatom[NetWMName], utf8string, 8,
		PropModeReplace, (unsigned char *) "dwm", 3);
	XChangeProperty(dpy, root, netatom[NetWMCheck], XA_WINDOW, 32,
		PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	/* EWMH support per view */
	XChangeProperty(dpy, root, netatom[NetSupported], XA_ATOM, 32,
		PropModeReplace, (unsigned char *) netatom, NetLast);
	#if EWMHTAGS_PATCH
	setnumdesktops();
	setcurrentdesktop();
	setdesktopnames();
	setviewport();
	#endif // EWMHTAGS_PATCH
	XDeleteProperty(dpy, root, netatom[NetClientList]);
	/* select events */
	wa.cursor = cursor[CurNormal]->cursor;
	wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask
		|ButtonPressMask|PointerMotionMask|EnterWindowMask
		|LeaveWindowMask|StructureNotifyMask|PropertyChangeMask;
	XChangeWindowAttributes(dpy, root, CWEventMask|CWCursor, &wa);
	XSelectInput(dpy, root, wa.event_mask);
	grabkeys();
	focus(NULL);
}


void
seturgent(Client *c, int urg)
{
	XWMHints *wmh;

	c->isurgent = urg;
	if (!(wmh = XGetWMHints(dpy, c->win)))
		return;
	wmh->flags = urg ? (wmh->flags | XUrgencyHint) : (wmh->flags & ~XUrgencyHint);
	XSetWMHints(dpy, c->win, wmh);
	XFree(wmh);
}

void
showhide(Client *c)
{
	if (!c)
		return;
	if (ISVISIBLE(c)) {
		#if SCRATCHPADS_PATCH && !SCRATCHPAD_KEEP_POSITION_AND_SIZE_PATCH
		if ((c->tags & SPTAGMASK) && c->isfloating) {
			c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
			c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
		}
		#endif // SCRATCHPADS_PATCH | SCRATCHPAD_KEEP_POSITION_AND_SIZE_PATCH
		/* show clients top down */
		#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
		if (!c->mon->lt[c->mon->sellt]->arrange && c->sfx != -9999 && !c->isfullscreen) {
			XMoveWindow(dpy, c->win, c->sfx, c->sfy);
			resize(c, c->sfx, c->sfy, c->sfw, c->sfh, 0);
			showhide(c->snext);
			return;
		}
		#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
		#if AUTORESIZE_PATCH
		if (c->needresize) {
			c->needresize = 0;
			XMoveResizeWindow(dpy, c->win, c->x, c->y, c->w, c->h);
		} else {
			XMoveWindow(dpy, c->win, c->x, c->y);
		}
		#else
		XMoveWindow(dpy, c->win, c->x, c->y);
		#endif // AUTORESIZE_PATCH
		if ((!c->mon->lt[c->mon->sellt]->arrange || c->isfloating)
			#if !FAKEFULLSCREEN_PATCH
			&& !c->isfullscreen
			#endif // !FAKEFULLSCREEN_PATCH
			)
			resize(c, c->x, c->y, c->w, c->h, 0);
		showhide(c->snext);
	} else {
		/* hide clients bottom up */
		showhide(c->snext);
		XMoveWindow(dpy, c->win, WIDTH(c) * -2, c->y);
	}
}

void
sigchld(int unused)
{
	if (signal(SIGCHLD, sigchld) == SIG_ERR)
		die("can't install SIGCHLD handler:");
	while (0 < waitpid(-1, NULL, WNOHANG));
}

void
spawn(const Arg *arg)
{
	#if STATUSCMD_PATCH && !DWMBLOCKS_PATCH
	char *cmd = NULL;
	#endif // STATUSCMD_PATCH | DWMBLOCKS_PATCH
	#if !NODMENU_PATCH
	if (arg->v == dmenucmd)
		dmenumon[0] = '0' + selmon->num;
	#endif // NODMENU_PATCH
	#if STATUSCMD_PATCH && !DWMBLOCKS_PATCH
	#if !NODMENU_PATCH
	else if (arg->v == statuscmd)
	#else
	if (arg->v == statuscmd)
	#endif // NODMENU_PATCH
	{
		int len = strlen(statuscmds[statuscmdn]) + 1;
		if (!(cmd = malloc(sizeof(char)*len + sizeof(statusexport))))
			die("malloc:");
		strcpy(cmd, statusexport);
		strcat(cmd, statuscmds[statuscmdn]);
		cmd[LENGTH(statusexport)-3] = '0' + lastbutton;
		statuscmd[2] = cmd;
	}
	#endif // STATUSCMD_PATCH | DWMBLOCKS_PATCH

	if (fork() == 0) {
		if (dpy)
			close(ConnectionNumber(dpy));
		#if SPAWNCMD_PATCH
		if (selmon->sel) {
			const char* const home = getenv("HOME");
			assert(home && strchr(home, '/'));
			const size_t homelen = strlen(home);
			char *cwd, *pathbuf = NULL;
			struct stat statbuf;

			cwd = strtok(selmon->sel->name, SPAWN_CWD_DELIM);
			/* NOTE: strtok() alters selmon->sel->name in-place,
			 * but that does not matter because we are going to
			 * exec() below anyway; nothing else will use it */
			while (cwd) {
				if (*cwd == '~') { /* replace ~ with $HOME */
					if (!(pathbuf = malloc(homelen + strlen(cwd)))) /* ~ counts for NULL term */
						die("fatal: could not malloc() %u bytes\n", homelen + strlen(cwd));
					strcpy(strcpy(pathbuf, home) + homelen, cwd + 1);
					cwd = pathbuf;
				}

				if (strchr(cwd, '/') && !stat(cwd, &statbuf)) {
					if (!S_ISDIR(statbuf.st_mode))
						cwd = dirname(cwd);

					if (!chdir(cwd))
						break;
				}

				cwd = strtok(NULL, SPAWN_CWD_DELIM);
			}

			free(pathbuf);
		}
		#endif // SPAWNCMD_PATCH
		setsid();
		execvp(((char **)arg->v)[0], (char **)arg->v);
		fprintf(stderr, "dwm: execvp %s", ((char **)arg->v)[0]);
		perror(" failed");
		exit(EXIT_SUCCESS);
	}
	#if STATUSCMD_PATCH && !DWMBLOCKS_PATCH
	free(cmd);
	#endif // STATUSCMD_PATCH | DWMBLOCKS_PATCH
}

void
tag(const Arg *arg)
{
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	unsigned int tagmask, tagindex;
	#endif // SWAPFOCUS_PATCH

	if (selmon->sel && arg->ui & TAGMASK) {
		selmon->sel->tags = arg->ui & TAGMASK;
		#if SWITCHTAG_PATCH
		if (selmon->sel->switchtag)
			selmon->sel->switchtag = 0;
		#endif // SWITCHTAG_PATCH
		focus(NULL);
		#if SWAPFOCUS_PATCH && PERTAG_PATCH
		selmon->pertag->prevclient[selmon->pertag->curtag] = NULL;
		for (tagmask = arg->ui & TAGMASK, tagindex = 1; tagmask!=0; tagmask >>= 1, tagindex++)
			if (tagmask & 1)
				selmon->pertag->prevclient[tagindex] = NULL;
		#endif // SWAPFOCUS_PATCH
		arrange(selmon);
		#if VIEWONTAG_PATCH
		view(arg);
		#endif // VIEWONTAG_PATCH
	}
}

void
tagmon(const Arg *arg)
{
	#if TAGMONFIXFS_PATCH
	Client *c = selmon->sel;
	if (!c || !mons->next)
		return;
	if (c->isfullscreen) {
		c->isfullscreen = 0;
		sendmon(c, dirtomon(arg->i));
		c->isfullscreen = 1;
		#if FAKEFULLSCREEN_CLIENT_PATCH
		if (!c->fakefullscreen) {
			resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
			XRaiseWindow(dpy, c->win);
		}
		#elif !FAKEFULLSCREEN_PATCH
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
	} else
		sendmon(c, dirtomon(arg->i));
	#else
	if (!selmon->sel || !mons->next)
		return;
	sendmon(selmon->sel, dirtomon(arg->i));
	#endif // TAGMONFIXFS_PATCH
}

void
togglebar(const Arg *arg)
{
	#if PERTAG_PATCH && PERTAGBAR_PATCH
	selmon->showbar = selmon->pertag->showbars[selmon->pertag->curtag] = !selmon->showbar;
	#else
	selmon->showbar = !selmon->showbar;
	#endif // PERTAG_PATCH
	updatebarpos(selmon);
	#if BARPADDING_PATCH
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx + sp, selmon->by + vp, selmon->ww - 2*sp, bh);
	#else
	XMoveResizeWindow(dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
	#endif // BARPADDING_PATCH
	#if EXTRABAR_PATCH
	#if BARPADDING_PATCH
	XMoveResizeWindow(dpy, selmon->extrabarwin, selmon->wx + sp, selmon->eby - vp, selmon->ww - 2*sp, bh);
	#else
	XMoveResizeWindow(dpy, selmon->extrabarwin, selmon->wx, selmon->eby, selmon->ww, bh);
	#endif // BARPADDING_PATCH
	#endif // EXTRABAR_PATCH
	#if SYSTRAY_PATCH
	if (showsystray) {
		XWindowChanges wc;
		if (!selmon->showbar)
			wc.y = -bh;
		else if (selmon->showbar) {
			wc.y = 0;
			if (!selmon->topbar)
				wc.y = selmon->mh - bh;
		}
		XConfigureWindow(dpy, systray->win, CWY, &wc);
	}
	#endif // SYSTRAY_PATCH
	arrange(selmon);
}

void
togglefloating(const Arg *arg)
{
	if (!selmon->sel)
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (selmon->sel->isfullscreen && !selmon->sel->fakefullscreen) /* no support for fullscreen windows */
		return;
	#else
	if (selmon->sel->isfullscreen) /* no support for fullscreen windows */
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // !FAKEFULLSCREEN_PATCH
	selmon->sel->isfloating = !selmon->sel->isfloating || selmon->sel->isfixed;
	#if FLOAT_BORDER_COLOR_PATCH
	if (selmon->sel->isfloating)
		XSetWindowBorder(dpy, selmon->sel->win, scheme[SchemeSel][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, selmon->sel->win, scheme[SchemeSel][ColBorder].pixel);
	#endif // FLOAT_BORDER_COLOR_PATCH
	if (selmon->sel->isfloating) {
		#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
		if (selmon->sel->sfx != -9999) {
			/* restore last known float dimensions */
			resize(selmon->sel, selmon->sel->sfx, selmon->sel->sfy,
			       selmon->sel->sfw, selmon->sel->sfh, 0);
			arrange(selmon);
			return;
		}
		#endif // SAVEFLOATS_PATCH // EXRESIZE_PATCH
		resize(selmon->sel, selmon->sel->x, selmon->sel->y,
			selmon->sel->w, selmon->sel->h, 0);
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	} else {
		/* save last known float dimensions */
		selmon->sel->sfx = selmon->sel->x;
		selmon->sel->sfy = selmon->sel->y;
		selmon->sel->sfw = selmon->sel->w;
		selmon->sel->sfh = selmon->sel->h;
	#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
	}
	arrange(selmon);
}

void
toggletag(const Arg *arg)
{
	unsigned int newtags;
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	unsigned int tagmask, tagindex;
	#endif // SWAPFOCUS_PATCH

	if (!selmon->sel)
		return;
	newtags = selmon->sel->tags ^ (arg->ui & TAGMASK);
	if (newtags) {
		selmon->sel->tags = newtags;
		focus(NULL);
		#if SWAPFOCUS_PATCH && PERTAG_PATCH
		for (tagmask = arg->ui & TAGMASK, tagindex = 1; tagmask!=0; tagmask >>= 1, tagindex++)
			if (tagmask & 1)
				selmon->pertag->prevclient[tagindex] = NULL;
		#endif // SWAPFOCUS_PATCH
		arrange(selmon);
	}
	#if EWMHTAGS_PATCH
	updatecurrentdesktop();
	#endif // EWMHTAGS_PATCH
}

void
toggleview(const Arg *arg)
{
	unsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);
	#if PERTAG_PATCH
	int i;
	#endif // PERTAG_PATCH

	#if TAGINTOSTACK_ALLMASTER_PATCH
	Client *const selected = selmon->sel;

	// clients in the master area should be the same after we add a new tag
	Client **const masters = calloc(selmon->nmaster, sizeof(Client *));
	if (!masters) {
		die("fatal: could not calloc() %u bytes \n", selmon->nmaster * sizeof(Client *));
	}
	// collect (from last to first) references to all clients in the master area
	Client *c;
	size_t j;
	for (c = nexttiled(selmon->clients), j = 0; c && j < selmon->nmaster; c = nexttiled(c->next), ++j)
		masters[selmon->nmaster - (j + 1)] = c;
	// put the master clients at the front of the list
	// > go from the 'last' master to the 'first'
	for (j = 0; j < selmon->nmaster; ++j)
		if (masters[j])
			pop(masters[j]);
	free(masters);

	// we also want to be sure not to mutate the focus
	focus(selected);
	#elif TAGINTOSTACK_ONEMASTER_PATCH
	// the first visible client should be the same after we add a new tag
	// we also want to be sure not to mutate the focus
	Client *const c = nexttiled(selmon->clients);
	if (c) {
		Client * const selected = selmon->sel;
		pop(c);
		focus(selected);
	}
	#endif // TAGINTOSTACK_ALLMASTER_PATCH / TAGINTOSTACK_ONEMASTER_PATCH

	#if !EMPTYVIEW_PATCH
	if (newtagset) {
	#endif // EMPTYVIEW_PATCH
		selmon->tagset[selmon->seltags] = newtagset;

		#if PERTAG_PATCH
		if (newtagset == ~0) {
			selmon->pertag->prevtag = selmon->pertag->curtag;
			selmon->pertag->curtag = 0;
		}
		/* test if the user did not select the same tag */
		if (!(newtagset & 1 << (selmon->pertag->curtag - 1))) {
			selmon->pertag->prevtag = selmon->pertag->curtag;
			for (i=0; !(newtagset & 1 << i); i++) ;
			selmon->pertag->curtag = i + 1;
		}

		/* apply settings for this view */
		selmon->nmaster = selmon->pertag->nmasters[selmon->pertag->curtag];
		selmon->mfact = selmon->pertag->mfacts[selmon->pertag->curtag];
		selmon->sellt = selmon->pertag->sellts[selmon->pertag->curtag];
		selmon->lt[selmon->sellt] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt];
		selmon->lt[selmon->sellt^1] = selmon->pertag->ltidxs[selmon->pertag->curtag][selmon->sellt^1];
		#if PERTAGBAR_PATCH
		if (selmon->showbar != selmon->pertag->showbars[selmon->pertag->curtag])
			togglebar(NULL);
		#endif // PERTAGBAR_PATCH
		#endif // PERTAG_PATCH
		focus(NULL);
		arrange(selmon);
	#if !EMPTYVIEW_PATCH
	}
	#endif // EMPTYVIEW_PATCH
	#if EWMHTAGS_PATCH
	updatecurrentdesktop();
	#endif // EWMHTAGS_PATCH
}

void
unfocus(Client *c, int setfocus)
{
	if (!c)
		return;
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	selmon->pertag->prevclient[selmon->pertag->curtag] = c;
	#endif // SWAPFOCUS_PATCH
	#if LOSEFULLSCREEN_PATCH
	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfullscreen && !c->fakefullscreen && ISVISIBLE(c))
		setfullscreen(c, 0);
	#else
	if (c->isfullscreen && ISVISIBLE(c))
		setfullscreen(c, 0);
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // LOSEFULLSCREEN_PATCH
	grabbuttons(c, 0);
	#if FLOAT_BORDER_COLOR_PATCH
	if (c->isfloating)
		XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColBorder].pixel);
	#else
	XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColBorder].pixel);
	#endif // FLOAT_BORDER_COLOR_PATCH
	if (setfocus) {
		XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
		XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
	}
}

void
unmanage(Client *c, int destroyed)
{
	Monitor *m = c->mon;
	XWindowChanges wc;

	#if SWALLOW_PATCH
	if (c->swallowing) {
		unswallow(c);
		return;
	}

	Client *s = swallowingclient(c->win);
	if (s) {
		free(s->swallowing);
		s->swallowing = NULL;
		arrange(m);
		focus(NULL);
		return;
	}
	#endif // SWALLOW_PATCH

	detach(c);
	detachstack(c);
	if (!destroyed) {
		wc.border_width = c->oldbw;
		XGrabServer(dpy); /* avoid race conditions */
		XSetErrorHandler(xerrordummy);
		XConfigureWindow(dpy, c->win, CWBorderWidth, &wc); /* restore border */
		XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
		setclientstate(c, WithdrawnState);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}
	free(c);
	#if SWALLOW_PATCH
	if (s)
		return;
	#endif // SWALLOW_PATCH
	focus(NULL);
	updateclientlist();
	arrange(m);
	#if SWITCHTAG_PATCH
	if (c->switchtag)
		view(&((Arg) { .ui = c->switchtag }));
	#endif // SWITCHTAG_PATCH
}

void
unmapnotify(XEvent *e)
{
	Client *c;
	XUnmapEvent *ev = &e->xunmap;

	if ((c = wintoclient(ev->window))) {
		if (ev->send_event)
			setclientstate(c, WithdrawnState);
		else
			unmanage(c, 0);
	#if SYSTRAY_PATCH
	} else if (showsystray && (c = wintosystrayicon(ev->window))) {
		/* KLUDGE! sometimes icons occasionally unmap their windows, but do
		 * _not_ destroy them. We map those windows back */
		XMapRaised(dpy, c->win);
		removesystrayicon(c);
		updatesystray();
	#endif // SYSTRAY_PATCH
	}
}

void
updatebars(void)
{
	unsigned int w;
	Monitor *m;
	XSetWindowAttributes wa = {
		.override_redirect = True,
		#if ALPHA_PATCH
		.background_pixel = 0,
		.border_pixel = 0,
		.colormap = cmap,
		#else
		.background_pixmap = ParentRelative,
		#endif // ALPHA_PATCH
		.event_mask = ButtonPressMask|ExposureMask
	};
	XClassHint ch = {"dwm", "dwm"};
	for (m = mons; m; m = m->next) {
		#if EXTRABAR_PATCH
		if (!m->barwin) {
		#else
		if (m->barwin)
			continue;
		#endif // EXTRABAR_PATCH
		w = m->ww;
		#if SYSTRAY_PATCH
		if (showsystray && m == systraytomon(m))
			w -= getsystraywidth();
		#endif // SYSTRAY_PATCH
		#if ALPHA_PATCH
		#if BARPADDING_PATCH
		m->barwin = XCreateWindow(dpy, root, m->wx + sp, m->by + vp, w - 2*sp, bh, 0, depth,
		                          InputOutput, visual,
		                          CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
		#else
		m->barwin = XCreateWindow(dpy, root, m->wx, m->by, w, bh, 0, depth,
		                          InputOutput, visual,
		                          CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
		#endif // BARPADDING_PATCH
		#else
		#if BARPADDING_PATCH
		m->barwin = XCreateWindow(dpy, root, m->wx + sp, m->by + vp, w - 2*sp, bh, 0, DefaultDepth(dpy, screen),
				CopyFromParent, DefaultVisual(dpy, screen),
				CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
		#else
		m->barwin = XCreateWindow(dpy, root, m->wx, m->by, w, bh, 0, DefaultDepth(dpy, screen),
				CopyFromParent, DefaultVisual(dpy, screen),
				CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
		#endif // BARPADDING_PATCH
		#endif // ALPHA_PATCH
		XDefineCursor(dpy, m->barwin, cursor[CurNormal]->cursor);
		#if SYSTRAY_PATCH
		if (showsystray && m == systraytomon(m))
			XMapRaised(dpy, systray->win);
		#endif // SYSTRAY_PATCH
		XMapRaised(dpy, m->barwin);
		XSetClassHint(dpy, m->barwin, &ch);
		#if EXTRABAR_PATCH
		}
		if (!m->extrabarwin) {
			#if ALPHA_PATCH
			#if BARPADDING_PATCH
			m->extrabarwin = XCreateWindow(dpy, root, m->wx + sp, m->eby - vp, m->ww - 2*sp, bh, 0, depth,
			                          InputOutput, visual,
			                          CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
			#else
			m->extrabarwin = XCreateWindow(dpy, root, m->wx, m->eby, m->ww, bh, 0, depth,
			                          InputOutput, visual,
			                          CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
			#endif // BARPADDING_PATCH
			#elif BARPADDING_PATCH
			m->extrabarwin = XCreateWindow(dpy, root, m->wx + sp, m->eby - vp, m->ww - 2*sp, bh, 0, DefaultDepth(dpy, screen),
					CopyFromParent, DefaultVisual(dpy, screen),
					CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
			#else
			m->extrabarwin = XCreateWindow(dpy, root, m->wx, m->eby, m->ww, bh, 0, DefaultDepth(dpy, screen),
					CopyFromParent, DefaultVisual(dpy, screen),
					CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
			#endif // ALPHA_PATCH
			XDefineCursor(dpy, m->extrabarwin, cursor[CurNormal]->cursor);
			XMapRaised(dpy, m->extrabarwin);
			XSetClassHint(dpy, m->extrabarwin, &ch);
		}
		#endif // EXTRABAR_PATCH

	}
}

void
updatebarpos(Monitor *m)
{
	m->wy = m->my;
	m->wh = m->mh;
	#if EXTRABAR_PATCH
	#if BARPADDING_PATCH
	m->wh = m->wh - vertpad * m->showbar * 2 - bh * m->showbar * 2;
	m->wy = m->showbar ? m->wy + bh + vertpad: m->wy;
	if (m->showbar) {
		m->by = m->topbar ? m->wy - bh - vertpad: m->wy + m->wh + vertpad;
		m->eby = m->topbar ? m->wy + m->wh + vertpad: m->wy - bh - vertpad;
	} else {
		m->by = -bh - vertpad;
		m->eby = -bh - vertpad;
	}
	#else
	m->wh = m->wh - bh * m->showbar * 2;
	m->wy = m->showbar ? m->wy + bh : m->wy;
	if (m->showbar) {
		m->by = m->topbar ? m->wy - bh : m->wy + m->wh;
		m->eby = m->topbar ? m->wy + m->wh : m->wy - bh;
	} else {
		m->by = -bh;
		m->eby = -bh;
	}
	#endif // BARPADDING_PATCH
	#elif BARPADDING_PATCH
	if (m->showbar) {
		m->wh = m->wh - vertpad - bh;
		m->by = m->topbar ? m->wy : m->wy + m->wh + vertpad;
		m->wy = m->topbar ? m->wy + bh + vp : m->wy;
	} else
		m->by = -bh - vp;
	#else
	if (m->showbar) {
		m->wh -= bh;
		m->by = m->topbar ? m->wy : m->wy + m->wh;
		m->wy = m->topbar ? m->wy + bh : m->wy;
	} else
		m->by = -bh;
	#endif // EXTRABAR_PATCH
}

void
updateclientlist()
{
	Client *c;
	Monitor *m;

	XDeleteProperty(dpy, root, netatom[NetClientList]);
	for (m = mons; m; m = m->next)
		for (c = m->clients; c; c = c->next)
			XChangeProperty(dpy, root, netatom[NetClientList],
				XA_WINDOW, 32, PropModeAppend,
				(unsigned char *) &(c->win), 1);
}

int
updategeom(void)
{
	int dirty = 0;

#ifdef XINERAMA
	if (XineramaIsActive(dpy)) {
		int i, j, n, nn;
		Client *c;
		Monitor *m;
		XineramaScreenInfo *info = XineramaQueryScreens(dpy, &nn);
		XineramaScreenInfo *unique = NULL;

		for (n = 0, m = mons; m; m = m->next, n++);
		/* only consider unique geometries as separate screens */
		unique = ecalloc(nn, sizeof(XineramaScreenInfo));
		for (i = 0, j = 0; i < nn; i++)
			if (isuniquegeom(unique, j, &info[i]))
				memcpy(&unique[j++], &info[i], sizeof(XineramaScreenInfo));
		XFree(info);
		nn = j;
		#if SORTSCREENS_PATCH
		sortscreens(unique, nn);
		#endif // SORTSCREENS_PATCH
		if (n <= nn) { /* new monitors available */
			for (i = 0; i < (nn - n); i++) {
				for (m = mons; m && m->next; m = m->next);
				if (m)
					m->next = createmon();
				else
					mons = createmon();
			}
			for (i = 0, m = mons; i < nn && m; m = m->next, i++) {
				if (i >= n
				|| unique[i].x_org != m->mx || unique[i].y_org != m->my
				|| unique[i].width != m->mw || unique[i].height != m->mh)
				{
					dirty = 1;
					m->num = i;
					m->mx = m->wx = unique[i].x_org;
					m->my = m->wy = unique[i].y_org;
					m->mw = m->ww = unique[i].width;
					m->mh = m->wh = unique[i].height;
					updatebarpos(m);
				}
				#if STATICSTATUS_PATCH && !STATUSALLMONS_PATCH
				if (i == statmonval)
					statmon = m;
				#endif // STATICSTATUS_PATCH
			}
		} else { /* less monitors available nn < n */
			for (i = nn; i < n; i++) {
				for (m = mons; m && m->next; m = m->next);
				while ((c = m->clients)) {
					dirty = 1;
					m->clients = c->next;
					detachstack(c);
					c->mon = mons;
					attach(c);
					attachstack(c);
				}
				if (m == selmon)
					selmon = mons;
				#if STATICSTATUS_PATCH && !STATUSALLMONS_PATCH
				if (m == statmon)
					statmon = mons;
				#endif // STATICSTATUS_PATCH
				cleanupmon(m);
			}
		}
		free(unique);
	} else
#endif /* XINERAMA */
	{ /* default monitor setup */
		if (!mons)
			mons = createmon();
		if (mons->mw != sw || mons->mh != sh) {
			dirty = 1;
			mons->mw = mons->ww = sw;
			mons->mh = mons->wh = sh;
			updatebarpos(mons);
		}
	}
	#if STATICSTATUS_PATCH && !STATUSALLMONS_PATCH
	if (!statmon)
		statmon = mons;
	#endif // STATICSTATUS_PATCH
	if (dirty) {
		selmon = mons;
		selmon = wintomon(root);
	}
	return dirty;
}

void
updatenumlockmask(void)
{
	unsigned int i, j;
	XModifierKeymap *modmap;

	numlockmask = 0;
	modmap = XGetModifierMapping(dpy);
	for (i = 0; i < 8; i++)
		for (j = 0; j < modmap->max_keypermod; j++)
			if (modmap->modifiermap[i * modmap->max_keypermod + j]
				== XKeysymToKeycode(dpy, XK_Num_Lock))
				numlockmask = (1 << i);
	XFreeModifiermap(modmap);
}

void
updatesizehints(Client *c)
{
	long msize;
	XSizeHints size;

	if (!XGetWMNormalHints(dpy, c->win, &size, &msize))
		/* size is uninitialized, ensure that size.flags aren't used */
		size.flags = PSize;
	if (size.flags & PBaseSize) {
		c->basew = size.base_width;
		c->baseh = size.base_height;
	} else if (size.flags & PMinSize) {
		c->basew = size.min_width;
		c->baseh = size.min_height;
	} else
		c->basew = c->baseh = 0;
	if (size.flags & PResizeInc) {
		c->incw = size.width_inc;
		c->inch = size.height_inc;
	} else
		c->incw = c->inch = 0;
	if (size.flags & PMaxSize) {
		c->maxw = size.max_width;
		c->maxh = size.max_height;
	} else
		c->maxw = c->maxh = 0;
	if (size.flags & PMinSize) {
		c->minw = size.min_width;
		c->minh = size.min_height;
	} else if (size.flags & PBaseSize) {
		c->minw = size.base_width;
		c->minh = size.base_height;
	} else
		c->minw = c->minh = 0;
	if (size.flags & PAspect) {
		c->mina = (float)size.min_aspect.y / size.min_aspect.x;
		c->maxa = (float)size.max_aspect.x / size.max_aspect.y;
	} else
		c->maxa = c->mina = 0.0;
	c->isfixed = (c->maxw && c->maxh && c->maxw == c->minw && c->maxh == c->minh);
}

void
updatestatus(void)
{
	#if STATUSALLMONS_PATCH
	Monitor* m;
	#endif // STATUSALLMONS_PATCH
	#if EXTRABAR_PATCH
	#if STATUS2D_PATCH
	char text[1024];
	#else
	char text[512];
	#endif // STATUS2D_PATCH
	if (!gettextprop(root, XA_WM_NAME, text, sizeof(text))) {
		strcpy(stext, "dwm-"VERSION);
		estext[0] = '\0';
	} else {
		char *e = strchr(text, statussep);
		if (e) {
			*e = '\0'; e++;
			strncpy(estext, e, sizeof(estext) - 1);
		} else {
			estext[0] = '\0';
		}
		#if STATUSCMD_PATCH
		copyvalidchars(stext, text);
		#else
		strncpy(stext, text, sizeof(stext) - 1);
		#endif // STATUSCMD_PATCH
	}
	#elif STATUSCMD_PATCH
	if (!gettextprop(root, XA_WM_NAME, rawstext, sizeof(rawstext)))
		strcpy(stext, "dwm-"VERSION);
	else
		copyvalidchars(stext, rawstext);
	#else
	if (!gettextprop(root, XA_WM_NAME, stext, sizeof(stext)))
		strcpy(stext, "dwm-"VERSION);
	#endif // EXTRABAR_PATCH
	#if STATUSALLMONS_PATCH
	for (m = mons; m; m = m->next)
		drawbar(m);
	#elif STATICSTATUS_PATCH
	drawbar(statmon);
	#else
	drawbar(selmon);
	#endif // STATUSALLMONS_PATCH | STATICSTATUS_PATCH
	#if SYSTRAY_PATCH
	if (showsystray)
		updatesystray();
	#endif // SYSTRAY_PATCH
}

void
updatetitle(Client *c)
{
	if (!gettextprop(c->win, netatom[NetWMName], c->name, sizeof c->name))
		gettextprop(c->win, XA_WM_NAME, c->name, sizeof c->name);
	if (c->name[0] == '\0') /* hack to mark broken clients */
		strcpy(c->name, broken);
}

void
updatewindowtype(Client *c)
{
	Atom state = getatomprop(c, netatom[NetWMState]);
	Atom wtype = getatomprop(c, netatom[NetWMWindowType]);

	if (state == netatom[NetWMFullscreen])
		setfullscreen(c, 1);

	#if EWMH_WINDOWS_FLOAT_PATCH
	if (wtype == netatom[NetWMWindowTypeDialog] ||
	    wtype == netatom[NetWMWindowTypeUtility] ||
	    wtype == netatom[NetWMWindowTypeToolbar] ||
	    wtype == netatom[NetWMWindowTypeSplash] ||
	    state == netatom[NetWMModal])
	#else
	if (wtype == netatom[NetWMWindowTypeDialog])
	#endif //EWMH_WINDOWS_FLOAT_PATCH
	{
		#if CENTER_PATCH
		if (c->x <= c->mon->mx && c->y <= c->mon->my)
			c->iscentered = 1;
		#endif // CENTER_PATCH
		c->isfloating = 1;
	}
}

void
updatewmhints(Client *c)
{
	XWMHints *wmh;

	if ((wmh = XGetWMHints(dpy, c->win))) {
		if (c == selmon->sel && wmh->flags & XUrgencyHint) {
			wmh->flags &= ~XUrgencyHint;
			XSetWMHints(dpy, c->win, wmh);
		} else {
			c->isurgent = (wmh->flags & XUrgencyHint) ? 1 : 0;
			#if URGENTBORDER_PATCH
			if (c->isurgent) {
				#if FLOAT_BORDER_COLOR_PATCH
				if (c->isfloating)
					XSetWindowBorder(dpy, c->win, scheme[SchemeUrg][ColFloat].pixel);
				else
				#endif
				XSetWindowBorder(dpy, c->win, scheme[SchemeUrg][ColBorder].pixel);
			}
			#endif // URGENTBORDER_PATCH
		}
		if (wmh->flags & InputHint)
			c->neverfocus = !wmh->input;
		else
			c->neverfocus = 0;
		XFree(wmh);
	}
}

void
view(const Arg *arg)
{
	#if EMPTYVIEW_PATCH
	if (arg->ui && (arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
	#else
	if ((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
	#endif // EMPTYVIEW_PATCH
		return;
	selmon->seltags ^= 1; /* toggle sel tagset */
	#if PERTAG_PATCH
	pertagview(arg);
	#if SWAPFOCUS_PATCH
	Client *unmodified = selmon->pertag->prevclient[selmon->pertag->curtag];
	#endif // SWAPFOCUS_PATCH
	#else
	if (arg->ui & TAGMASK)
		selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
	#endif // PERTAG_PATCH
	focus(NULL);
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	selmon->pertag->prevclient[selmon->pertag->curtag] = unmodified;
	#endif // SWAPFOCUS_PATCH
	arrange(selmon);
	#if EWMHTAGS_PATCH
	updatecurrentdesktop();
	#endif // EWMHTAGS_PATCH
}

Client *
wintoclient(Window w)
{
	Client *c;
	Monitor *m;

	for (m = mons; m; m = m->next)
		for (c = m->clients; c; c = c->next)
			if (c->win == w)
				return c;
	return NULL;
}

Monitor *
wintomon(Window w)
{
	int x, y;
	Client *c;
	Monitor *m;

	if (w == root && getrootptr(&x, &y))
		return recttomon(x, y, 1, 1);
	for (m = mons; m; m = m->next)
		#if EXTRABAR_PATCH
		if (w == m->barwin || w == m->extrabarwin)
		#else
		if (w == m->barwin)
		#endif // EXTRABAR_PATCH
			return m;
	if ((c = wintoclient(w)))
		return c->mon;
	return selmon;
}

/* There's no way to check accesses to destroyed windows, thus those cases are
 * ignored (especially on UnmapNotify's). Other types of errors call Xlibs
 * default error handler, which may call exit. */
int
xerror(Display *dpy, XErrorEvent *ee)
{
	if (ee->error_code == BadWindow
	|| (ee->request_code == X_SetInputFocus && ee->error_code == BadMatch)
	|| (ee->request_code == X_PolyText8 && ee->error_code == BadDrawable)
	|| (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable)
	|| (ee->request_code == X_PolySegment && ee->error_code == BadDrawable)
	|| (ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch)
	|| (ee->request_code == X_GrabButton && ee->error_code == BadAccess)
	|| (ee->request_code == X_GrabKey && ee->error_code == BadAccess)
	|| (ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
		return 0;
	fprintf(stderr, "dwm: fatal error: request code=%d, error code=%d\n",
		ee->request_code, ee->error_code);
	return xerrorxlib(dpy, ee); /* may call exit */
}

int
xerrordummy(Display *dpy, XErrorEvent *ee)
{
	return 0;
}

/* Startup Error handler to check if another window manager
 * is already running. */
int
xerrorstart(Display *dpy, XErrorEvent *ee)
{
	die("dwm: another window manager is already running");
	return -1;
}

void
zoom(const Arg *arg)
{
	Client *c = selmon->sel;
	#if ZOOMSWAP_PATCH
	Client *at = NULL, *cold, *cprevious = NULL, *p;
	#endif // ZOOMSWAP_PATCH

	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	selmon->pertag->prevclient[selmon->pertag->curtag] = nexttiled(selmon->clients);
	#endif // SWAPFOCUS_PATCH

	if (!selmon->lt[selmon->sellt]->arrange
	|| (selmon->sel && selmon->sel->isfloating)
	#if ZOOMSWAP_PATCH
	|| !c
	#endif // ZOOMSWAP_PATCH
	)
		return;

	#if ZOOMSWAP_PATCH
	if (c == nexttiled(selmon->clients)) {
		#if PERTAG_PATCH
		p = selmon->pertag->prevzooms[selmon->pertag->curtag];
		#else
		p = prevzoom;
		#endif // PERTAG_PATCH
		at = prevtiled(p);
		if (at)
			cprevious = nexttiled(at->next);
		if (!cprevious || cprevious != p) {
			#if PERTAG_PATCH
			selmon->pertag->prevzooms[selmon->pertag->curtag] = NULL;
			#else
			prevzoom = NULL;
			#endif // PERTAG_PATCH
			#if SWAPFOCUS_PATCH && PERTAG_PATCH
			if (!c || !(c = selmon->pertag->prevclient[selmon->pertag->curtag] = nexttiled(c->next)))
			#else
			if (!c || !(c = nexttiled(c->next)))
			#endif // SWAPFOCUS_PATCH
				return;
		} else
			#if SWAPFOCUS_PATCH && PERTAG_PATCH
			c = selmon->pertag->prevclient[selmon->pertag->curtag] = cprevious;
			#else
			c = cprevious;
			#endif // SWAPFOCUS_PATCH
	}

	cold = nexttiled(selmon->clients);
	if (c != cold && !at)
		at = prevtiled(c);
	detach(c);
	attach(c);
	/* swap windows instead of pushing the previous one down */
	if (c != cold && at) {
		#if PERTAG_PATCH
		selmon->pertag->prevzooms[selmon->pertag->curtag] = cold;
		#else
		prevzoom = cold;
		#endif // PERTAG_PATCH
		if (cold && at != cold) {
			detach(cold);
			cold->next = at->next;
			at->next = cold;
		}
	}
	focus(c);
	arrange(c->mon);
	#else
	if (c == nexttiled(selmon->clients))
		#if SWAPFOCUS_PATCH && PERTAG_PATCH
		if (!c || !(c = selmon->pertag->prevclient[selmon->pertag->curtag] = nexttiled(c->next)))
		#else
		if (!c || !(c = nexttiled(c->next)))
		#endif // SWAPFOCUS_PATCH
			return;
	pop(c);
	#endif // ZOOMSWAP_PATCH
}

int
main(int argc, char *argv[])
{
	#if CMDCUSTOMIZE_PATCH
	for (int i=1;i<argc;i+=1)
		if (!strcmp("-v", argv[i]))
			die("dwm-"VERSION);
		else if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i]))
			die(help());
		#if !PANGO_PATCH
		else if (!strcmp("-fn", argv[i])) /* font set */
			fonts[0] = argv[++i];
		#endif // PANGO_PATCH
		#if !VTCOLORS_PATCH
		else if (!strcmp("-nb", argv[i])) /* normal background color */
			colors[SchemeNorm][1] = argv[++i];
		else if (!strcmp("-nf", argv[i])) /* normal foreground color */
			colors[SchemeNorm][0] = argv[++i];
		else if (!strcmp("-sb", argv[i])) /* selected background color */
			colors[SchemeSel][1] = argv[++i];
		else if (!strcmp("-sf", argv[i])) /* selected foreground color */
			colors[SchemeSel][0] = argv[++i];
		#endif // !VTCOLORS_PATCH
		#if NODMENU_PATCH
		else if (!strcmp("-df", argv[i])) /* dmenu font */
			dmenucmd[2] = argv[++i];
		else if (!strcmp("-dnb", argv[i])) /* dmenu normal background color */
			dmenucmd[4] = argv[++i];
		else if (!strcmp("-dnf", argv[i])) /* dmenu normal foreground color */
			dmenucmd[6] = argv[++i];
		else if (!strcmp("-dsb", argv[i])) /* dmenu selected background color */
			dmenucmd[8] = argv[++i];
		else if (!strcmp("-dsf", argv[i])) /* dmenu selected foreground color */
			dmenucmd[10] = argv[++i];
		#else
		else if (!strcmp("-df", argv[i])) /* dmenu font */
			dmenucmd[4] = argv[++i];
		else if (!strcmp("-dnb", argv[i])) /* dmenu normal background color */
			dmenucmd[6] = argv[++i];
		else if (!strcmp("-dnf", argv[i])) /* dmenu normal foreground color */
			dmenucmd[8] = argv[++i];
		else if (!strcmp("-dsb", argv[i])) /* dmenu selected background color */
			dmenucmd[10] = argv[++i];
		else if (!strcmp("-dsf", argv[i])) /* dmenu selected foreground color */
			dmenucmd[12] = argv[++i];
		#endif // NODMENU_PATCH
		else die(help());
	#else
	if (argc == 2 && !strcmp("-v", argv[1]))
		die("dwm-"VERSION);
	else if (argc != 1)
		die("usage: dwm [-v]");
	#endif // CMDCUSTOMIZE_PATCH
	if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
		fputs("warning: no locale support\n", stderr);
	if (!(dpy = XOpenDisplay(NULL)))
		die("dwm: cannot open display");
	#if SWALLOW_PATCH
	if (!(xcon = XGetXCBConnection(dpy)))
		die("dwm: cannot get xcb connection\n");
	#endif // SWALLOW_PATCH
	checkotherwm();
	#if XRDB_PATCH && !VTCOLORS_PATCH
	XrmInitialize();
	loadxrdb();
	#endif // XRDB_PATCH && !VTCOLORS_PATCH

	setup();
#ifdef __OpenBSD__
	if (pledge("stdio rpath proc exec", NULL) == -1)
		die("pledge");
#endif /* __OpenBSD__ */
	scan();
	#if AUTOSTART_PATCH
	runAutostart();
	#endif
	run();
	#if RESTARTSIG_PATCH
	if (restart)
		execvp(argv[0], argv);
	#endif // RESTARTSIG_PATCH
	cleanup();
	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}
