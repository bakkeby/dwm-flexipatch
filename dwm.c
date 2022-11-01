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

#if BAR_FLEXWINTITLE_PATCH
#ifndef FLEXTILE_DELUXE_LAYOUT
#define FLEXTILE_DELUXE_LAYOUT 1
#endif
#endif

#if BAR_PANGO_PATCH
#include <pango/pango.h>
#endif // BAR_PANGO_PATCH

#if RESTARTSIG_PATCH
#include <poll.h>
#endif // RESTARTSIG_PATCH

#if XKB_PATCH
#include <X11/XKBlib.h>
#endif // XKB_PATCH

#if SPAWNCMD_PATCH
#include <assert.h>
#include <libgen.h>
#include <sys/stat.h>
#define SPAWN_CWD_DELIM " []{}()<>\"':"
#endif // SPAWNCMD_PATCH

/* macros */
#define Button6                 6
#define Button7                 7
#define Button8                 8
#define Button9                 9
#define NUMTAGS                 9
#define BARRULES                20
#if TAB_PATCH
#define MAXTABS                 50
#endif // TAB_PATCH
#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#if BAR_ANYBAR_PATCH
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->mx+(m)->mw) - MAX((x),(m)->mx)) \
                               * MAX(0, MIN((y)+(h),(m)->my+(m)->mh) - MAX((y),(m)->my)))
#else
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->wx+(m)->ww) - MAX((x),(m)->wx)) \
                               * MAX(0, MIN((y)+(h),(m)->wy+(m)->wh) - MAX((y),(m)->wy)))
#endif // BAR_ANYBAR_PATCH
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
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MOUSEMASK               (BUTTONMASK|PointerMotionMask)
#define WIDTH(X)                ((X)->w + 2 * (X)->bw)
#define HEIGHT(X)               ((X)->h + 2 * (X)->bw)
#define WTYPE                   "_NET_WM_WINDOW_TYPE_"
#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
#define TOTALTAGS               (NUMTAGS + LENGTH(scratchpads))
#define TAGMASK                 ((1 << TOTALTAGS) - 1)
#define SPTAG(i)                ((1 << NUMTAGS) << (i))
#define SPTAGMASK               (((1 << LENGTH(scratchpads))-1) << NUMTAGS)
#else
#define TAGMASK                 ((1 << NUMTAGS) - 1)
#endif // SCRATCHPADS_PATCH
#define TEXTWM(X)               (drw_fontset_getwidth(drw, (X), True) + lrpad)
#define TEXTW(X)                (drw_fontset_getwidth(drw, (X), False) + lrpad)
#define HIDDEN(C)               ((getstate(C->win) == IconicState))

/* enums */
enum {
	#if RESIZEPOINT_PATCH || RESIZECORNERS_PATCH
	CurResizeBR,
	CurResizeBL,
	CurResizeTR,
	CurResizeTL,
	#endif // RESIZEPOINT_PATCH | RESIZECORNERS_PATCH
	#if DRAGMFACT_PATCH
	CurResizeHorzArrow,
	CurResizeVertArrow,
	#endif // DRAGMFACT_PATCH
	#if DRAGCFACT_PATCH
	CurIronCross,
	#endif // DRAGCFACT_PATCH
	CurNormal,
	CurResize,
	CurMove,
	CurLast
}; /* cursor */

enum {
	SchemeNorm,
	SchemeSel,
	SchemeTitleNorm,
	SchemeTitleSel,
	SchemeTagsNorm,
	SchemeTagsSel,
	SchemeHidNorm,
	SchemeHidSel,
	SchemeUrg,
	#if RENAMED_SCRATCHPADS_PATCH
	SchemeScratchSel,
	SchemeScratchNorm,
	#endif // RENAMED_SCRATCHPADS_PATCH
	#if BAR_FLEXWINTITLE_PATCH
	SchemeFlexActTTB,
	SchemeFlexActLTR,
	SchemeFlexActMONO,
	SchemeFlexActGRID,
	SchemeFlexActGRD1,
	SchemeFlexActGRD2,
	SchemeFlexActGRDM,
	SchemeFlexActHGRD,
	SchemeFlexActDWDL,
	SchemeFlexActSPRL,
	SchemeFlexInaTTB,
	SchemeFlexInaLTR,
	SchemeFlexInaMONO,
	SchemeFlexInaGRID,
	SchemeFlexInaGRD1,
	SchemeFlexInaGRD2,
	SchemeFlexInaGRDM,
	SchemeFlexInaHGRD,
	SchemeFlexInaDWDL,
	SchemeFlexInaSPRL,
	SchemeFlexSelTTB,
	SchemeFlexSelLTR,
	SchemeFlexSelMONO,
	SchemeFlexSelGRID,
	SchemeFlexSelGRD1,
	SchemeFlexSelGRD2,
	SchemeFlexSelGRDM,
	SchemeFlexSelHGRD,
	SchemeFlexSelDWDL,
	SchemeFlexSelSPRL,
	SchemeFlexActFloat,
	SchemeFlexInaFloat,
	SchemeFlexSelFloat,
	#endif // BAR_FLEXWINTITLE_PATCH
}; /* color schemes */

enum {
	NetSupported, NetWMName, NetWMState, NetWMCheck,
	NetWMFullscreen, NetActiveWindow, NetWMWindowType,
	#if BAR_WINICON_PATCH
	NetWMIcon,
	#endif // BAR_WINICON_PATCH
	#if BAR_SYSTRAY_PATCH
	NetSystemTray, NetSystemTrayOP, NetSystemTrayOrientation,
	NetSystemTrayVisual, NetWMWindowTypeDock, NetSystemTrayOrientationHorz,
	#endif // BAR_SYSTRAY_PATCH
	#if BAR_EWMHTAGS_PATCH
	NetDesktopNames, NetDesktopViewport, NetNumberOfDesktops, NetCurrentDesktop,
	#endif // BAR_EWMHTAGS_PATCH
	NetClientList,
	#if NET_CLIENT_LIST_STACKING_PATCH
	NetClientListStacking,
	#endif // NET_CLIENT_LIST_STACKING_PATCH
	NetLast
}; /* EWMH atoms */

enum {
	WMProtocols,
	WMDelete,
	WMState,
	WMTakeFocus,
	#if WINDOWROLERULE_PATCH
	WMWindowRole,
	#endif // WINDOWROLERULE_PATCH
	WMLast
}; /* default atoms */

#if SEAMLESS_RESTART_PATCH
enum {
	ClientFields,
	ClientTags,
	ClientLast
}; /* dwm client atoms */
#endif // SEAMLESS_RESTART_PATCH

enum {
	#if BAR_STATUSBUTTON_PATCH
	ClkButton,
	#endif // BAR_STATUSBUTTON_PATCH
	#if TAB_PATCH
	ClkTabBar,
	#endif // TAB_PATCH
	ClkTagBar,
	ClkLtSymbol,
	ClkStatusText,
	ClkWinTitle,
	ClkClientWin,
	ClkRootWin,
	#if XKB_PATCH
	ClkXKB,
	#endif // XKB_PATCH
	ClkLast
}; /* clicks */

enum {
	BAR_ALIGN_LEFT,
	BAR_ALIGN_CENTER,
	BAR_ALIGN_RIGHT,
	BAR_ALIGN_LEFT_LEFT,
	BAR_ALIGN_LEFT_RIGHT,
	BAR_ALIGN_LEFT_CENTER,
	BAR_ALIGN_NONE,
	BAR_ALIGN_RIGHT_LEFT,
	BAR_ALIGN_RIGHT_RIGHT,
	BAR_ALIGN_RIGHT_CENTER,
	BAR_ALIGN_LAST
}; /* bar alignment */

#if IPC_PATCH
typedef struct TagState TagState;
struct TagState {
       int selected;
       int occupied;
       int urgent;
};

typedef struct ClientState ClientState;
struct ClientState {
       int isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
};
#endif // IPC_PATCH

typedef union {
	#if IPC_PATCH
	long i;
	unsigned long ui;
	#else
	int i;
	unsigned int ui;
	#endif // IPC_PATCH
	float f;
	const void *v;
} Arg;

typedef struct Monitor Monitor;
typedef struct Bar Bar;
struct Bar {
	Window win;
	Monitor *mon;
	Bar *next;
	int idx;
	int showbar;
	int topbar;
	int external;
	int borderpx;
	int borderscheme;
	int bx, by, bw, bh; /* bar geometry */
	int w[BARRULES]; // width, array length == barrules, then use r index for lookup purposes
	int x[BARRULES]; // x position, array length == ^
};

typedef struct {
	int x;
	int y;
	int h;
	int w;
} BarArg;

typedef struct {
	int monitor;
	int bar;
	int alignment; // see bar alignment enum
	int (*widthfunc)(Bar *bar, BarArg *a);
	int (*drawfunc)(Bar *bar, BarArg *a);
	int (*clickfunc)(Bar *bar, Arg *arg, BarArg *a);
	int (*hoverfunc)(Bar *bar, BarArg *a, XMotionEvent *ev);
	char *name; // for debugging
	int x, w; // position, width for internal use
} BarRule;

typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg *arg);
	const Arg arg;
} Button;

#if XKB_PATCH
typedef struct XkbInfo XkbInfo;
struct XkbInfo {
	XkbInfo *next;
	XkbInfo *prev;
	int group;
	Window w;
};
#endif // XKB_PATCH

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
	#if SEAMLESS_RESTART_PATCH
	unsigned int idx;
	#endif // SEAMLESS_RESTART_PATCH
	int oldx, oldy, oldw, oldh;
	int basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid;
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
	#if PLACEMOUSE_PATCH
	int beingmoved;
	#endif // PLACEMOUSE_PATCH
	#if SIZEHINTS_ISFREESIZE_PATCH
	int isfreesize;
	#endif // SIZEHINTS_ISFREESIZE_PATCH
	#if SWALLOW_PATCH
	int isterminal, noswallow;
	pid_t pid;
	#endif // SWALLOW_PATCH
	#if STEAM_PATCH
	int issteam;
	#endif // STEAM_PATCH
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
	#if IPC_PATCH
	ClientState prevstate;
	#endif // IPC_PATCH
	#if RENAMED_SCRATCHPADS_PATCH
	char scratchkey;
	#endif // RENAMED_SCRATCHPADS_PATCH
	#if XKB_PATCH
	XkbInfo *xkb;
	#endif // XKB_PATCH
	#if BAR_WINICON_PATCH
	unsigned int icw, ich;
	Picture icon;
	#endif // BAR_WINICON_PATCH
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

#if INSETS_PATCH
typedef struct {
	int x;
	int y;
	int w;
	int h;
} Inset;
#endif // INSETS_PATCH

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
	int mx, my, mw, mh;   /* screen size */
	int wx, wy, ww, wh;   /* window area  */
	#if TAB_PATCH
	int ty;               /* tab bar geometry */
	#endif // TAB_PATCH
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
	#if TAB_PATCH
	int showtab;
	int toptab;
	Window tabwin;
	int ntabs;
	int tab_widths[MAXTABS];
	#endif // TAB_PATCH
	Client *clients;
	Client *sel;
	Client *stack;
	Monitor *next;
	Bar *bar;
	const Layout *lt[2];
	#if BAR_ALTERNATIVE_TAGS_PATCH
	unsigned int alttag;
	#endif // BAR_ALTERNATIVE_TAGS_PATCH
	#if PERTAG_PATCH
	Pertag *pertag;
	#endif // PERTAG_PATCH
	#if INSETS_PATCH
	Inset inset;
	#endif // INSETS_PATCH
	#if BAR_TAGLABELS_PATCH
	char taglabel[NUMTAGS][64];
	#endif // BAR_TAGLABELS_PATCH
	#if IPC_PATCH
	char lastltsymbol[16];
	TagState tagstate;
	Client *lastsel;
	const Layout *lastlt;
	#endif // IPC_PATCH
	#if BAR_TAGPREVIEW_PATCH
	Window tagwin;
	int previewshow;
	Pixmap tagmap[NUMTAGS];
	#endif // BAR_TAGPREVIEW_PATCH
};

typedef struct {
	const char *class;
	#if WINDOWROLERULE_PATCH
	const char *role;
	#endif // WINDOWROLERULE_PATCH
	const char *instance;
	const char *title;
	const char *wintype;
	unsigned int tags;
	#if SWITCHTAG_PATCH
	int switchtag;
	#endif // SWITCHTAG_PATCH
	#if CENTER_PATCH
	int iscentered;
	#endif // CENTER_PATCH
	int isfloating;
	#if SELECTIVEFAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
	int isfakefullscreen;
	#endif // SELECTIVEFAKEFULLSCREEN_PATCH
	#if SIZEHINTS_ISFREESIZE_PATCH
	int isfreesize;
	#endif // SIZEHINTS_ISFREESIZE_PATCH
	#if ISPERMANENT_PATCH
	int ispermanent;
	#endif // ISPERMANENT_PATCH
	#if SWALLOW_PATCH
	int isterminal;
	int noswallow;
	#endif // SWALLOW_PATCH
	#if FLOATPOS_PATCH
	const char *floatpos;
	#endif // FLOATPOS_PATCH
	int monitor;
	#if RENAMED_SCRATCHPADS_PATCH
	const char scratchkey;
	#endif // RENAMED_SCRATCHPADS_PATCH
	#if XKB_PATCH
	int xkb_layout;
	#endif // XKB_PATCH
} Rule;

#if XKB_PATCH
#define RULE(...) { .monitor = -1, .xkb_layout = -1, __VA_ARGS__ },
#else
#define RULE(...) { .monitor = -1, __VA_ARGS__ },
#endif // XKB_PATCH

/* Cross patch compatibility rule macro helper macros */
#define FLOATING , .isfloating = 1
#if CENTER_PATCH
#define CENTERED , .iscentered = 1
#else
#define CENTERED
#endif // CENTER_PATCH
#if ISPERMANENT_PATCH
#define PERMANENT , .ispermanent = 1
#else
#define PERMANENT
#endif // ISPERMANENT_PATCH
#if SELECTIVEFAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
#define FAKEFULLSCREEN , .isfakefullscreen = 1
#else
#define FAKEFULLSCREEN
#endif // SELECTIVEFAKEFULLSCREEN_PATCH
#if SWALLOW_PATCH
#define NOSWALLOW , .noswallow = 1
#define TERMINAL , .isterminal = 1
#else
#define NOSWALLOW
#define TERMINAL
#endif // SWALLOW_PATCH
#if SWITCHTAG_PATCH
#define SWITCHTAG , .switchtag = 1
#else
#define SWITCHTAG
#endif // SWITCHTAG_PATCH

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
static void drawbarwin(Bar *bar);
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
static Atom getatomprop(Client *c, Atom prop, Atom req);
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
static void pop(Client *c);
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
#if BAR_SYSTRAY_PATCH
static int sendevent(Window w, Atom proto, int m, long d0, long d1, long d2, long d3, long d4);
#else
static int sendevent(Client *c, Atom proto);
#endif // BAR_SYSTRAY_PATCH
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
#if RIODRAW_PATCH
static pid_t spawncmd(const Arg *arg);
#endif // RIODRAW_PATCH
static void tag(const Arg *arg);
static void tagmon(const Arg *arg);
static void togglebar(const Arg *arg);
static void togglefloating(const Arg *arg);
static void toggletag(const Arg *arg);
static void toggleview(const Arg *arg);
static void unfocus(Client *c, int setfocus, Client *nextfocus);
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
static void updatewmhints(Client *c);
static void view(const Arg *arg);
static Client *wintoclient(Window w);
static Monitor *wintomon(Window w);
static int xerror(Display *dpy, XErrorEvent *ee);
static int xerrordummy(Display *dpy, XErrorEvent *ee);
static int xerrorstart(Display *dpy, XErrorEvent *ee);
static void zoom(const Arg *arg);

/* bar functions */

#include "patch/include.h"

/* variables */
static const char broken[] = "broken";
#if BAR_PANGO_PATCH || BAR_STATUS2D_PATCH && !BAR_STATUSCOLORS_PATCH
static char stext[1024];
#else
static char stext[512];
#endif // BAR_PANGO_PATCH | BAR_STATUS2D_PATCH
#if BAR_EXTRASTATUS_PATCH || BAR_STATUSCMD_PATCH
#if BAR_STATUS2D_PATCH
static char rawstext[1024];
#else
static char rawstext[512];
#endif // BAR_STATUS2D_PATCH
#endif // BAR_EXTRASTATUS_PATCH | BAR_STATUSCMD_PATCH
#if BAR_EXTRASTATUS_PATCH
#if BAR_STATUS2D_PATCH && !BAR_STATUSCOLORS_PATCH
static char estext[1024];
#else
static char estext[512];
#endif // BAR_STATUS2D_PATCH
#if BAR_STATUSCMD_PATCH
static char rawestext[1024];
#endif // BAR_STATUS2D_PATCH | BAR_STATUSCMD_PATCH
#endif // BAR_EXTRASTATUS_PATCH

#if XKB_PATCH
static int xkbEventType = 0;
#endif // XKB_PATCH
static int screen;
static int sw, sh;           /* X display screen geometry width, height */
static int bh;               /* bar geometry */
static int lrpad;            /* sum of left and right padding for text */
/* Some clients (e.g. alacritty) helpfully send configure requests with a new size or position
 * when they detect that they have been moved to another monitor. This can cause visual glitches
 * when moving (or resizing) client windows from one monitor to another. This variable is used
 * internally to ignore such configure requests while movemouse or resizemouse are being used. */
static int ignoreconfigurerequests = 0;
#if WARP_PATCH
static int force_warp = 0; // force warp in some situations, e.g. killclient
static int ignore_warp = 0; // force skip warp in some situations, e.g. dragmfact, dragcfact
#endif // WARP_PATCH
static int (*xerrorxlib)(Display *, XErrorEvent *);
static unsigned int numlockmask = 0;
#if RIODRAW_PATCH
static int riodimensions[4] = { -1, -1, -1, -1 };
static pid_t riopid = 0;
#endif // RIODRAW_PATCH
static void (*handler[LASTEvent]) (XEvent *) = {
	[ButtonPress] = buttonpress,
	#if COMBO_PATCH || BAR_HOLDBAR_PATCH
	[ButtonRelease] = keyrelease,
	#endif // COMBO_PATCH / BAR_HOLDBAR_PATCH
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
	#if COMBO_PATCH || BAR_HOLDBAR_PATCH
	[KeyRelease] = keyrelease,
	#endif // COMBO_PATCH / BAR_HOLDBAR_PATCH
	[MappingNotify] = mappingnotify,
	[MapRequest] = maprequest,
	#if !FOCUSONCLICK_PATCH
	[MotionNotify] = motionnotify,
	#endif // FOCUSONCLICK_PATCH
	[PropertyNotify] = propertynotify,
	#if BAR_SYSTRAY_PATCH
	[ResizeRequest] = resizerequest,
	#endif // BAR_SYSTRAY_PATCH
	[UnmapNotify] = unmapnotify
};
static Atom wmatom[WMLast], netatom[NetLast];
#if BAR_SYSTRAY_PATCH
static Atom xatom[XLast];
#endif // BAR_SYSTRAY_PATCH
#if SEAMLESS_RESTART_PATCH
static Atom clientatom[ClientLast];
#endif // SEAMLESS_RESTART_PATCH
#if ON_EMPTY_KEYS_PATCH
static int isempty = 0;
#endif // ON_EMPTY_KEYS_PATCH
#if RESTARTSIG_PATCH
static volatile sig_atomic_t running = 1;
#else
static int running = 1;
#endif // RESTARTSIG_PATCH
static Cur *cursor[CurLast];
static Clr **scheme;
static Display *dpy;
static Drw *drw;
static Monitor *mons, *selmon;
static Window root, wmcheckwin;

/* configuration, allows nested code to access above variables */
#include "config.h"

#include "patch/include.c"

/* compile-time check if all tags fit into an unsigned int bit array. */
#if SCRATCHPAD_ALT_1_PATCH
struct NumTags { char limitexceeded[NUMTAGS > 30 ? -1 : 1]; };
#else
struct NumTags { char limitexceeded[NUMTAGS > 31 ? -1 : 1]; };
#endif // SCRATCHPAD_ALT_1_PATCH

/* function implementations */
void
applyrules(Client *c)
{
	const char *class, *instance;
	Atom wintype;
	#if WINDOWROLERULE_PATCH
	char role[64];
	#endif // WINDOWROLERULE_PATCH
	unsigned int i;
	#if SWITCHTAG_PATCH
	unsigned int newtagset;
	#endif // SWITCHTAG_PATCH
	const Rule *r;
	Monitor *m;
	XClassHint ch = { NULL, NULL };

	/* rule matching */
	#if SWALLOW_PATCH
	c->noswallow = -1;
	#endif // SWALLOW_PATCH
	#if SIZEHINTS_ISFREESIZE_PATCH
	c->isfreesize = 1;
	#endif // SIZEHINTS_ISFREESIZE_PATCH
	c->isfloating = 0;
	c->tags = 0;
	#if RENAMED_SCRATCHPADS_PATCH
	c->scratchkey = 0;
	#endif // RENAMED_SCRATCHPADS_PATCH
	XGetClassHint(dpy, c->win, &ch);
	class    = ch.res_class ? ch.res_class : broken;
	instance = ch.res_name  ? ch.res_name  : broken;
	wintype  = getatomprop(c, netatom[NetWMWindowType], XA_ATOM);
	#if WINDOWROLERULE_PATCH
	gettextprop(c->win, wmatom[WMWindowRole], role, sizeof(role));
	#endif // WINDOWROLERULE_PATCH

	#if STEAM_PATCH
	if (strstr(class, "Steam") || strstr(class, "steam_app_"))
		c->issteam = 1;
	#endif // STEAM_PATCH

	for (i = 0; i < LENGTH(rules); i++) {
		r = &rules[i];
		if ((!r->title || strstr(c->name, r->title))
		&& (!r->class || strstr(class, r->class))
		#if WINDOWROLERULE_PATCH
		&& (!r->role || strstr(role, r->role))
		#endif // WINDOWROLERULE_PATCH
		&& (!r->instance || strstr(instance, r->instance))
		&& (!r->wintype || wintype == XInternAtom(dpy, r->wintype, False)))
		{
			#if CENTER_PATCH
			c->iscentered = r->iscentered;
			#endif // CENTER_PATCH
			#if ISPERMANENT_PATCH
			c->ispermanent = r->ispermanent;
			#endif // ISPERMANENT_PATCH
			#if SELECTIVEFAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
			c->fakefullscreen = r->isfakefullscreen;
			#endif // SELECTIVEFAKEFULLSCREEN_PATCH
			#if SWALLOW_PATCH
			c->isterminal = r->isterminal;
			c->noswallow = r->noswallow;
			#endif // SWALLOW_PATCH
			#if SIZEHINTS_ISFREESIZE_PATCH
			c->isfreesize = r->isfreesize;
			#endif // SIZEHINTS_ISFREESIZE_PATCH
			c->isfloating = r->isfloating;
			c->tags |= r->tags;
			#if RENAMED_SCRATCHPADS_PATCH
			c->scratchkey = r->scratchkey;
			#elif SCRATCHPADS_PATCH
			if ((r->tags & SPTAGMASK) && r->isfloating) {
				c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
				c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
			}
			#endif // SCRATCHPADS_PATCH
			for (m = mons; m && m->num != r->monitor; m = m->next);
			if (m)
				c->mon = m;
			#if FLOATPOS_PATCH
			if (c->isfloating && r->floatpos) {
				#if CENTER_PATCH
				c->iscentered = 0;
				#endif // CENTER_PATCH
				setfloatpos(c, r->floatpos);
			}
			#endif // FLOATPOS_PATCH

			#if SWITCHTAG_PATCH
			#if SWALLOW_PATCH
			if (r->switchtag && (
				c->noswallow > 0 ||
				!termforwin(c) ||
				!(c->isfloating && swallowfloating && c->noswallow < 0)))
			#else
			if (r->switchtag)
			#endif // SWALLOW_PATCH
			{
				selmon = c->mon;
				if (r->switchtag == 2 || r->switchtag == 4)
					newtagset = c->mon->tagset[c->mon->seltags] ^ c->tags;
				else
					newtagset = c->tags;

				/* Switch to the client's tag, but only if that tag is not already shown */
				if (newtagset && !(c->tags & c->mon->tagset[c->mon->seltags])) {
					if (r->switchtag == 3 || r->switchtag == 4)
						c->switchtag = c->mon->tagset[c->mon->seltags];
					if (r->switchtag == 1 || r->switchtag == 3) {
						#if PERTAG_PATCH
						pertagview(&((Arg) { .ui = newtagset }));
						arrange(c->mon);
						#else
						view(&((Arg) { .ui = newtagset }));
						#endif // PERTAG_PATCH
					} else {
						#if TAGSYNC_PATCH
						for (m = mons; m; m = m->next)
							m->tagset[m->seltags] = newtagset;
						arrange(NULL);
						#else
						c->mon->tagset[c->mon->seltags] = newtagset;
						arrange(c->mon);
						#endif // TAGSYNC_PATCH
					}
				}
			}
			#endif // SWITCHTAG_PATCH
			#if XKB_PATCH
			if (r->xkb_layout > -1)
				c->xkb->group = r->xkb_layout;
			#endif // XKB_PATCH
			#if ONLY_ONE_RULE_MATCH_PATCH
			break;
			#endif // ONLY_ONE_RULE_MATCH_PATCH
		}
	}
	if (ch.res_class)
		XFree(ch.res_class);
	if (ch.res_name)
		XFree(ch.res_name);
	#if EMPTYVIEW_PATCH
	if (c->tags & TAGMASK)                    c->tags = c->tags & TAGMASK;
	#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
	else if (c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags] & ~SPTAGMASK;
	#elif SCRATCHPAD_ALT_1_PATCH
	else if (c->tags != SCRATCHPAD_MASK && c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags];
	#else
	else if (c->mon->tagset[c->mon->seltags]) c->tags = c->mon->tagset[c->mon->seltags];
	#endif // SCRATCHPADS_PATCH
	else                                      c->tags = 1;
	#elif SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
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
		if (!c->hintsvalid)
			updatesizehints(c);
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
	#if TAB_PATCH
	updatebarpos(m);
	XMoveResizeWindow(dpy, m->tabwin, m->wx, m->ty, m->ww, th);
	#endif // TAB_PATCH
	strncpy(m->ltsymbol, m->lt[m->sellt]->symbol, sizeof m->ltsymbol);
	if (m->lt[m->sellt]->arrange)
		m->lt[m->sellt]->arrange(m);
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
	int click, i, r;
	#if TAB_PATCH
	int x;
	#endif // TAB_PATCH
	Arg arg = {0};
	Client *c;
	Monitor *m;
	Bar *bar;
	XButtonPressedEvent *ev = &e->xbutton;
	const BarRule *br;
	BarArg carg = { 0, 0, 0, 0 };
	click = ClkRootWin;

	#if BAR_STATUSCMD_PATCH && !BAR_DWMBLOCKS_PATCH
	*lastbutton = '0' + ev->button;
	#endif // BAR_STATUSCMD_PATCH | BAR_DWMBLOCKS_PATCH

	/* focus monitor if necessary */
	if ((m = wintomon(ev->window)) && m != selmon
		#if FOCUSONCLICK_PATCH
		&& (focusonwheel || (ev->button != Button4 && ev->button != Button5))
		#endif // FOCUSONCLICK_PATCH
	) {
		unfocus(selmon->sel, 1, NULL);
		selmon = m;
		focus(NULL);
	}

	for (bar = selmon->bar; bar; bar = bar->next) {
		if (ev->window == bar->win) {
			for (r = 0; r < LENGTH(barrules); r++) {
				br = &barrules[r];
				if (br->bar != bar->idx || (br->monitor == 'A' && m != selmon) || br->clickfunc == NULL)
					continue;
				if (br->monitor != 'A' && br->monitor != -1 && br->monitor != bar->mon->num)
					continue;
				if (bar->x[r] <= ev->x && ev->x <= bar->x[r] + bar->w[r]) {
					carg.x = ev->x - bar->x[r];
					carg.y = ev->y - bar->borderpx;
					carg.w = bar->w[r];
					carg.h = bar->bh - 2 * bar->borderpx;
					click = br->clickfunc(bar, &arg, &carg);
					if (click < 0)
						return;
					break;
				}
			}
			break;
		}
	}

	#if TAB_PATCH
	if (ev->window == selmon->tabwin) {
		for (i = 0, x = 0, c = selmon->clients; c; c = c->next) {
			if (!ISVISIBLE(c) || HIDDEN(c))
				continue;
			x += selmon->tab_widths[i];
			if (ev->x > x)
				++i;
			else
				break;
			if (i >= m->ntabs)
				break;
		}
		if (c) {
			click = ClkTabBar;
			arg.ui = i;
		}
	}
	#endif // TAB_PATCH

	if (click == ClkRootWin && (c = wintoclient(ev->window))) {
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

	for (i = 0; i < LENGTH(buttons); i++) {
		if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
				&& CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state)) {
			buttons[i].func(
				(
					click == ClkTagBar
					#if TAB_PATCH
					|| click == ClkTabBar
					#endif // TAB_PATCH
					#if BAR_WINTITLEACTIONS_PATCH
					|| click == ClkWinTitle
					#endif // BAR_WINTITLEACTIONS_PATCH
				) && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg
			);
		}
	}
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
	Monitor *m;
	Layout foo = { "", NULL };
	size_t i;

	#if ALT_TAB_PATCH
	alttabend();
	#endif // ALT_TAB_PATCH

	#if SEAMLESS_RESTART_PATCH
	for (m = mons; m; m = m->next)
		persistmonitorstate(m);
	#endif // SEAMLESS_RESTART_PATCH

	#if COOL_AUTOSTART_PATCH
	/* kill child processes */
	for (i = 0; i < autostart_len; i++) {
		if (0 < autostart_pids[i]) {
			kill(autostart_pids[i], SIGTERM);
			waitpid(autostart_pids[i], NULL, 0);
		}
	}
	#endif // COOL_AUTOSTART_PATCH

	selmon->lt[selmon->sellt] = &foo;
	for (m = mons; m; m = m->next)
		while (m->stack)
			unmanage(m->stack, 0);
	XUngrabKey(dpy, AnyKey, AnyModifier, root);
	while (mons)
		cleanupmon(mons);
	#if BAR_SYSTRAY_PATCH
	if (showsystray && systray) {
		while (systray->icons)
			removesystrayicon(systray->icons);
		if (systray->win) {
			XUnmapWindow(dpy, systray->win);
			XDestroyWindow(dpy, systray->win);
		}
		free(systray);
	}
	#endif // BAR_SYSTRAY_PATCH
	for (i = 0; i < CurLast; i++)
		drw_cur_free(drw, cursor[i]);
	#if BAR_STATUS2D_PATCH && !BAR_STATUSCOLORS_PATCH
	for (i = 0; i < LENGTH(colors) + 1; i++)
	#else
	for (i = 0; i < LENGTH(colors); i++)
	#endif // BAR_STATUS2D_PATCH
		free(scheme[i]);
	free(scheme);
	XDestroyWindow(dpy, wmcheckwin);
	drw_free(drw);
	XSync(dpy, False);
	XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
	XDeleteProperty(dpy, root, netatom[NetActiveWindow]);

	#if IPC_PATCH
	ipc_cleanup();

	if (close(epoll_fd) < 0)
		fprintf(stderr, "Failed to close epoll file descriptor\n");
	#endif // IPC_PATCH
}

void
cleanupmon(Monitor *mon)
{
	Monitor *m;
	Bar *bar;

	if (mon == mons)
		mons = mons->next;
	else {
		for (m = mons; m && m->next != mon; m = m->next);
		m->next = mon->next;
	}
	for (bar = mon->bar; bar; bar = mon->bar) {
		if (!bar->external) {
			XUnmapWindow(dpy, bar->win);
			XDestroyWindow(dpy, bar->win);
		}
		mon->bar = bar->next;
		#if BAR_SYSTRAY_PATCH
		if (systray && bar == systray->bar)
			systray->bar = NULL;
		#endif // BAR_SYSTRAY_PATCH
		free(bar);
	}
	#if TAB_PATCH
	XUnmapWindow(dpy, mon->tabwin);
	XDestroyWindow(dpy, mon->tabwin);
	#endif // TAB_PATCH
	#if PERTAG_PATCH
	free(mon->pertag);
	#endif // PERTAG_PATCH
	#if BAR_TAGPREVIEW_PATCH
	for (size_t i = 0; i < NUMTAGS; i++)
		if (mon->tagmap[i])
			XFreePixmap(dpy, mon->tagmap[i]);
	XUnmapWindow(dpy, mon->tagwin);
	XDestroyWindow(dpy, mon->tagwin);
	#endif // BAR_TAGPREVIEW_PATCH
	free(mon);
}

void
clientmessage(XEvent *e)
{
	#if BAR_SYSTRAY_PATCH
	XWindowAttributes wa;
	XSetWindowAttributes swa;
	#endif // BAR_SYSTRAY_PATCH
	XClientMessageEvent *cme = &e->xclient;
	Client *c = wintoclient(cme->window);
	#if FOCUSONNETACTIVE_PATCH
	unsigned int i;
	#endif // FOCUSONNETACTIVE_PATCH

	#if BAR_SYSTRAY_PATCH
	if (showsystray && systray && cme->window == systray->win && cme->message_type == netatom[NetSystemTrayOP]) {
		/* add systray icons */
		if (cme->data.l[1] == SYSTEM_TRAY_REQUEST_DOCK) {
			if (!(c = (Client *)calloc(1, sizeof(Client))))
				die("fatal: could not malloc() %u bytes\n", sizeof(Client));
			if (!(c->win = cme->data.l[2])) {
				free(c);
				return;
			}

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
			XClassHint ch = {"dwmsystray", "dwmsystray"};
			XSetClassHint(dpy, c->win, &ch);
			XReparentWindow(dpy, c->win, systray->win, 0, 0);
			/* use parents background color */
			swa.background_pixel = scheme[SchemeNorm][ColBg].pixel;
			XChangeWindowAttributes(dpy, c->win, CWBackPixel, &swa);
			sendevent(c->win, netatom[Xembed], StructureNotifyMask, CurrentTime, XEMBED_EMBEDDED_NOTIFY, 0 , systray->win, XEMBED_EMBEDDED_VERSION);
			XSync(dpy, False);
			setclientstate(c, NormalState);
		}
		return;
	}
	#endif // BAR_SYSTRAY_PATCH

	if (!c)
		return;
	if (cme->message_type == netatom[NetWMState]) {
		if (cme->data.l[1] == netatom[NetWMFullscreen]
		|| cme->data.l[2] == netatom[NetWMFullscreen]) {
			#if FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
			if (c->fakefullscreen == 2 && c->isfullscreen)
				c->fakefullscreen = 3;
			#endif // FAKEFULLSCREEN_CLIENT_PATCH
			setfullscreen(c, (cme->data.l[0] == 1 /* _NET_WM_STATE_ADD    */
				|| (cme->data.l[0] == 2 /* _NET_WM_STATE_TOGGLE */
				#if !FAKEFULLSCREEN_PATCH
				&& !c->isfullscreen
				#endif // !FAKEFULLSCREEN_PATCH
			)));
		}
	} else if (cme->message_type == netatom[NetActiveWindow]) {
		#if FOCUSONNETACTIVE_PATCH
		if (c->tags & c->mon->tagset[c->mon->seltags])
			focus(c);
		else {
			for (i = 0; i < NUMTAGS && !((1 << i) & c->tags); i++);
			if (i < NUMTAGS) {
				if (c != selmon->sel)
					unfocus(selmon->sel, 0, NULL);
				selmon = c->mon;
				if (((1 << i) & TAGMASK) != selmon->tagset[selmon->seltags])
					view(&((Arg) { .ui = 1 << i }));
				focus(c);
				restack(selmon);
			}
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
	Bar *bar;
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
			drw_resize(drw, sw, sh);
			updatebars();
			for (m = mons; m; m = m->next) {
				#if !FAKEFULLSCREEN_PATCH
				for (c = m->clients; c; c = c->next)
					#if FAKEFULLSCREEN_CLIENT_PATCH
					if (c->isfullscreen && c->fakefullscreen != 1)
					#else
					if (c->isfullscreen)
					#endif // FAKEFULLSCREEN_CLIENT_PATCH
						resizeclient(c, m->mx, m->my, m->mw, m->mh);
				#endif // !FAKEFULLSCREEN_PATCH
				for (bar = m->bar; bar; bar = bar->next)
					XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
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
	#if BAR_ANYBAR_PATCH
	Bar *bar;
	#endif // BAR_ANYBAR_PATCH
	XConfigureRequestEvent *ev = &e->xconfigurerequest;
	XWindowChanges wc;

	if (ignoreconfigurerequests)
		return;

	if ((c = wintoclient(ev->window))) {
		if (ev->value_mask & CWBorderWidth)
			c->bw = ev->border_width;
		else if (c->isfloating || !selmon->lt[selmon->sellt]->arrange) {
			m = c->mon;
			#if STEAM_PATCH
			if (!c->issteam) {
				if (ev->value_mask & CWX) {
					c->oldx = c->x;
					c->x = m->mx + ev->x;
				}
				if (ev->value_mask & CWY) {
					c->oldy = c->y;
					c->y = m->my + ev->y;
				}
			}
			#else
			if (ev->value_mask & CWX) {
				c->oldx = c->x;
				c->x = m->mx + ev->x;
			}
			if (ev->value_mask & CWY) {
				c->oldy = c->y;
				c->y = m->my + ev->y;
			}
			#endif // STEAM_PATCH
			if (ev->value_mask & CWWidth) {
				c->oldw = c->w;
				c->w = ev->width;
			}
			if (ev->value_mask & CWHeight) {
				c->oldh = c->h;
				c->h = ev->height;
			}
			if ((c->x + c->w) > m->mx + m->mw && c->isfloating)
				c->x = m->mx + (m->mw / 2 - WIDTH(c) / 2);  /* center in x direction */
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
		#if BAR_ANYBAR_PATCH
		m = wintomon(ev->window);
		for (bar = m->bar; bar; bar = bar->next) {
			if (bar->win == ev->window) {
				wc.y = bar->by;
				wc.x = bar->bx;
			}
		}
		#endif // BAR_ANYBAR_PATCH
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
	Monitor *m, *mon;
	int i, n, mi, max_bars = 2, istopbar = topbar;
	#if MONITOR_RULES_PATCH
	int layout;
	#endif // MONITOR_RULES_PATCH

	const BarRule *br;
	Bar *bar;
	#if MONITOR_RULES_PATCH
	int j;
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
	#if TAB_PATCH
	m->showtab = showtab;
	m->toptab = toptab;
	m->ntabs = 0;
	#endif // TAB_PATCH
	#if SETBORDERPX_PATCH
	m->borderpx = borderpx;
	#endif // SETBORDERPX_PATCH
	#if VANITYGAPS_PATCH
	m->gappih = gappih;
	m->gappiv = gappiv;
	m->gappoh = gappoh;
	m->gappov = gappov;
	#endif // VANITYGAPS_PATCH
	for (mi = 0, mon = mons; mon; mon = mon->next, mi++); // monitor index
	m->num = mi;
	#if MONITOR_RULES_PATCH
	for (j = 0; j < LENGTH(monrules); j++) {
		mr = &monrules[j];
		if ((mr->monitor == -1 || mr->monitor == m->num)
		#if PERTAG_PATCH
				&& (mr->tag <= 0 || (m->tagset[0] & (1 << (mr->tag - 1))))
		#endif // PERTAG_PATCH
		) {
			layout = MAX(mr->layout, 0);
			layout = MIN(layout, LENGTH(layouts) - 1);
			m->lt[0] = &layouts[layout];
			m->lt[1] = &layouts[1 % LENGTH(layouts)];
			strncpy(m->ltsymbol, layouts[layout].symbol, sizeof m->ltsymbol);

			if (mr->mfact > -1)
				m->mfact = mr->mfact;
			if (mr->nmaster > -1)
				m->nmaster = mr->nmaster;
			if (mr->showbar > -1)
				m->showbar = mr->showbar;
			if (mr->topbar > -1)
				istopbar = mr->topbar;
			break;
		}
	}
	#else
	m->lt[0] = &layouts[0];
	m->lt[1] = &layouts[1 % LENGTH(layouts)];
	strncpy(m->ltsymbol, layouts[0].symbol, sizeof m->ltsymbol);
	#endif // MONITOR_RULES_PATCH

	/* Derive the number of bars for this monitor based on bar rules */
	for (n = -1, i = 0; i < LENGTH(barrules); i++) {
		br = &barrules[i];
		if (br->monitor == 'A' || br->monitor == -1 || br->monitor == m->num)
			n = MAX(br->bar, n);
	}

	m->bar = NULL;
	for (i = 0; i <= n && i < max_bars; i++) {
		bar = ecalloc(1, sizeof(Bar));
		bar->mon = m;
		bar->idx = i;
		bar->next = m->bar;
		bar->topbar = istopbar;
		m->bar = bar;
		istopbar = !istopbar;
		bar->showbar = 1;
		bar->external = 0;
		#if BAR_BORDER_PATCH
		bar->borderpx = borderpx;
		#else
		bar->borderpx = 0;
		#endif // BAR_BORDER_PATCH
		bar->bh = bh + bar->borderpx * 2;
		bar->borderscheme = SchemeNorm;
	}

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
	for (i = 0; i <= NUMTAGS; i++) {
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
			if ((mr->monitor == -1 || mr->monitor == m->num) && (mr->tag == -1 || mr->tag == i)) {
				layout = MAX(mr->layout, 0);
				layout = MIN(layout, LENGTH(layouts) - 1);
				m->pertag->ltidxs[i][0] = &layouts[layout];
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

		#if PERTAG_VANITYGAPS_PATCH && VANITYGAPS_PATCH
		m->pertag->enablegaps[i] = 1;
		m->pertag->gaps[i] =
			((gappoh & 0xFF) << 0) | ((gappov & 0xFF) << 8) | ((gappih & 0xFF) << 16) | ((gappiv & 0xFF) << 24);
		#endif // PERTAG_VANITYGAPS_PATCH | VANITYGAPS_PATCH
	}
	#endif // PERTAG_PATCH

	#if SEAMLESS_RESTART_PATCH
	restoremonitorstate(m);
	#endif // SEAMLESS_RESTART_PATCH

	#if INSETS_PATCH
	m->inset = default_inset;
	#endif // INSETS_PATCH
	return m;
}

void
destroynotify(XEvent *e)
{
	Client *c;
	#if BAR_ANYBAR_PATCH
	Monitor *m;
	Bar *bar;
	#endif // BAR_ANYBAR_PATCH
	XDestroyWindowEvent *ev = &e->xdestroywindow;

	if ((c = wintoclient(ev->window)))
		unmanage(c, 1);
	#if SWALLOW_PATCH
	else if ((c = swallowingclient(ev->window)))
		unmanage(c->swallowing, 1);
	#endif // SWALLOW_PATCH
	#if BAR_SYSTRAY_PATCH
	else if (showsystray && (c = wintosystrayicon(ev->window))) {
		removesystrayicon(c);
		drawbarwin(systray->bar);
	}
	#endif // BAR_SYSTRAY_PATCH
	#if BAR_ANYBAR_PATCH
	else {
		 m = wintomon(ev->window);
		 for (bar = m->bar; bar; bar = bar->next) {
		 	if (bar->win == ev->window) {
				unmanagealtbar(ev->window);
				break;
			}
		}
	}
	#endif // BAR_ANYBAR_PATCH
}

void
detach(Client *c)
{
	Client **tc;
	#if SEAMLESS_RESTART_PATCH
	c->idx = 0;
	#endif // SEAMLESS_RESTART_PATCH

	for (tc = &c->mon->clients; *tc && *tc != c; tc = &(*tc)->next);
	*tc = c->next;
	c->next = NULL;
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
	c->snext = NULL;
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
	Bar *bar;
	
	#if !BAR_FLEXWINTITLE_PATCH
	if (m->showbar)
	#endif // BAR_FLEXWINTITLE_PATCH
		for (bar = m->bar; bar; bar = bar->next)
			drawbarwin(bar);
}

void
drawbars(void)
{
	Monitor *m;
	for (m = mons; m; m = m->next)
		drawbar(m);
}

void
drawbarwin(Bar *bar)
{
	if (!bar || !bar->win || bar->external)
		return;
	int r, w, total_drawn = 0;
	int rx, lx, rw, lw; // bar size, split between left and right if a center module is added
	const BarRule *br;

	if (bar->borderpx) {
		XSetForeground(drw->dpy, drw->gc, scheme[bar->borderscheme][ColBorder].pixel);
		XFillRectangle(drw->dpy, drw->drawable, drw->gc, 0, 0, bar->bw, bar->bh);
	}

	BarArg warg = { 0 };
	BarArg darg  = { 0 };
	warg.h = bar->bh - 2 * bar->borderpx;

	rw = lw = bar->bw - 2 * bar->borderpx;
	rx = lx = bar->borderpx;

	drw_setscheme(drw, scheme[SchemeNorm]);
	drw_rect(drw, lx, bar->borderpx, lw, bar->bh - 2 * bar->borderpx, 1, 1);
	for (r = 0; r < LENGTH(barrules); r++) {
		br = &barrules[r];
		if (br->bar != bar->idx || !br->widthfunc || (br->monitor == 'A' && bar->mon != selmon))
			continue;
		if (br->monitor != 'A' && br->monitor != -1 && br->monitor != bar->mon->num)
			continue;
		drw_setscheme(drw, scheme[SchemeNorm]);
		warg.w = (br->alignment < BAR_ALIGN_RIGHT_LEFT ? lw : rw);

		w = br->widthfunc(bar, &warg);
		w = MIN(warg.w, w);

		if (lw <= 0) { // if left is exhausted then switch to right side, and vice versa
			lw = rw;
			lx = rx;
		} else if (rw <= 0) {
			rw = lw;
			rx = lx;
		}

		switch(br->alignment) {
		default:
		case BAR_ALIGN_NONE:
		case BAR_ALIGN_LEFT_LEFT:
		case BAR_ALIGN_LEFT:
			bar->x[r] = lx;
			if (lx == rx) {
				rx += w;
				rw -= w;
			}
			lx += w;
			lw -= w;
			break;
		case BAR_ALIGN_LEFT_RIGHT:
		case BAR_ALIGN_RIGHT:
			bar->x[r] = lx + lw - w;
			if (lx == rx)
				rw -= w;
			lw -= w;
			break;
		case BAR_ALIGN_LEFT_CENTER:
		case BAR_ALIGN_CENTER:
			bar->x[r] = lx + lw / 2 - w / 2;
			if (lx == rx) {
				rw = rx + rw - bar->x[r] - w;
				rx = bar->x[r] + w;
			}
			lw = bar->x[r] - lx;
			break;
		case BAR_ALIGN_RIGHT_LEFT:
			bar->x[r] = rx;
			if (lx == rx) {
				lx += w;
				lw -= w;
			}
			rx += w;
			rw -= w;
			break;
		case BAR_ALIGN_RIGHT_RIGHT:
			bar->x[r] = rx + rw - w;
			if (lx == rx)
				lw -= w;
			rw -= w;
			break;
		case BAR_ALIGN_RIGHT_CENTER:
			bar->x[r] = rx + rw / 2 - w / 2;
			if (lx == rx) {
				lw = lx + lw - bar->x[r] + w;
				lx = bar->x[r] + w;
			}
			rw = bar->x[r] - rx;
			break;
		}
		bar->w[r] = w;
		darg.x = bar->x[r];
		darg.y = bar->borderpx;
		darg.h = bar->bh - 2 * bar->borderpx;
		darg.w = bar->w[r];
		if (br->drawfunc)
			total_drawn += br->drawfunc(bar, &darg);
	}

	if (total_drawn == 0 && bar->showbar) {
		bar->showbar = 0;
		updatebarpos(bar->mon);
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		arrange(bar->mon);
	}
	else if (total_drawn > 0 && !bar->showbar) {
		bar->showbar = 1;
		updatebarpos(bar->mon);
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
		drw_map(drw, bar->win, 0, 0, bar->bw, bar->bh);
		arrange(bar->mon);
	} else
		drw_map(drw, bar->win, 0, 0, bar->bw, bar->bh);
}

#if !FOCUSONCLICK_PATCH
void
enternotify(XEvent *e)
{
	Client *c;
	#if LOSEFULLSCREEN_PATCH
	Client *sel;
	#endif // LOSEFULLSCREEN_PATCH
	Monitor *m;
	XCrossingEvent *ev = &e->xcrossing;

	if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != root)
		return;
	c = wintoclient(ev->window);
	m = c ? c->mon : wintomon(ev->window);
	if (m != selmon) {
		#if LOSEFULLSCREEN_PATCH
		sel = selmon->sel;
		selmon = m;
		unfocus(sel, 1, c);
		#else
		unfocus(selmon->sel, 1, c);
		selmon = m;
		#endif // LOSEFULLSCREEN_PATCH
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
		#if TAB_PATCH
		drawtabs();
		#endif // TAB_PATCH
	}
}

void
focus(Client *c)
{
	if (!c || !ISVISIBLE(c))
		for (c = selmon->stack; c && !ISVISIBLE(c); c = c->snext);
	if (selmon->sel && selmon->sel != c)
		unfocus(selmon->sel, 0, c);
	if (c) {
		if (c->mon != selmon)
			selmon = c->mon;
		if (c->isurgent)
			seturgent(c, 0);
		detachstack(c);
		attachstack(c);
		grabbuttons(c, 1);
		#if !BAR_FLEXWINTITLE_PATCH
		#if RENAMED_SCRATCHPADS_PATCH
		if (c->scratchkey != 0 && c->isfloating)
			XSetWindowBorder(dpy, c->win, scheme[SchemeScratchSel][ColFloat].pixel);
		else if (c->scratchkey != 0)
			XSetWindowBorder(dpy, c->win, scheme[SchemeScratchSel][ColBorder].pixel);
		else if (c->isfloating)
			XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColFloat].pixel);
		else
			XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColBorder].pixel);
		#else
		if (c->isfloating)
			XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColFloat].pixel);
		else
			XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColBorder].pixel);
		#endif // RENAMED_SCRATCHPADS_PATCH
		#endif // BAR_FLEXWINTITLE_PATCH
		setfocus(c);
	} else {
		#if NODMENU_PATCH
		XSetInputFocus(dpy, selmon->bar && selmon->bar->win ? selmon->bar->win : root, RevertToPointerRoot, CurrentTime);
		#else
		XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
		#endif // NODMENU_PATCH
		XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
	}
	selmon->sel = c;
	drawbars();

	#if ON_EMPTY_KEYS_PATCH
	if ((isempty && selmon->sel) || (!isempty && !selmon->sel)) {
		isempty = !isempty;
		grabkeys();
	}
	#endif // ON_EMPTY_KEYS_PATCH
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
	#if LOSEFULLSCREEN_PATCH
	Client *sel;
	#endif // LOSEFULLSCREEN_PATCH

	if (!mons->next)
		return;
	if ((m = dirtomon(arg->i)) == selmon)
		return;
	#if LOSEFULLSCREEN_PATCH
	sel = selmon->sel;
	selmon = m;
	unfocus(sel, 0, NULL);
	#else
	unfocus(selmon->sel, 0, NULL);
	selmon = m;
	#endif // LOSEFULLSCREEN_PATCH
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

	#if LOSEFULLSCREEN_PATCH
	if (!selmon->sel)
		return;
	#elif FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
	if (!selmon->sel || (selmon->sel->isfullscreen && !selmon->sel->fakefullscreen))
		return;
	#else
	if (!selmon->sel || (selmon->sel->isfullscreen && lockfullscreen))
		return;
	#endif // LOSEFULLSCREEN_PATCH
	#if BAR_WINTITLEACTIONS_PATCH
	if (arg->i > 0) {
		for (c = selmon->sel->next; c && (!ISVISIBLE(c) || (arg->i == 1 && HIDDEN(c))); c = c->next);
		if (!c)
			for (c = selmon->clients; c && (!ISVISIBLE(c) || (arg->i == 1 && HIDDEN(c))); c = c->next);
	} else {
		for (i = selmon->clients; i != selmon->sel; i = i->next)
			if (ISVISIBLE(i) && !(arg->i == -1 && HIDDEN(i)))
				c = i;
		if (!c)
			for (; i; i = i->next)
				if (ISVISIBLE(i) && !(arg->i == -1 && HIDDEN(i)))
					c = i;
	}
	#else
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
	#endif // BAR_WINTITLEACTIONS_PATCH
	if (c) {
		focus(c);
		restack(selmon);
	}
}
#endif // STACKER_PATCH

Atom
getatomprop(Client *c, Atom prop, Atom req)
{
	int di;
	unsigned long dl;
	unsigned char *p = NULL;
	Atom da, atom = None;

	#if BAR_SYSTRAY_PATCH
	if (prop == xatom[XembedInfo])
		req = xatom[XembedInfo];
	#endif // BAR_SYSTRAY_PATCH

	/* FIXME getatomprop should return the number of items and a pointer to
	 * the stored data instead of this workaround */
	if (XGetWindowProperty(dpy, c->win, prop, 0L, sizeof atom, False, req,
		&da, &di, &dl, &dl, &p) == Success && p) {
		atom = *(Atom *)p;
		#if BAR_SYSTRAY_PATCH
		if (da == xatom[XembedInfo] && dl == 2)
			atom = ((Atom *)p)[1];
		#endif // BAR_SYSTRAY_PATCH
		XFree(p);
	}
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
	if (name.encoding == XA_STRING) {
		strncpy(text, (char *)name.value, size - 1);
	} else if (XmbTextPropertyToTextList(dpy, &name, &list, &n) >= Success && n > 0 && *list) {
		strncpy(text, *list, size - 1);
		XFreeStringList(list);
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
			if (buttons[i].click == ClkClientWin
			#if NO_MOD_BUTTONS_PATCH
				&& (nomodbuttons || buttons[i].mask != 0)
			#endif // NO_MOD_BUTTONS_PATCH
			)
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
		#if ON_EMPTY_KEYS_PATCH
		if (!selmon->sel)
			for (i = 0; i < LENGTH(on_empty_keys); i++)
				if ((code = XKeysymToKeycode(dpy, on_empty_keys[i].keysym)))
					for (j = 0; j < LENGTH(modifiers); j++)
						XGrabKey(dpy, code, on_empty_keys[i].mod | modifiers[j], root,
								True, GrabModeAsync, GrabModeAsync);
		#endif // ON_EMPTY_KEYS_PATCH
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
	int keysyms_return;
	KeySym* keysym;
	XKeyEvent *ev;

	ev = &e->xkey;
	keysym = XGetKeyboardMapping(dpy, (KeyCode)ev->keycode, 1, &keysyms_return);
	for (i = 0; i < LENGTH(keys); i++)
		if (*keysym == keys[i].keysym
				&& CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
				&& keys[i].func)
			keys[i].func(&(keys[i].arg));
	#if ON_EMPTY_KEYS_PATCH
	if (!selmon->sel)
		for (i = 0; i < LENGTH(on_empty_keys); i++)
			if (*keysym == on_empty_keys[i].keysym
					&& CLEANMASK(on_empty_keys[i].mod) == CLEANMASK(ev->state)
					&& on_empty_keys[i].func)
				on_empty_keys[i].func(&(on_empty_keys[i].arg));
	#endif // ON_EMPTY_KEYS_PATCH
	XFree(keysym);
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
	#if BAR_SYSTRAY_PATCH
	if (!sendevent(selmon->sel->win, wmatom[WMDelete], NoEventMask, wmatom[WMDelete], CurrentTime, 0, 0, 0))
	#else
	if (!sendevent(selmon->sel, wmatom[WMDelete]))
	#endif // BAR_SYSTRAY_PATCH
	{
		XGrabServer(dpy);
		XSetErrorHandler(xerrordummy);
		XSetCloseDownMode(dpy, DestroyAll);
		XKillClient(dpy, selmon->sel->win);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
		#if WARP_PATCH
		force_warp = 1;
		#endif // WARP_PATCH
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
	#if SEAMLESS_RESTART_PATCH
	int settings_restored;
	#endif // SEAMLESS_RESTART_PATCH
	Window trans = None;
	XWindowChanges wc;

	c = ecalloc(1, sizeof(Client));
	c->win = w;
	#if SWALLOW_PATCH
	c->pid = winpid(w);
	#endif // SWALLOW_PATCH
	/* geometry */
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	c->sfx = c->sfy = c->sfw = c->sfh = -9999;
	#endif // SAVEFLOATS_PATCH | EXRESIZE_PATCH
	c->x = c->oldx = wa->x;
	c->y = c->oldy = wa->y;
	c->w = c->oldw = wa->width;
	c->h = c->oldh = wa->height;
	c->oldbw = wa->border_width;
	#if CFACTS_PATCH
	c->cfact = 1.0;
	#endif // CFACTS_PATCH
	#if SEAMLESS_RESTART_PATCH
	settings_restored = restoreclientstate(c);
	#endif // SEAMLESS_RESTART_PATCH
	#if BAR_WINICON_PATCH
	updateicon(c);
	#endif // BAR_WINICON_PATCH
	updatetitle(c);

	#if XKB_PATCH
	/* Setting current xkb state must be before applyrules */
	if (!(c->xkb = findxkb(c->win)))
		c->xkb = createxkb(c->win);
	#endif // XKB_PATCH

	if (XGetTransientForHint(dpy, w, &trans) && (t = wintoclient(trans))) {
		c->mon = t->mon;
		c->tags = t->tags;
		#if SETBORDERPX_PATCH
		c->bw = c->mon->borderpx;
		#else
		c->bw = borderpx;
		#endif // SETBORDERPX_PATCH
		#if CENTER_TRANSIENT_WINDOWS_BY_PARENT_PATCH
		c->x = t->x + WIDTH(t) / 2 - WIDTH(c) / 2;
		c->y = t->y + HEIGHT(t) / 2 - HEIGHT(c) / 2;
		#elif CENTER_PATCH && CENTER_TRANSIENT_WINDOWS_PATCH
		c->iscentered = 1;
		#elif CENTER_TRANSIENT_WINDOWS_PATCH
		c->x = c->mon->wx + (c->mon->ww - WIDTH(c)) / 2;
		c->y = c->mon->wy + (c->mon->wh - HEIGHT(c)) / 2;
		#elif CENTER_PATCH
		if (c->x == c->mon->wx && c->y == c->mon->wy)
			c->iscentered = 1;
		#endif // CENTER_TRANSIENT_WINDOWS_PATCH | CENTER_TRANSIENT_WINDOWS_BY_PARENT_PATCH | CENTER_PATCH
	} else {
		#if SEAMLESS_RESTART_PATCH
		if (!settings_restored)
			c->mon = selmon;
		#else
		c->mon = selmon;
		#endif // SEAMLESS_RESTART_PATCH
		#if CENTER_PATCH
		if (c->x == c->mon->wx && c->y == c->mon->wy)
			c->iscentered = 1;
		#endif // CENTER_PATCH
		#if SETBORDERPX_PATCH
		c->bw = c->mon->borderpx;
		#else
		c->bw = borderpx;
		#endif // SETBORDERPX_PATCH
		#if SEAMLESS_RESTART_PATCH
		if (!settings_restored)
			applyrules(c);
		#else
		applyrules(c);
		#endif // SEAMLESS_RESTART_PATCH
		#if SWALLOW_PATCH
		term = termforwin(c);
		if (term)
			c->mon = term->mon;
		#endif // SWALLOW_PATCH
	}

	if (c->x + WIDTH(c) > c->mon->wx + c->mon->ww)
		c->x = c->mon->wx + c->mon->ww - WIDTH(c);
	if (c->y + HEIGHT(c) > c->mon->wy + c->mon->wh)
		c->y = c->mon->wy + c->mon->wh - HEIGHT(c);
	c->x = MAX(c->x, c->mon->wx);
	c->y = MAX(c->y, c->mon->wy);

	wc.border_width = c->bw;
	XConfigureWindow(dpy, w, CWBorderWidth, &wc);
	#if !BAR_FLEXWINTITLE_PATCH
	if (c->isfloating)
		XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColBorder].pixel);
	#endif // BAR_FLEXWINTITLE_PATCH
	configure(c); /* propagates border_width, if size doesn't change */
	updatesizehints(c);
	updatewmhints(c);
	#if DECORATION_HINTS_PATCH
	updatemotifhints(c);
	#endif // DECORATION_HINTS_PATCH

	#if CENTER_PATCH && SAVEFLOATS_PATCH || CENTER_PATCH && EXRESIZE_PATCH
	if (c->iscentered) {
		c->sfx = c->x = c->mon->wx + (c->mon->ww - WIDTH(c)) / 2;
		c->sfy = c->y = c->mon->wy + (c->mon->wh - HEIGHT(c)) / 2;
	}
	#elif CENTER_PATCH
	if (c->iscentered) {
		c->x = c->mon->wx + (c->mon->ww - WIDTH(c)) / 2;
		c->y = c->mon->wy + (c->mon->wh - HEIGHT(c)) / 2;
	}
	#elif ALWAYSCENTER_PATCH && SAVEFLOATS_PATCH || ALWAYSCENTER_PATCH && EXRESIZE_PATCH
	c->sfx = c->x = c->mon->wx + (c->mon->ww - WIDTH(c)) / 2;
	c->sfy = c->y = c->mon->wy + (c->mon->wh - HEIGHT(c)) / 2;
	#elif ALWAYSCENTER_PATCH
	c->x = c->mon->wx + (c->mon->ww - WIDTH(c)) / 2;
	c->y = c->mon->wy + (c->mon->wh - HEIGHT(c)) / 2;
	#endif // CENTER_PATCH / ALWAYSCENTER_PATCH / SAVEFLOATS_PATCH
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	if (c->sfw == -9999) {
		c->sfw = c->w;
		c->sfh = c->h;
	}
	#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH

	if (getatomprop(c, netatom[NetWMState], XA_ATOM) == netatom[NetWMFullscreen])
		setfullscreen(c, 1);

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
	if (c->isfloating) {
		XRaiseWindow(dpy, c->win);
		XSetWindowBorder(dpy, w, scheme[SchemeNorm][ColFloat].pixel);
	}
	#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH || SEAMLESS_RESTART_PATCH
	attachx(c);
	#else
	attach(c);
	#endif
	attachstack(c);
	XChangeProperty(dpy, root, netatom[NetClientList], XA_WINDOW, 32, PropModeAppend,
		(unsigned char *) &(c->win), 1);
	#if NET_CLIENT_LIST_STACKING_PATCH
	XChangeProperty(dpy, root, netatom[NetClientListStacking], XA_WINDOW, 32, PropModePrepend,
		(unsigned char *) &(c->win), 1);
	#endif // NET_CLIENT_LIST_STACKING_PATCH
	XMoveResizeWindow(dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h); /* some windows require this */

	#if BAR_WINTITLEACTIONS_PATCH
	if (!HIDDEN(c))
		setclientstate(c, NormalState);
	#else
	setclientstate(c, NormalState);
	#endif // BAR_WINTITLEACTIONS_PATCH
	if (c->mon == selmon)
		unfocus(selmon->sel, 0, c);
	c->mon->sel = c;
	#if SWALLOW_PATCH
	if (!(term && swallow(term, c))) {
		#if RIODRAW_PATCH
		if (riopid && (!riodraw_matchpid || isdescprocess(riopid, c->pid))) {
			if (riodimensions[3] != -1)
				rioposition(c, riodimensions[0], riodimensions[1], riodimensions[2], riodimensions[3]);
			else {
				killclient(&((Arg) { .v = c }));
				return;
			}
		}
		#endif // RIODRAW_PATCH
		arrange(c->mon);
		#if BAR_WINTITLEACTIONS_PATCH
		if (!HIDDEN(c))
			XMapWindow(dpy, c->win);
		#else
		XMapWindow(dpy, c->win);
		#endif // BAR_WINTITLEACTIONS_PATCH
	}
	#else
	#if RIODRAW_PATCH
	if (riopid) {
		if (riodimensions[3] != -1)
			rioposition(c, riodimensions[0], riodimensions[1], riodimensions[2], riodimensions[3]);
		else {
			killclient(&((Arg) { .v = c }));
			return;
		}
	}
	#endif // RIODRAW_PATCH
	arrange(c->mon);
	#if BAR_WINTITLEACTIONS_PATCH
	if (!HIDDEN(c))
		XMapWindow(dpy, c->win);
	#else
	XMapWindow(dpy, c->win);
	#endif // BAR_WINTITLEACTIONS_PATCH
	#endif // SWALLOW_PATCH
	focus(NULL);

	#if BAR_EWMHTAGS_PATCH
	setfloatinghint(c);
	#endif // BAR_EWMHTAGS_PATCH
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

	#if BAR_SYSTRAY_PATCH
	Client *i;
	if (showsystray && systray && (i = wintosystrayicon(ev->window))) {
		sendevent(i->win, netatom[Xembed], StructureNotifyMask, CurrentTime, XEMBED_WINDOW_ACTIVATE, 0, systray->win, XEMBED_EMBEDDED_VERSION);
		drawbarwin(systray->bar);
	}
	#endif // BAR_SYSTRAY_PATCH

	if (!XGetWindowAttributes(dpy, ev->window, &wa) || wa.override_redirect)
		return;
	#if BAR_ANYBAR_PATCH
	if (wmclasscontains(ev->window, altbarclass, ""))
		managealtbar(ev->window, &wa);
	else
	#endif // BAR_ANYBAR_PATCH
	if (!wintoclient(ev->window))
		manage(ev->window, &wa);
}

#if !FOCUSONCLICK_PATCH
void
motionnotify(XEvent *e)
{
	static Monitor *mon = NULL;
	Monitor *m;
	Bar *bar;
	#if LOSEFULLSCREEN_PATCH
	Client *sel;
	#endif // LOSEFULLSCREEN_PATCH
	XMotionEvent *ev = &e->xmotion;

	if ((bar = wintobar(ev->window))) {
		barhover(e, bar);
		return;
	}

	#if BAR_TAGPREVIEW_PATCH
	if (selmon->previewshow != 0)
		hidetagpreview(selmon);
	#endif // BAR_TAGPREVIEW_PATCH

	if (ev->window != root)
		return;
	if ((m = recttomon(ev->x_root, ev->y_root, 1, 1)) != mon && mon) {
		#if LOSEFULLSCREEN_PATCH
		sel = selmon->sel;
		selmon = m;
		unfocus(sel, 1, NULL);
		#else
		unfocus(selmon->sel, 1, NULL);
		selmon = m;
		#endif // LOSEFULLSCREEN_PATCH
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
	#if FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
	if (c->isfullscreen && c->fakefullscreen != 1) /* no support moving fullscreen windows by mouse */
		return;
	#else
	if (c->isfullscreen) /* no support moving fullscreen windows by mouse */
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // FAKEFULLSCREEN_PATCH
	restack(selmon);
	nx = ocx = c->x;
	ny = ocy = c->y;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurMove]->cursor, CurrentTime) != GrabSuccess)
		return;
	if (!getrootptr(&x, &y))
		return;
	ignoreconfigurerequests = 1;
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
			&& (abs(nx - c->x) > snap || abs(ny - c->y) > snap)) {
				#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
				c->sfx = -9999; // disable savefloats when using movemouse
				#endif // SAVEFLOATS_PATCH | EXRESIZE_PATCH
				togglefloating(NULL);
			}
			if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
				resize(c, nx, ny, c->w, c->h, 1);
			}
			break;
		}
	} while (ev.type != ButtonRelease);

	XUngrabPointer(dpy, CurrentTime);
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if (c->tags & SPTAGMASK) {
			c->mon->tagset[c->mon->seltags] ^= (c->tags & SPTAGMASK);
			m->tagset[m->seltags] |= (c->tags & SPTAGMASK);
		}
		#endif // SCRATCHPADS_PATCH
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	/* save last known float coordinates */
	if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
		c->sfx = nx;
		c->sfy = ny;
	}
	#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
	ignoreconfigurerequests = 0;
}

Client *
nexttiled(Client *c)
{
	#if BAR_WINTITLEACTIONS_PATCH
	for (; c && (c->isfloating || !ISVISIBLE(c) || HIDDEN(c)); c = c->next);
	#else
	for (; c && (c->isfloating || !ISVISIBLE(c)); c = c->next);
	#endif // BAR_WINTITLEACTIONS_PATCH
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

	#if BAR_SYSTRAY_PATCH
	if (showsystray && (c = wintosystrayicon(ev->window))) {
		if (ev->atom == XA_WM_NORMAL_HINTS) {
			updatesizehints(c);
			updatesystrayicongeom(c, c->w, c->h);
		}
		else
			updatesystrayiconstate(c, ev);
		drawbarwin(systray->bar);
	}
	#endif // BAR_SYSTRAY_PATCH

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
			c->hintsvalid = 0;
			break;
		case XA_WM_HINTS:
			updatewmhints(c);
			if (c->isurgent)
				drawbars();
			#if TAB_PATCH
			drawtabs();
			#endif // TAB_PATCH
			break;
		}
		if (ev->atom == XA_WM_NAME || ev->atom == netatom[NetWMName]) {
			updatetitle(c);
			if (c == c->mon->sel)
				drawbar(c->mon);
			#if TAB_PATCH
			drawtab(c->mon);
			#endif // TAB_PATCH
		}
		#if DECORATION_HINTS_PATCH
		if (ev->atom == motifatom)
			updatemotifhints(c);
		#endif // DECORATION_HINTS_PATCH
		#if BAR_WINICON_PATCH
		else if (ev->atom == netatom[NetWMIcon]) {
			updateicon(c);
			if (c == c->mon->sel)
				drawbar(c->mon);
		}
		#endif // BAR_WINICON_PATCH
	}
}

void
quit(const Arg *arg)
{
	#if RESTARTSIG_PATCH
	restart = arg->i;
	#endif // RESTARTSIG_PATCH
	#if ONLYQUITONEMPTY_PATCH
	Monitor *m;
	Client *c;
	unsigned int n = 0;

	for (m = mons; m; m = m->next)
		for (c = m->clients; c; c = c->next, n++);

	#if RESTARTSIG_PATCH
	if (restart || n <= quit_empty_window_count)
	#else
	if (n <= quit_empty_window_count)
	#endif // RESTARTSIG_PATCH
		running = 0;
	else
		fprintf(stderr, "[dwm] not exiting (n=%d)\n", n);

	#else // !ONLYQUITONEMPTY_PATCH
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
	#if ROUNDED_CORNERS_PATCH
	drawroundedcorners(c);
	#endif // ROUNDED_CORNERS_PATCH
	#if NOBORDER_PATCH
	if (((nexttiled(c->mon->clients) == c && !nexttiled(c->next))
		#if MONOCLE_LAYOUT
		|| &monocle == c->mon->lt[c->mon->sellt]->arrange
		#endif // MONOCLE_LAYOUT
		#if DECK_LAYOUT
		|| (&deck == c->mon->lt[c->mon->sellt]->arrange &&
			c->mon->nmaster == 0)
		#endif // DECK_LAYOUT
		#if FLEXTILE_DELUXE_LAYOUT
		|| (&flextile == c->mon->lt[c->mon->sellt]->arrange && (
			(c->mon->ltaxis[LAYOUT] == NO_SPLIT &&
			 c->mon->ltaxis[MASTER] == MONOCLE) ||
			(c->mon->ltaxis[STACK] == MONOCLE &&
			 c->mon->nmaster == 0)))
		#endif //FLEXTILE_DELUXE_LAYOUT
		)
		#if FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
		&& (c->fakefullscreen == 1 || !c->isfullscreen)
		#else
		&& !c->isfullscreen
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
		&& !c->isfloating
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
	int ocx, ocy, nw, nh, nx, ny;
	#if RESIZEPOINT_PATCH || RESIZECORNERS_PATCH
	int opx, opy, och, ocw;
	int horizcorner, vertcorner;
	unsigned int dui;
	Window dummy;
	#endif // RESIZEPOINT_PATCH | RESIZECORNERS_PATCH
	Client *c;
	Monitor *m;
	XEvent ev;
	Time lasttime = 0;

	if (!(c = selmon->sel))
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfullscreen && c->fakefullscreen != 1) /* no support resizing fullscreen windows by mouse */
		return;
	#else
	if (c->isfullscreen) /* no support resizing fullscreen windows by mouse */
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // !FAKEFULLSCREEN_PATCH
	restack(selmon);
	nx = ocx = c->x;
	ny = ocy = c->y;
	nh = c->h;
	nw = c->w;
	#if RESIZEPOINT_PATCH
	och = c->h;
	ocw = c->w;
	#elif RESIZECORNERS_PATCH
	och = c->y + c->h;
	ocw = c->x + c->w;
	#endif // RESIZEPOINT_PATCH | RESIZECORNERS_PATCH
	#if RESIZEPOINT_PATCH || RESIZECORNERS_PATCH
	if (!XQueryPointer(dpy, c->win, &dummy, &dummy, &opx, &opy, &nx, &ny, &dui))
		return;
	horizcorner = nx < c->w / 2;
	vertcorner  = ny < c->h / 2;
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[horizcorner | (vertcorner << 1)]->cursor, CurrentTime) != GrabSuccess)
		return;
	#if RESIZECORNERS_PATCH
	XWarpPointer (dpy, None, c->win, 0, 0, 0, 0,
			horizcorner ? (-c->bw) : (c->w + c->bw - 1),
			vertcorner ? (-c->bw) : (c->h + c->bw - 1));
	#endif // RESIZECORNERS_PATCH
	#else
	if (XGrabPointer(dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
		None, cursor[CurResize]->cursor, CurrentTime) != GrabSuccess)
		return;
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
	#endif // RESIZEPOINT_PATCH | RESIZECORNERS_PATCH
	ignoreconfigurerequests = 1;
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

			#if RESIZEPOINT_PATCH
			nx = horizcorner ? (ocx + ev.xmotion.x - opx) : c->x;
			ny = vertcorner ? (ocy + ev.xmotion.y - opy) : c->y;
			nw = MAX(horizcorner ? (ocx + ocw - nx) : (ocw + (ev.xmotion.x - opx)), 1);
			nh = MAX(vertcorner ? (ocy + och - ny) : (och + (ev.xmotion.y - opy)), 1);
			#elif RESIZECORNERS_PATCH
			nx = horizcorner ? ev.xmotion.x : c->x;
			ny = vertcorner ? ev.xmotion.y : c->y;
			nw = MAX(horizcorner ? (ocw - nx) : (ev.xmotion.x - ocx - 2 * c->bw + 1), 1);
			nh = MAX(vertcorner ? (och - ny) : (ev.xmotion.y - ocy - 2 * c->bw + 1), 1);
			#else
			nw = MAX(ev.xmotion.x - ocx - 2 * c->bw + 1, 1);
			nh = MAX(ev.xmotion.y - ocy - 2 * c->bw + 1, 1);
			#endif // RESIZEPOINT_PATCH | RESIZECORNERS_PATCH
			if (c->mon->wx + nw >= selmon->wx && c->mon->wx + nw <= selmon->wx + selmon->ww
			&& c->mon->wy + nh >= selmon->wy && c->mon->wy + nh <= selmon->wy + selmon->wh)
			{
				if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
				&& (abs(nw - c->w) > snap || abs(nh - c->h) > snap)) {
					#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
					c->sfx = -9999; // disable savefloats when using resizemouse
					#endif // SAVEFLOATS_PATCH | EXRESIZE_PATCH
					togglefloating(NULL);
				}
			}
			if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
				resize(c, nx, ny, nw, nh, 1);
			}
			break;
		}
	} while (ev.type != ButtonRelease);

	#if !RESIZEPOINT_PATCH
	#if RESIZECORNERS_PATCH
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0,
			horizcorner ? (-c->bw) : (c->w + c->bw - 1),
			vertcorner ? (-c->bw) : (c->h + c->bw - 1));
	#else
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
	#endif // RESIZECORNERS_PATCH
	#endif // RESIZEPOINT_PATCH
	XUngrabPointer(dpy, CurrentTime);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
	if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if (c->tags & SPTAGMASK) {
			c->mon->tagset[c->mon->seltags] ^= (c->tags & SPTAGMASK);
			m->tagset[m->seltags] |= (c->tags & SPTAGMASK);
		}
		#endif // SCRATCHPADS_PATCH
		sendmon(c, m);
		selmon = m;
		focus(NULL);
	}
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	/* save last known float dimensions */
	if (!selmon->lt[selmon->sellt]->arrange || c->isfloating) {
		c->sfx = nx;
		c->sfy = ny;
		c->sfw = nw;
		c->sfh = nh;
	}
	#endif // SAVEFLOATS_PATCH | EXRESIZE_PATCH
	ignoreconfigurerequests = 0;
}

void
restack(Monitor *m)
{
	Client *c, *f = NULL;
	XEvent ev;
	XWindowChanges wc;
	#if WARP_PATCH && FLEXTILE_DELUXE_LAYOUT
	int n;
	#endif // WARP_PATCH

	drawbar(m);
	#if TAB_PATCH
	drawtab(m);
	#endif // TAB_PATCH
	if (!m->sel)
		return;
	if (m->sel->isfloating || !m->lt[m->sellt]->arrange)
		XRaiseWindow(dpy, m->sel->win);
	if (m->lt[m->sellt]->arrange) {
		wc.stack_mode = Below;
		if (m->bar) {
			wc.sibling = m->bar->win;
		} else {
			for (f = m->stack; f && (f->isfloating || !ISVISIBLE(f)); f = f->snext); // find first tiled stack client
			if (f)
				wc.sibling = f->win;
		}
		for (c = m->stack; c; c = c->snext)
			if (!c->isfloating && ISVISIBLE(c) && c != f) {
				XConfigureWindow(dpy, c->win, CWSibling|CWStackMode, &wc);
				wc.sibling = c->win;
			}
	}
	XSync(dpy, False);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
	#if WARP_PATCH && FLEXTILE_DELUXE_LAYOUT || WARP_PATCH && MONOCLE_LAYOUT
	#if FLEXTILE_DELUXE_LAYOUT
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	#endif // FLEXTILE_DELUXE_LAYOUT
	if (m == selmon && (m->tagset[m->seltags] & m->sel->tags) && (
		#if MONOCLE_LAYOUT && FLEXTILE_DELUXE_LAYOUT
		(m->lt[m->sellt]->arrange != &monocle
		&& !(m->ltaxis[MASTER] == MONOCLE && (abs(m->ltaxis[LAYOUT] == NO_SPLIT || !m->nmaster || n <= m->nmaster))))
		#elif MONOCLE_LAYOUT
		m->lt[m->sellt]->arrange != &monocle
		#else
		!(m->ltaxis[MASTER] == MONOCLE && (abs(m->ltaxis[LAYOUT] == NO_SPLIT || !m->nmaster || n <= m->nmaster)))
		#endif // FLEXTILE_DELUXE_LAYOUT
		|| m->sel->isfloating)
	)
		warp(m->sel);
	#endif // WARP_PATCH
}

#if IPC_PATCH
void
run(void)
{
	int event_count = 0;
	const int MAX_EVENTS = 10;
	struct epoll_event events[MAX_EVENTS];

	XSync(dpy, False);

	/* main event loop */
	while (running) {
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

		for (int i = 0; i < event_count; i++) {
			int event_fd = events[i].data.fd;
			DEBUG("Got event from fd %d\n", event_fd);

			if (event_fd == dpy_fd) {
				// -1 means EPOLLHUP
				if (handlexevent(events + i) == -1)
					return;
			} else if (event_fd == ipc_get_sock_fd()) {
				ipc_handle_socket_epoll_event(events + i);
			} else if (ipc_is_client_registered(event_fd)) {
				if (ipc_handle_client_epoll_event(events + i, mons, &lastselmon, selmon,
						NUMTAGS, layouts, LENGTH(layouts)) < 0) {
					fprintf(stderr, "Error handling IPC event on fd %d\n", event_fd);
				}
			} else {
				fprintf(stderr, "Got event from unknown fd %d, ptr %p, u32 %d, u64 %lu",
				event_fd, events[i].data.ptr, events[i].data.u32,
				events[i].data.u64);
				fprintf(stderr, " with events %d\n", events[i].events);
				return;
			}
		}
	}
}
#elif RESTARTSIG_PATCH
void
run(void)
{
	XEvent ev;
	XSync(dpy, False);
	/* main event loop */
	while (running) {
		struct pollfd pfd = {
			.fd = ConnectionNumber(dpy),
			.events = POLLIN,
		};
		int pending = XPending(dpy) > 0 || poll(&pfd, 1, -1) > 0;

		if (!running)
			break;
		if (!pending)
			continue;

		XNextEvent(dpy, &ev);
		if (handler[ev.type])
			handler[ev.type](&ev); /* call handler */
	}
}
#else
void
run(void)
{
	XEvent ev;
	/* main event loop */
	XSync(dpy, False);
	while (running && !XNextEvent(dpy, &ev)) {

		#if XKB_PATCH
		/* Unfortunately the xkbEventType is not constant hence it can't be part of the
		 * normal event handler below */
		if (ev.type == xkbEventType) {
			xkbeventnotify(&ev);
			continue;
		}
		#endif // XKB_PATCH

		if (handler[ev.type])
			handler[ev.type](&ev); /* call handler */
	}
}
#endif // IPC_PATCH | RESTARTSIG_PATCH

void
scan(void)
{
	#if SWALLOW_PATCH
	scanner = 1;
	char swin[256];
	#endif // SWALLOW_PATCH
	unsigned int i, num;
	Window d1, d2, *wins = NULL;
	XWindowAttributes wa;

	if (XQueryTree(dpy, root, &d1, &d2, &wins, &num)) {
		for (i = 0; i < num; i++) {
			if (!XGetWindowAttributes(dpy, wins[i], &wa)
			|| wa.override_redirect || XGetTransientForHint(dpy, wins[i], &d1))
				continue;
			#if BAR_ANYBAR_PATCH
			if (wmclasscontains(wins[i], altbarclass, ""))
				managealtbar(wins[i], &wa);
			else
			#endif // BAR_ANYBAR_PATCH
			if (wa.map_state == IsViewable || getstate(wins[i]) == IconicState)
				manage(wins[i], &wa);
			#if SWALLOW_PATCH
			else if (gettextprop(wins[i], netatom[NetClientList], swin, sizeof swin))
				manage(wins[i], &wa);
			#endif // SWALLOW_PATCH
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
	#if SWALLOW_PATCH
	scanner = 0;
	#endif // SWALLOW_PATCH
}

void
sendmon(Client *c, Monitor *m)
{
	#if EXRESIZE_PATCH
	Monitor *oldm = selmon;
	#endif // EXRESIZE_PATCH
	if (c->mon == m)
		return;
	#if SENDMON_KEEPFOCUS_PATCH && !EXRESIZE_PATCH
	int hadfocus = (c == selmon->sel);
	#endif // SENDMON_KEEPFOCUS_PATCH
	unfocus(c, 1, NULL);
	detach(c);
	detachstack(c);
	#if SENDMON_KEEPFOCUS_PATCH && !EXRESIZE_PATCH
	arrange(c->mon);
	#endif // SENDMON_KEEPFOCUS_PATCH
	c->mon = m;
	#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
	if (!(c->tags & SPTAGMASK))
	#endif // SCRATCHPADS_PATCH
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
	#elif SENDMON_KEEPFOCUS_PATCH
	arrange(m);
	if (hadfocus) {
		focus(c);
		restack(m);
	} else
		focus(NULL);
	#else
	focus(NULL);
	arrange(NULL);
	#endif // EXRESIZE_PATCH / SENDMON_KEEPFOCUS_PATCH
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
#if BAR_SYSTRAY_PATCH
sendevent(Window w, Atom proto, int mask, long d0, long d1, long d2, long d3, long d4)
#else
sendevent(Client *c, Atom proto)
#endif // BAR_SYSTRAY_PATCH
{
	int n;
	Atom *protocols;
	#if BAR_SYSTRAY_PATCH
	Atom mt;
	#endif // BAR_SYSTRAY_PATCH
	int exists = 0;
	XEvent ev;

	#if BAR_SYSTRAY_PATCH
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
	#endif // BAR_SYSTRAY_PATCH

	if (exists) {
		#if BAR_SYSTRAY_PATCH
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
		#endif // BAR_SYSTRAY_PATCH
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
		#if XKB_PATCH
		XkbLockGroup(dpy, XkbUseCoreKbd, c->xkb->group);
		#endif // XKB_PATCH
	}
	#if BAR_SYSTRAY_PATCH
	sendevent(c->win, wmatom[WMTakeFocus], NoEventMask, wmatom[WMTakeFocus], CurrentTime, 0, 0, 0);
	#else
	sendevent(c, wmatom[WMTakeFocus]);
	#endif // BAR_SYSTRAY_PATCH
}

#if FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
void
setfullscreen(Client *c, int fullscreen)
{
	XEvent ev;
	int savestate = 0, restorestate = 0;

	if ((c->fakefullscreen == 0 && fullscreen && !c->isfullscreen) // normal fullscreen
			|| (c->fakefullscreen == 2 && fullscreen)) // fake fullscreen --> actual fullscreen
		savestate = 1; // go actual fullscreen
	else if ((c->fakefullscreen == 0 && !fullscreen && c->isfullscreen) // normal fullscreen exit
			|| (c->fakefullscreen >= 2 && !fullscreen)) // fullscreen exit --> fake fullscreen
		restorestate = 1; // go back into tiled

	/* If leaving fullscreen and the window was previously fake fullscreen (2), then restore
	 * that while staying in fullscreen. The exception to this is if we are in said state, but
	 * the client itself disables fullscreen (3) then we let the client go out of fullscreen
	 * while keeping fake fullscreen enabled (as otherwise there will be a mismatch between the
	 * client and the window manager's perception of the client's fullscreen state). */
	if (c->fakefullscreen == 2 && !fullscreen && c->isfullscreen) {
		c->fakefullscreen = 1;
		c->isfullscreen = 1;
		fullscreen = 1;
	} else if (c->fakefullscreen == 3) // client exiting actual fullscreen
		c->fakefullscreen = 1;

	if (fullscreen != c->isfullscreen) { // only send property change if necessary
		if (fullscreen)
			XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
				PropModeReplace, (unsigned char*)&netatom[NetWMFullscreen], 1);
		else
			XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
				PropModeReplace, (unsigned char*)0, 0);
	}

	c->isfullscreen = fullscreen;

	/* Some clients, e.g. firefox, will send a client message informing the window manager
	 * that it is going into fullscreen after receiving the above signal. This has the side
	 * effect of this function (setfullscreen) sometimes being called twice when toggling
	 * fullscreen on and off via the window manager as opposed to the application itself.
	 * To protect against obscure issues where the client settings are stored or restored
	 * when they are not supposed to we add an additional bit-lock on the old state so that
	 * settings can only be stored and restored in that precise order. */
	if (savestate && !(c->oldstate & (1 << 1))) {
		c->oldbw = c->bw;
		c->oldstate = c->isfloating | (1 << 1);
		c->bw = 0;
		c->isfloating = 1;
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
	} else if (restorestate && (c->oldstate & (1 << 1))) {
		c->bw = c->oldbw;
		c->isfloating = c->oldstate = c->oldstate & 1;
		c->x = c->oldx;
		c->y = c->oldy;
		c->w = c->oldw;
		c->h = c->oldh;
		resizeclient(c, c->x, c->y, c->w, c->h);
		restack(c->mon);
	} else
		resizeclient(c, c->x, c->y, c->w, c->h);

	/* Exception: if the client was in actual fullscreen and we exit out to fake fullscreen
	 * mode, then the focus would sometimes drift to whichever window is under the mouse cursor
	 * at the time. To avoid this we ask X for all EnterNotify events and just ignore them.
	 */
	if (!c->isfullscreen)
		while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
#else
void
setfullscreen(Client *c, int fullscreen)
{
	if (fullscreen && !c->isfullscreen) {
		XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
			PropModeReplace, (unsigned char*)&netatom[NetWMFullscreen], 1);
		c->isfullscreen = 1;
		#if !FAKEFULLSCREEN_PATCH
		c->oldbw = c->bw;
		c->oldstate = c->isfloating;
		c->bw = 0;
		c->isfloating = 1;
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
		#endif // !FAKEFULLSCREEN_PATCH
	} else if (!fullscreen && c->isfullscreen){
		XChangeProperty(dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
			PropModeReplace, (unsigned char*)0, 0);
		c->isfullscreen = 0;
		#if !FAKEFULLSCREEN_PATCH
		c->bw = c->oldbw;
		c->isfloating = c->oldstate;
		c->x = c->oldx;
		c->y = c->oldy;
		c->w = c->oldw;
		c->h = c->oldh;
		resizeclient(c, c->x, c->y, c->w, c->h);
		arrange(c->mon);
		#endif // !FAKEFULLSCREEN_PATCH
	}
	#if FAKEFULLSCREEN_PATCH
	resizeclient(c, c->x, c->y, c->w, c->h);
	#endif // FAKEFULLSCREEN_PATCH
}
#endif // FAKEFULLSCREEN_CLIENT_PATCH

void
setlayout(const Arg *arg)
{
	#if !TOGGLELAYOUT_PATCH
	if (!arg || !arg->v || arg->v != selmon->lt[selmon->sellt]) {
	#endif // TOGGLELAYOUT_PATCH
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
	#if !TOGGLELAYOUT_PATCH
	}
	#endif // TOGGLELAYOUT_PATCH
	#if TOGGLELAYOUT_PATCH
	if (arg && arg->v && arg->v != selmon->lt[selmon->sellt ^ 1])
	#else
	if (arg && arg->v)
	#endif // TOGGLELAYOUT_PATCH
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
	#if XKB_PATCH
	XkbStateRec xkbstate;
	#endif // XKB_PATCH
	Atom utf8string;

	/* clean up any zombies immediately */
	sigchld(0);

	#if RESTARTSIG_PATCH
	signal(SIGHUP, sighup);
	signal(SIGTERM, sigterm);
	#endif // RESTARTSIG_PATCH

	/* the one line of bloat that would have saved a lot of time for a lot of people */
	putenv("_JAVA_AWT_WM_NONREPARENTING=1");

	/* init screen */
	screen = DefaultScreen(dpy);
	sw = DisplayWidth(dpy, screen);
	sh = DisplayHeight(dpy, screen);
	root = RootWindow(dpy, screen);
	#if BAR_ALPHA_PATCH
	xinitvisual();
	drw = drw_create(dpy, screen, root, sw, sh, visual, depth, cmap);
	#else
	drw = drw_create(dpy, screen, root, sw, sh);
	#endif // BAR_ALPHA_PATCH
	#if BAR_PANGO_PATCH
	if (!drw_font_create(drw, font))
	#else
	if (!drw_fontset_create(drw, fonts, LENGTH(fonts)))
	#endif // BAR_PANGO_PATCH
		die("no fonts could be loaded.");
	#if BAR_STATUSPADDING_PATCH
	lrpad = drw->fonts->h + horizpadbar;
	bh = drw->fonts->h + vertpadbar;
	#else
	lrpad = drw->fonts->h;
	#if BAR_HEIGHT_PATCH
	bh = bar_height ? bar_height : drw->fonts->h + 2;
	#else
	bh = drw->fonts->h + 2;
	#endif // BAR_HEIGHT_PATCH
	#endif // BAR_STATUSPADDING_PATCH
	#if TAB_PATCH
	th = bh;
	#endif // TAB_PATCH
	updategeom();
	/* init atoms */
	utf8string = XInternAtom(dpy, "UTF8_STRING", False);
	wmatom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
	wmatom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	wmatom[WMState] = XInternAtom(dpy, "WM_STATE", False);
	wmatom[WMTakeFocus] = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
	#if WINDOWROLERULE_PATCH
	wmatom[WMWindowRole] = XInternAtom(dpy, "WM_WINDOW_ROLE", False);
	#endif // WINDOWROLERULE_PATCH
	#if SEAMLESS_RESTART_PATCH
	clientatom[ClientFields] = XInternAtom(dpy, "_DWM_CLIENT_FIELDS", False);
	clientatom[ClientTags] = XInternAtom(dpy, "_DWM_CLIENT_TAGS", False);
	#endif // SEAMLESS_RESTART_PATCH
	netatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
	netatom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
	#if BAR_SYSTRAY_PATCH
	netatom[NetSystemTray] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_S0", False);
	netatom[NetSystemTrayOP] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_OPCODE", False);
	netatom[NetSystemTrayOrientation] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_ORIENTATION", False);
	netatom[NetSystemTrayOrientationHorz] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_ORIENTATION_HORZ", False);
	netatom[NetSystemTrayVisual] = XInternAtom(dpy, "_NET_SYSTEM_TRAY_VISUAL", False);
	netatom[NetWMWindowTypeDock] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DOCK", False);
	xatom[Manager] = XInternAtom(dpy, "MANAGER", False);
	xatom[Xembed] = XInternAtom(dpy, "_XEMBED", False);
	xatom[XembedInfo] = XInternAtom(dpy, "_XEMBED_INFO", False);
	#endif // BAR_SYSTRAY_PATCH
	#if BAR_EWMHTAGS_PATCH
	netatom[NetDesktopViewport] = XInternAtom(dpy, "_NET_DESKTOP_VIEWPORT", False);
	netatom[NetNumberOfDesktops] = XInternAtom(dpy, "_NET_NUMBER_OF_DESKTOPS", False);
	netatom[NetCurrentDesktop] = XInternAtom(dpy, "_NET_CURRENT_DESKTOP", False);
	netatom[NetDesktopNames] = XInternAtom(dpy, "_NET_DESKTOP_NAMES", False);
	#endif // BAR_EWMHTAGS_PATCH
	#if BAR_WINICON_PATCH
	netatom[NetWMIcon] = XInternAtom(dpy, "_NET_WM_ICON", False);
	#endif // BAR_WINICON_PATCH
	netatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
	netatom[NetWMState] = XInternAtom(dpy, "_NET_WM_STATE", False);
	netatom[NetWMCheck] = XInternAtom(dpy, "_NET_SUPPORTING_WM_CHECK", False);
	netatom[NetWMFullscreen] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
	netatom[NetWMWindowType] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
	netatom[NetClientList] = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
	#if NET_CLIENT_LIST_STACKING_PATCH
	netatom[NetClientListStacking] = XInternAtom(dpy, "_NET_CLIENT_LIST_STACKING", False);
	#endif // NET_CLIENT_LIST_STACKING_PATCH
	#if DECORATION_HINTS_PATCH
	motifatom = XInternAtom(dpy, "_MOTIF_WM_HINTS", False);
	#endif // DECORATION_HINTS_PATCH
	/* init cursors */
	cursor[CurNormal] = drw_cur_create(drw, XC_left_ptr);
	cursor[CurResize] = drw_cur_create(drw, XC_sizing);
	#if RESIZEPOINT_PATCH || RESIZECORNERS_PATCH
	cursor[CurResizeBR] = drw_cur_create(drw, XC_bottom_right_corner);
	cursor[CurResizeBL] = drw_cur_create(drw, XC_bottom_left_corner);
	cursor[CurResizeTR] = drw_cur_create(drw, XC_top_right_corner);
	cursor[CurResizeTL] = drw_cur_create(drw, XC_top_left_corner);
	#endif // RESIZEPOINT_PATCH | RESIZECORNERS_PATCH
	#if DRAGMFACT_PATCH
	cursor[CurResizeHorzArrow] = drw_cur_create(drw, XC_sb_h_double_arrow);
	cursor[CurResizeVertArrow] = drw_cur_create(drw, XC_sb_v_double_arrow);
	#endif // DRAGMFACT_PATCH
	#if DRAGCFACT_PATCH
	cursor[CurIronCross] = drw_cur_create(drw, XC_iron_cross);
	#endif // DRAGCFACT_PATCH
	cursor[CurMove] = drw_cur_create(drw, XC_fleur);
	/* init appearance */
	#if BAR_VTCOLORS_PATCH
	get_vt_colors();
	if (get_luminance(colors[SchemeTagsNorm][ColBg]) > 50) {
		strcpy(colors[SchemeTitleNorm][ColBg], title_bg_light);
		strcpy(colors[SchemeTitleSel][ColBg], title_bg_light);
	} else {
		strcpy(colors[SchemeTitleNorm][ColBg], title_bg_dark);
		strcpy(colors[SchemeTitleSel][ColBg], title_bg_dark);
	}
	#endif // BAR_VTCOLORS_PATCH
	#if BAR_STATUS2D_PATCH && !BAR_STATUSCOLORS_PATCH
	scheme = ecalloc(LENGTH(colors) + 1, sizeof(Clr *));
	#if BAR_ALPHA_PATCH
	scheme[LENGTH(colors)] = drw_scm_create(drw, colors[0], alphas[0], ColCount);
	#else
	scheme[LENGTH(colors)] = drw_scm_create(drw, colors[0], ColCount);
	#endif // BAR_ALPHA_PATCH
	#else
	scheme = ecalloc(LENGTH(colors), sizeof(Clr *));
	#endif // BAR_STATUS2D_PATCH
	for (i = 0; i < LENGTH(colors); i++)
		#if BAR_ALPHA_PATCH
		scheme[i] = drw_scm_create(drw, colors[i], alphas[i], ColCount);
		#else
		scheme[i] = drw_scm_create(drw, colors[i], ColCount);
		#endif // BAR_ALPHA_PATCH
	#if BAR_POWERLINE_STATUS_PATCH
	statusscheme = ecalloc(LENGTH(statuscolors), sizeof(Clr *));
	for (i = 0; i < LENGTH(statuscolors); i++)
		#if BAR_ALPHA_PATCH
		statusscheme[i] = drw_scm_create(drw, statuscolors[i], alphas[0], ColCount);
		#else
		statusscheme[i] = drw_scm_create(drw, statuscolors[i], ColCount);
		#endif // BAR_ALPHA_PATCH
	#endif // BAR_POWERLINE_STATUS_PATCH

	updatebars();
	updatestatus();
	#if BAR_TAGPREVIEW_PATCH
	updatepreview();
	#endif // BAR_TAGPREVIEW_PATCH

	/* supporting window for NetWMCheck */
	wmcheckwin = XCreateSimpleWindow(dpy, root, 0, 0, 1, 1, 0, 0, 0);
	XChangeProperty(dpy, wmcheckwin, netatom[NetWMCheck], XA_WINDOW, 32,
		PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	#if LG3D_PATCH
	XChangeProperty(dpy, wmcheckwin, netatom[NetWMName], utf8string, 8,
		PropModeReplace, (unsigned char *) "LG3D", 4);
	#else
	XChangeProperty(dpy, wmcheckwin, netatom[NetWMName], utf8string, 8,
		PropModeReplace, (unsigned char *) "dwm", 3);
	#endif // LG3D_PATCH
	XChangeProperty(dpy, root, netatom[NetWMCheck], XA_WINDOW, 32,
		PropModeReplace, (unsigned char *) &wmcheckwin, 1);
	/* EWMH support per view */
	XChangeProperty(dpy, root, netatom[NetSupported], XA_ATOM, 32,
		PropModeReplace, (unsigned char *) netatom, NetLast);
	#if BAR_EWMHTAGS_PATCH
	setnumdesktops();
	setcurrentdesktop();
	setdesktopnames();
	setviewport();
	#endif // BAR_EWMHTAGS_PATCH
	XDeleteProperty(dpy, root, netatom[NetClientList]);
	#if NET_CLIENT_LIST_STACKING_PATCH
	XDeleteProperty(dpy, root, netatom[NetClientListStacking]);
	#endif // NET_CLIENT_LIST_STACKING_PATCH
	/* select events */
	wa.cursor = cursor[CurNormal]->cursor;
	wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask
		|ButtonPressMask|PointerMotionMask|EnterWindowMask
		|LeaveWindowMask|StructureNotifyMask|PropertyChangeMask;
	XChangeWindowAttributes(dpy, root, CWEventMask|CWCursor, &wa);
	XSelectInput(dpy, root, wa.event_mask);

	#if XKB_PATCH
	/* get xkb extension info, events and current state */
	if (!XkbQueryExtension(dpy, NULL, &xkbEventType, NULL, NULL, NULL))
		fputs("warning: can not query xkb extension\n", stderr);
	XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbStateNotify,
	                      XkbAllStateComponentsMask, XkbGroupStateMask);
	XkbGetState(dpy, XkbUseCoreKbd, &xkbstate);
	xkbGlobal.group = xkbstate.locked_group;
	#endif // XKB_PATCH

	grabkeys();
	focus(NULL);
	#if IPC_PATCH
	setupepoll();
	#endif // IPC_PATCH
	#if BAR_ANYBAR_PATCH
	if (usealtbar)
		spawnbar();
	#endif // BAR_ANYBAR_PATCH
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
		#if !RENAMED_SCRATCHPADS_PATCH
		#if SCRATCHPADS_PATCH && SCRATCHPADS_KEEP_POSITION_AND_SIZE_PATCH
		if (
			(c->tags & SPTAGMASK) &&
			c->isfloating &&
			(
				c->x < c->mon->mx ||
				c->x > c->mon->mx + c->mon->mw ||
				c->y < c->mon->my ||
				c->y > c->mon->my + c->mon->mh
			)
		) {
			c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
			c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
		}
		#elif SCRATCHPADS_PATCH
		if ((c->tags & SPTAGMASK) && c->isfloating) {
			c->x = c->mon->wx + (c->mon->ww / 2 - WIDTH(c) / 2);
			c->y = c->mon->wy + (c->mon->wh / 2 - HEIGHT(c) / 2);
		}
		#endif // SCRATCHPADS_KEEP_POSITION_AND_SIZE_PATCH | SCRATCHPADS_PATCH
		#endif // RENAMED_SCRATCHPADS_PATCH
		/* show clients top down */
		#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
		if (!c->mon->lt[c->mon->sellt]->arrange && c->sfx != -9999 && !c->isfullscreen) {
			XMoveResizeWindow(dpy, c->win, c->sfx, c->sfy, c->sfw, c->sfh);
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
		#if RENAMED_SCRATCHPADS_PATCH && RENAMED_SCRATCHPADS_AUTO_HIDE_PATCH
		/* optional: auto-hide scratchpads when moving to other tags */
		if (c->scratchkey != 0 && !(c->tags & c->mon->tagset[c->mon->seltags]))
			c->tags = 0;
		#endif // RENAMED_SCRATCHPADS_AUTO_HIDE_PATCH
		/* hide clients bottom up */
		showhide(c->snext);
		XMoveWindow(dpy, c->win, WIDTH(c) * -2, c->y);
	}
}

void
sigchld(int unused)
{
	#if COOL_AUTOSTART_PATCH
	pid_t pid;
	#endif // COOL_AUTOSTART_PATCH
	if (signal(SIGCHLD, sigchld) == SIG_ERR)
		die("can't install SIGCHLD handler:");
	#if COOL_AUTOSTART_PATCH
	while (0 < (pid = waitpid(-1, NULL, WNOHANG))) {
		pid_t *p, *lim;

		if (!(p = autostart_pids))
			continue;
		lim = &p[autostart_len];

		for (; p < lim; p++) {
			if (*p == pid) {
				*p = -1;
				break;
			}
		}
	}
	#else
	while (0 < waitpid(-1, NULL, WNOHANG));
	#endif // COOL_AUTOSTART_PATCH
}

#if RIODRAW_PATCH
void
spawn(const Arg *arg)
{
	spawncmd(arg);
}

pid_t
spawncmd(const Arg *arg)
#else
void
spawn(const Arg *arg)
#endif // RIODRAW_PATCH
{
	#if RIODRAW_PATCH
	pid_t pid;
	#endif // RIODRAW_PATCH
	#if !NODMENU_PATCH
	if (arg->v == dmenucmd)
		dmenumon[0] = '0' + selmon->num;
	#endif // NODMENU_PATCH

	#if RIODRAW_PATCH
	if ((pid = fork()) == 0)
	#else
	if (fork() == 0)
	#endif // RIODRAW_PATCH
	{
		if (dpy)
			close(ConnectionNumber(dpy));

		#if BAR_STATUSCMD_PATCH && !BAR_DWMBLOCKS_PATCH
		if (arg->v == statuscmd) {
			for (int i = 0; i < LENGTH(statuscmds); i++) {
				if (statuscmdn == statuscmds[i].id) {
					statuscmd[2] = statuscmds[i].cmd;
					setenv("BUTTON", lastbutton, 1);
					break;
				}
			}
			if (!statuscmd[2])
				exit(EXIT_SUCCESS);
		}
		#endif // BAR_STATUSCMD_PATCH | BAR_DWMBLOCKS_PATCH
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
		die("dwm: execvp '%s' failed:", ((char **)arg->v)[0]);
	}
	#if RIODRAW_PATCH
	return pid;
	#endif // RIODRAW_PATCH
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
		if ((arg->ui & TAGMASK) != selmon->tagset[selmon->seltags])
			view(arg);
		#endif // VIEWONTAG_PATCH
	}
}

void
tagmon(const Arg *arg)
{
	Client *c = selmon->sel;
	Monitor *dest;
	#if SEAMLESS_RESTART_PATCH && SAVEFLOATS_PATCH
	int restored;
	#endif // SEAMLESS_RESTART_PATCH | SAVEFLOATS_PATCH
	if (!c || !mons->next)
		return;
	dest = dirtomon(arg->i);
	#if SEAMLESS_RESTART_PATCH && SAVEFLOATS_PATCH
	savewindowfloatposition(c, c->mon);
	restored = restorewindowfloatposition(c, dest);
	if (restored && (!dest->lt[dest->sellt]->arrange || c->isfloating)) {
		XMoveResizeWindow(dpy, c->win, c->sfx, c->sfy, c->sfw, c->sfh);
		resize(c, c->sfx, c->sfy, c->sfw, c->sfh, 1);
	}
	#endif // SEAMLESS_RESTART_PATCH | SAVEFLOATS_PATCH
	#if TAGMONFIXFS_PATCH
	if (c->isfullscreen) {
		c->isfullscreen = 0;
		sendmon(c, dest);
		c->isfullscreen = 1;
		#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
		if (c->fakefullscreen != 1) {
			resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
			XRaiseWindow(dpy, c->win);
		}
		#elif !FAKEFULLSCREEN_PATCH
		resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
		XRaiseWindow(dpy, c->win);
		#endif // FAKEFULLSCREEN_CLIENT_PATCH
	} else
		sendmon(c, dest);
	#else
	sendmon(c, dest);
	#endif // TAGMONFIXFS_PATCH
}

void
togglebar(const Arg *arg)
{
	Bar *bar;
	#if BAR_HOLDBAR_PATCH && PERTAG_PATCH && PERTAGBAR_PATCH
	selmon->showbar = selmon->pertag->showbars[selmon->pertag->curtag] = (selmon->showbar == 2 ? 1 : !selmon->showbar);
	#elif BAR_HOLDBAR_PATCH
	selmon->showbar = (selmon->showbar == 2 ? 1 : !selmon->showbar);
	#elif PERTAG_PATCH && PERTAGBAR_PATCH
	selmon->showbar = selmon->pertag->showbars[selmon->pertag->curtag] = !selmon->showbar;
	#else
	selmon->showbar = !selmon->showbar;
	#endif // BAR_HOLDBAR_PATCH | PERTAG_PATCH
	updatebarpos(selmon);
	for (bar = selmon->bar; bar; bar = bar->next)
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	#if BAR_SYSTRAY_PATCH
	if (!selmon->showbar && systray)
		XMoveWindow(dpy, systray->win, -32000, -32000);
	#endif // BAR_SYSTRAY_PATCH
	arrange(selmon);
}

void
togglefloating(const Arg *arg)
{
	Client *c = selmon->sel;
	if (arg && arg->v)
		c = (Client*)arg->v;
	if (!c)
		return;
	#if !FAKEFULLSCREEN_PATCH
	#if FAKEFULLSCREEN_CLIENT_PATCH
	if (c->isfullscreen && c->fakefullscreen != 1) /* no support for fullscreen windows */
		return;
	#else
	if (c->isfullscreen) /* no support for fullscreen windows */
		return;
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#endif // !FAKEFULLSCREEN_PATCH
	c->isfloating = !c->isfloating || c->isfixed;
	#if !BAR_FLEXWINTITLE_PATCH
	if (c->isfloating)
		XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, c->win, scheme[SchemeSel][ColBorder].pixel);
	#endif // BAR_FLEXWINTITLE_PATCH
	if (c->isfloating) {
		#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
		if (c->sfx != -9999) {
			/* restore last known float dimensions */
			resize(c, c->sfx, c->sfy, c->sfw, c->sfh, 0);
		} else
		#endif // SAVEFLOATS_PATCH // EXRESIZE_PATCH
		resize(c, c->x, c->y, c->w, c->h, 0);
	#if SAVEFLOATS_PATCH || EXRESIZE_PATCH
	} else {
		/* save last known float dimensions */
		c->sfx = c->x;
		c->sfy = c->y;
		c->sfw = c->w;
		c->sfh = c->h;
	#endif // SAVEFLOATS_PATCH / EXRESIZE_PATCH
	}
	arrange(c->mon);

	#if BAR_EWMHTAGS_PATCH
	setfloatinghint(c);
	#endif // BAR_EWMHTAGS_PATCH
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
	#if BAR_EWMHTAGS_PATCH
	updatecurrentdesktop();
	#endif // BAR_EWMHTAGS_PATCH
}

void
toggleview(const Arg *arg)
{
	unsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);;
	#if TAGSYNC_PATCH
	Monitor *origselmon = selmon;
	for (selmon = mons; selmon; selmon = selmon->next) {
	#endif // TAGSYNC_PATCH
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
		#if BAR_TAGPREVIEW_PATCH
		tagpreviewswitchtag();
		#endif // BAR_TAGPREVIEW_PATCH
		selmon->tagset[selmon->seltags] = newtagset;

		#if PERTAG_PATCH
		#if SCRATCHPADS_PATCH && !RENAMED_SCRATCHPADS_PATCH
		if (newtagset == ~SPTAGMASK)
		#else
		if (newtagset == ~0)
		#endif // SCRATCHPADS_PATCH
		{
			selmon->pertag->prevtag = selmon->pertag->curtag;
			selmon->pertag->curtag = 0;
		}
		/* test if the user did not select the same tag */
		if (!(newtagset & 1 << (selmon->pertag->curtag - 1))) {
			selmon->pertag->prevtag = selmon->pertag->curtag;
			for (i = 0; !(newtagset & 1 << i); i++) ;
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
		#if !TAGSYNC_PATCH
		focus(NULL);
		arrange(selmon);
		#endif // TAGSYNC_PATCH
	#if !EMPTYVIEW_PATCH
	}
	#endif // EMPTYVIEW_PATCH
	#if TAGSYNC_PATCH
	}
	selmon = origselmon;
	#if !EMPTYVIEW_PATCH
	if (newtagset) {
	#endif // EMPTYVIEW_PATCH
		focus(NULL);
		arrange(NULL);
	#if !EMPTYVIEW_PATCH
	}
	#endif // EMPTYVIEW_PATCH
	#endif // TAGSYNC_PATCH
	#if BAR_EWMHTAGS_PATCH
	updatecurrentdesktop();
	#endif // BAR_EWMHTAGS_PATCH
}

void
unfocus(Client *c, int setfocus, Client *nextfocus)
{
	if (!c)
		return;
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	selmon->pertag->prevclient[selmon->pertag->curtag] = c;
	#endif // SWAPFOCUS_PATCH
	#if LOSEFULLSCREEN_PATCH
	if (c->isfullscreen && ISVISIBLE(c) && c->mon == selmon && nextfocus && !nextfocus->isfloating)
		#if FAKEFULLSCREEN_CLIENT_PATCH
		if (c->fakefullscreen != 1)
			setfullscreen(c, 0);
		#else
		setfullscreen(c, 0);
		#endif // #if FAKEFULLSCREEN_CLIENT_PATCH
	#endif // LOSEFULLSCREEN_PATCH
	grabbuttons(c, 0);
	#if !BAR_FLEXWINTITLE_PATCH
	#if RENAMED_SCRATCHPADS_PATCH
	if (c->scratchkey != 0 && c->isfloating)
		XSetWindowBorder(dpy, c->win, scheme[SchemeScratchNorm][ColFloat].pixel);
	else if (c->scratchkey != 0)
		XSetWindowBorder(dpy, c->win, scheme[SchemeScratchNorm][ColBorder].pixel);
	else if (c->isfloating)
		XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColBorder].pixel);
	#else
	if (c->isfloating)
		XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColFloat].pixel);
	else
		XSetWindowBorder(dpy, c->win, scheme[SchemeNorm][ColBorder].pixel);
	#endif // RENAMED_SCRATCHPADS_PATCH
	#endif // BAR_FLEXWINTITLE_PATCH
	if (setfocus) {
		XSetInputFocus(dpy, root, RevertToPointerRoot, CurrentTime);
		XDeleteProperty(dpy, root, netatom[NetActiveWindow]);
	}
}

void
unmanage(Client *c, int destroyed)
{
	Monitor *m = c->mon;
	#if SWITCHTAG_PATCH
	unsigned int switchtag = c->switchtag;
	#endif // SWITCHTAG_PATCH
	XWindowChanges wc;
	#if XKB_PATCH
	XkbInfo *xkb;
	#endif // XKB_PATCH

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
	#if BAR_WINICON_PATCH
	freeicon(c);
	#endif // BAR_WINICON_PATCH
	if (!destroyed) {
		wc.border_width = c->oldbw;
		XGrabServer(dpy); /* avoid race conditions */
		XSetErrorHandler(xerrordummy);
		XSelectInput(dpy, c->win, NoEventMask);
		XConfigureWindow(dpy, c->win, CWBorderWidth, &wc); /* restore border */
		XUngrabButton(dpy, AnyButton, AnyModifier, c->win);
		setclientstate(c, WithdrawnState);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}
	#if XKB_PATCH
	else {
		xkb = findxkb(c->win);
		if (xkb != NULL) {
			if (xkb->prev)
				xkb->prev->next = xkb->next;
			if (xkb->next)
				xkb->next->prev = xkb->prev;
			free(xkb);
		}
	}
	#endif // XKB_PATCH

	#if SCRATCHPAD_ALT_1_PATCH
	if (scratchpad_last_showed == c)
		scratchpad_last_showed = NULL;
	#endif // SCRATCHPAD_ALT_1_PATCH

	free(c);
	#if SWALLOW_PATCH
	if (s)
		return;
	#endif // SWALLOW_PATCH
	focus(NULL);
	updateclientlist();
	arrange(m);
	#if SWITCHTAG_PATCH
	if (switchtag && ((switchtag & TAGMASK) != selmon->tagset[selmon->seltags]))
		view(&((Arg) { .ui = switchtag }));
	#endif // SWITCHTAG_PATCH
}

void
unmapnotify(XEvent *e)
{
	Client *c;
	#if BAR_ANYBAR_PATCH
	Monitor *m;
	Bar *bar;
	#endif // BAR_ANYBAR_PATCH
	XUnmapEvent *ev = &e->xunmap;

	if ((c = wintoclient(ev->window))) {
		if (ev->send_event)
			setclientstate(c, WithdrawnState);
		else
			unmanage(c, 0);
	#if BAR_SYSTRAY_PATCH
	} else if (showsystray && (c = wintosystrayicon(ev->window))) {
		/* KLUDGE! sometimes icons occasionally unmap their windows, but do
		 * _not_ destroy them. We map those windows back */
		XMapRaised(dpy, c->win);
		removesystrayicon(c);
		drawbarwin(systray->bar);
	#endif // BAR_SYSTRAY_PATCH
	}
	#if BAR_ANYBAR_PATCH
	else {
		 m = wintomon(ev->window);
		 for (bar = m->bar; bar; bar = bar->next) {
		 	if (bar->win == ev->window) {
				unmanagealtbar(ev->window);
				break;
			}
		}
	}
	#endif // BAR_ANYBAR_PATCH
}

void
updatebars(void)
{
	Bar *bar;
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
		#if BAR_TAGPREVIEW_PATCH
		.event_mask = ButtonPressMask|ExposureMask|PointerMotionMask
		#else
		.event_mask = ButtonPressMask|ExposureMask
		#endif // BAR_TAGPREVIEW_PATCH
	};
	XClassHint ch = {"dwm", "dwm"};
	for (m = mons; m; m = m->next) {
		for (bar = m->bar; bar; bar = bar->next) {
			if (bar->external)
				continue;
			if (!bar->win) {
				#if BAR_ALPHA_PATCH
				bar->win = XCreateWindow(dpy, root, bar->bx, bar->by, bar->bw, bar->bh, 0, depth,
				                          InputOutput, visual,
				                          CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
				#else
				bar->win = XCreateWindow(dpy, root, bar->bx, bar->by, bar->bw, bar->bh, 0, DefaultDepth(dpy, screen),
						CopyFromParent, DefaultVisual(dpy, screen),
						CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
				#endif // BAR_ALPHA_PATCH
				XDefineCursor(dpy, bar->win, cursor[CurNormal]->cursor);
				XMapRaised(dpy, bar->win);
				XSetClassHint(dpy, bar->win, &ch);
			}
		}
		#if TAB_PATCH
		if (!m->tabwin) {
			#if BAR_ALPHA_PATCH
			m->tabwin = XCreateWindow(dpy, root, m->wx, m->ty, m->ww, th, 0, depth,
							InputOutput, visual,
							CWOverrideRedirect|CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &wa);
			#else
			m->tabwin = XCreateWindow(dpy, root, m->wx, m->ty, m->ww, th, 0, DefaultDepth(dpy, screen),
							CopyFromParent, DefaultVisual(dpy, screen),
							CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
			#endif // BAR_ALPHA_PATCH
			XDefineCursor(dpy, m->tabwin, cursor[CurNormal]->cursor);
			XMapRaised(dpy, m->tabwin);
		}
		#endif // TAB_PATCH
	}
}

void
updatebarpos(Monitor *m)
{
	#if TAB_PATCH
	Client *c;
	int nvis = 0;
	#endif // TAB_PATCH

	m->wx = m->mx;
	m->wy = m->my;
	m->ww = m->mw;
	m->wh = m->mh;
	Bar *bar;
	int y_pad = 0;
	int x_pad = 0;
	#if BAR_PADDING_VANITYGAPS_PATCH && VANITYGAPS_PATCH
	#if PERTAG_VANITYGAPS_PATCH && PERTAG_PATCH
	if (!selmon || selmon->pertag->enablegaps[selmon->pertag->curtag])
	#else
	if (enablegaps)
	#endif // PERTAG_VANITYGAPS_PATCH
	{
		y_pad = gappoh;
		x_pad = gappov;
	}
	#elif BAR_PADDING_PATCH
	y_pad = vertpad;
	x_pad = sidepad;
	#endif // BAR_PADDING_PATCH | BAR_PADDING_VANITYGAPS_PATCH

	#if INSETS_PATCH
	// Custom insets
	Inset inset = m->inset;
	m->wx += inset.x;
	m->wy += inset.y;
	m->ww -= inset.w + inset.x;
	m->wh -= inset.h + inset.y;
	#endif // INSETS_PATCH

	for (bar = m->bar; bar; bar = bar->next) {
		bar->bx = m->wx + x_pad;
		#if BAR_ANYBAR_PATCH && !BAR_ANYBAR_MANAGE_WIDTH_PATCH
		if (bar->external)
			continue;
		#endif // BAR_ANYBAR_PATCH | BAR_ANYBAR_MANAGE_WIDTH_PATCH
		bar->bw = m->ww - 2 * x_pad;
	}

	for (bar = m->bar; bar; bar = bar->next)
		if (!m->showbar || !bar->showbar)
			bar->by = -bar->bh - y_pad;

	#if TAB_PATCH
	for (c = m->clients; c; c = c->next) {
		if (ISVISIBLE(c) && !HIDDEN(c))
			++nvis;
	}

	if (m->showtab == showtab_always
	   || ((m->showtab == showtab_auto) && (nvis > 1) && (m->lt[m->sellt]->arrange == monocle))) {
		m->wh -= th;
		m->ty = m->toptab ? m->wy : m->wy + m->wh;
		if (m->toptab)
			m->wy += th;
	} else {
		m->ty = -th;
	}
	#endif // TAB_PATCH

	if (!m->showbar)
		return;
	for (bar = m->bar; bar; bar = bar->next) {
		if (!bar->showbar)
			continue;
		if (bar->topbar)
			m->wy = m->wy + bar->bh + y_pad;
		m->wh -= y_pad + bar->bh;
		bar->by = (bar->topbar ? m->wy - bar->bh : m->wy + m->wh);
	}
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

	#if NET_CLIENT_LIST_STACKING_PATCH
	XDeleteProperty(dpy, root, netatom[NetClientListStacking]);
	for (m = mons; m; m = m->next)
		for (c = m->stack; c; c = c->snext)
			XChangeProperty(dpy, root, netatom[NetClientListStacking],
				XA_WINDOW, 32, PropModeAppend,
				(unsigned char *) &(c->win), 1);
	#endif // NET_CLIENT_LIST_STACKING_PATCH
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
		/* new monitors if nn > n */
		for (i = n; i < nn; i++) {
			for (m = mons; m && m->next; m = m->next);
			if (m)
				m->next = createmon();
			else
				mons = createmon();
		}
		for (i = 0, m = mons; i < nn && m; m = m->next, i++)
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
		/* removed monitors if n > nn */
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
			cleanupmon(m);
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
		#if SIZEHINTS_PATCH || SIZEHINTS_RULED_PATCH || SIZEHINTS_ISFREESIZE_PATCH
		size.flags = 0;
		#else
		size.flags = PSize;
		#endif // SIZEHINTS_PATCH | SIZEHINTS_RULED_PATCH | SIZEHINTS_ISFREESIZE_PATCH
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
	#if SIZEHINTS_PATCH || SIZEHINTS_RULED_PATCH || SIZEHINTS_ISFREESIZE_PATCH
	#if SIZEHINTS_ISFREESIZE_PATCH
	if ((size.flags & PSize) && c->isfreesize)
	#else
	if (size.flags & PSize)
	#endif // SIZEHINTS_ISFREESIZE_PATCH
	{
		c->basew = size.base_width;
		c->baseh = size.base_height;
		c->isfloating = 1;
	}
	#if SIZEHINTS_RULED_PATCH
	checkfloatingrules(c);
	#endif // SIZEHINTS_RULED_PATCH
	#endif // SIZEHINTS_PATCH
	c->isfixed = (c->maxw && c->maxh && c->maxw == c->minw && c->maxh == c->minh);
	c->hintsvalid = 1;
}

void
updatestatus(void)
{
	Monitor *m;
	#if BAR_EXTRASTATUS_PATCH
	if (!gettextprop(root, XA_WM_NAME, rawstext, sizeof(rawstext))) {
		strcpy(stext, "dwm-"VERSION);
		estext[0] = '\0';
	} else {
		char *e = strchr(rawstext, statussep);
		if (e) {
			*e = '\0'; e++;
			#if BAR_STATUSCMD_PATCH
			strncpy(rawestext, e, sizeof(estext) - 1);
			copyvalidchars(estext, rawestext);
			#else
			strncpy(estext, e, sizeof(estext) - 1);
			#endif // BAR_STATUSCMD_PATCH
		} else {
			estext[0] = '\0';
		}
		#if BAR_STATUSCMD_PATCH
		copyvalidchars(stext, rawstext);
		#else
		strncpy(stext, rawstext, sizeof(stext) - 1);
		#endif // BAR_STATUSCMD_PATCH
	}
	#elif BAR_STATUSCMD_PATCH
	if (!gettextprop(root, XA_WM_NAME, rawstext, sizeof(rawstext)))
		strcpy(stext, "dwm-"VERSION);
	else
		copyvalidchars(stext, rawstext);
	#else
	if (!gettextprop(root, XA_WM_NAME, stext, sizeof(stext)))
		strcpy(stext, "dwm-"VERSION);
	#endif // BAR_EXTRASTATUS_PATCH | BAR_STATUSCMD_PATCH
	for (m = mons; m; m = m->next)
		drawbar(m);
}

void
updatetitle(Client *c)
{
	#if IPC_PATCH
	char oldname[sizeof(c->name)];
	strcpy(oldname, c->name);
	#endif // IPC_PATCH

	if (!gettextprop(c->win, netatom[NetWMName], c->name, sizeof c->name))
		gettextprop(c->win, XA_WM_NAME, c->name, sizeof c->name);
	if (c->name[0] == '\0') /* hack to mark broken clients */
		strcpy(c->name, broken);

	#if IPC_PATCH
	for (Monitor *m = mons; m; m = m->next) {
		if (m->sel == c && strcmp(oldname, c->name) != 0)
			ipc_focused_title_change_event(m->num, c->win, oldname, c->name);
	}
	#endif // IPC_PATCH
}

void
updatewmhints(Client *c)
{
	XWMHints *wmh;

	if ((wmh = XGetWMHints(dpy, c->win))) {
		if (c == selmon->sel && wmh->flags & XUrgencyHint) {
			wmh->flags &= ~XUrgencyHint;
			XSetWMHints(dpy, c->win, wmh);
		} else
			c->isurgent = (wmh->flags & XUrgencyHint) ? 1 : 0;
		if (c->isurgent) {
			if (c->isfloating)
				XSetWindowBorder(dpy, c->win, scheme[SchemeUrg][ColFloat].pixel);
			else
				XSetWindowBorder(dpy, c->win, scheme[SchemeUrg][ColBorder].pixel);
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
	#if TAGSYNC_PATCH
	Monitor *origselmon = selmon;
	for (selmon = mons; selmon; selmon = selmon->next) {
	#endif // TAGSYNC_PATCH
	#if EMPTYVIEW_PATCH
	if (arg->ui && (arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
	#else
	if ((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
	#endif // EMPTYVIEW_PATCH
	{
		#if TOGGLETAG_PATCH
		view(&((Arg) { .ui = 0 }));
		#endif // TOGGLETAG_PATCH
		return;
	}
	#if BAR_TAGPREVIEW_PATCH
	tagpreviewswitchtag();
	#endif // BAR_TAGPREVIEW_PATCH
	selmon->seltags ^= 1; /* toggle sel tagset */
	#if PERTAG_PATCH
	pertagview(arg);
	#else
	if (arg->ui & TAGMASK)
		selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
	#endif // PERTAG_PATCH
	#if TAGSYNC_PATCH
	}
	selmon = origselmon;
	#endif // TAGSYNC_PATCH
	focus(NULL);
	#if TAGSYNC_PATCH
	arrange(NULL);
	#else
	arrange(selmon);
	#endif // TAGSYNC_PATCH
	#if BAR_EWMHTAGS_PATCH
	updatecurrentdesktop();
	#endif // BAR_EWMHTAGS_PATCH
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
	Bar *bar;

	if (w == root && getrootptr(&x, &y))
		return recttomon(x, y, 1, 1);
	for (m = mons; m; m = m->next)
		for (bar = m->bar; bar; bar = bar->next)
			if (w == bar->win)
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
	if (arg && arg->v)
		c = (Client*)arg->v;
	if (!c)
		return;
	#if ZOOMSWAP_PATCH
	Client *at = NULL, *cold, *cprevious = NULL, *p;
	#endif // ZOOMSWAP_PATCH

	#if ZOOMFLOATING_PATCH
	if (c && c->isfloating)
		togglefloating(&((Arg) { .v = c }));
	#endif // ZOOMFLOATING_PATCH

	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	c->mon->pertag->prevclient[c->mon->pertag->curtag] = nexttiled(c->mon->clients);
	#endif // SWAPFOCUS_PATCH

	if (!c->mon->lt[c->mon->sellt]->arrange || !c || c->isfloating)
		return;

	#if ZOOMSWAP_PATCH
	if (c == nexttiled(c->mon->clients)) {
		#if PERTAG_PATCH
		p = c->mon->pertag->prevzooms[c->mon->pertag->curtag];
		#else
		p = prevzoom;
		#endif // PERTAG_PATCH
		at = findbefore(p);
		if (at)
			cprevious = nexttiled(at->next);
		if (!cprevious || cprevious != p) {
			#if PERTAG_PATCH
			c->mon->pertag->prevzooms[c->mon->pertag->curtag] = NULL;
			#else
			prevzoom = NULL;
			#endif // PERTAG_PATCH
			#if SWAPFOCUS_PATCH && PERTAG_PATCH
			if (!c || !(c = c->mon->pertag->prevclient[c->mon->pertag->curtag] = nexttiled(c->next)))
			#else
			if (!c || !(c = nexttiled(c->next)))
			#endif // SWAPFOCUS_PATCH
				return;
		} else
			#if SWAPFOCUS_PATCH && PERTAG_PATCH
			c = c->mon->pertag->prevclient[c->mon->pertag->curtag] = cprevious;
			#else
			c = cprevious;
			#endif // SWAPFOCUS_PATCH
	}

	cold = nexttiled(c->mon->clients);
	if (c != cold && !at)
		at = findbefore(c);
	detach(c);
	attach(c);
	/* swap windows instead of pushing the previous one down */
	if (c != cold && at) {
		#if PERTAG_PATCH
		c->mon->pertag->prevzooms[c->mon->pertag->curtag] = cold;
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
	#elif SWAPFOCUS_PATCH && PERTAG_PATCH
	if (c == nexttiled(c->mon->clients) && !(c = c->mon->pertag->prevclient[c->mon->pertag->curtag] = nexttiled(c->next)))
		return;
	pop(c);
	#else
	if (c == nexttiled(selmon->clients) && !(c = nexttiled(c->next)))
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
		else if (!strcmp("-fn", argv[i])) /* font set */
		#if BAR_PANGO_PATCH
			strcpy(font, argv[++i]);
		#else
			fonts[0] = argv[++i];
		#endif // BAR_PANGO_PATCH
		#if !BAR_VTCOLORS_PATCH
		else if (!strcmp("-nb", argv[i])) /* normal background color */
			colors[SchemeNorm][1] = argv[++i];
		else if (!strcmp("-nf", argv[i])) /* normal foreground color */
			colors[SchemeNorm][0] = argv[++i];
		else if (!strcmp("-sb", argv[i])) /* selected background color */
			colors[SchemeSel][1] = argv[++i];
		else if (!strcmp("-sf", argv[i])) /* selected foreground color */
			colors[SchemeSel][0] = argv[++i];
		#endif // !BAR_VTCOLORS_PATCH
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
	#if XRDB_PATCH && !BAR_VTCOLORS_PATCH
	XrmInitialize();
	loadxrdb();
	#endif // XRDB_PATCH && !BAR_VTCOLORS_PATCH
	#if COOL_AUTOSTART_PATCH
	autostart_exec();
	#endif // COOL_AUTOSTART_PATCH
	setup();
#ifdef __OpenBSD__
	#if SWALLOW_PATCH
	if (pledge("stdio rpath proc exec ps", NULL) == -1)
	#else
	if (pledge("stdio rpath proc exec", NULL) == -1)
	#endif // SWALLOW_PATCH
		die("pledge");
#endif /* __OpenBSD__ */
	scan();
	#if AUTOSTART_PATCH
	runautostart();
	#endif
	run();
	cleanup();
	XCloseDisplay(dpy);
	#if RESTARTSIG_PATCH
	if (restart)
		execvp(argv[0], argv);
	#endif // RESTARTSIG_PATCH
	return EXIT_SUCCESS;
}

