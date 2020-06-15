/* See LICENSE file for copyright and license details. */

/* appearance */
#if ROUNDED_CORNERS_PATCH
static const unsigned int borderpx       = 0;   /* border pixel of windows */
static const int corner_radius           = 10;
#else
static const unsigned int borderpx       = 1;   /* border pixel of windows */
#endif // ROUNDED_CORNERS_PATCH
static const unsigned int snap           = 32;  /* snap pixel */
#if VANITYGAPS_PATCH
static const unsigned int gappih         = 20;  /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;  /* vert inner gap between windows */
static const unsigned int gappoh         = 10;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 30;  /* vert outer gap between windows and screen edge */
static const int smartgaps               = 0;   /* 1 means no outer gap when there is only one window */
#endif // VANITYGAPS_PATCH
#if HOLDBAR_PATCH
static const int showbar                 = 0;   /* 0 means no bar */
#else
static const int showbar                 = 1;   /* 0 means no bar */
#endif // HOLDBAR_PATCH
static const int topbar                  = 1;   /* 0 means bottom bar */
#if BARPADDING_PATCH
static const int vertpad                 = 10;  /* vertical padding of bar */
static const int sidepad                 = 10;  /* horizontal padding of bar */
#endif // BARPADDING_PATCH
#if FOCUSONCLICK_PATCH
static const int focusonwheel            = 0;
#endif // FOCUSONCLICK_PATCH
#if STATUSPADDING_PATCH
static const int horizpadbar             = 2;   /* horizontal padding for statusbar */
static const int vertpadbar              = 0;   /* vertical padding for statusbar */
#endif // STATUSPADDING_PATCH
#if STATICSTATUS_PATCH && !STATUSALLMONS_PATCH
static const int statmonval              = 0;
#endif // STATICSTATUS_PATCH
#if SYSTRAY_PATCH
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
#endif // SYSTRAY_PATCH
#if ONLYQUITONEMPTY_PATCH
static const int quit_empty_window_count = 2;   /* only allow dwm to quit if no windows are open, value here represents number of deamons */
#endif // ONLYQUITONEMPTY_PATCH
#if EXTRABAR_PATCH
static const char statussep              = ';';      /* separator between status bars */
#endif // EXTRABAR_PATCH
#if PANGO_PATCH
static const char font[]                 = "monospace 10";
#else
static const char *fonts[]               = { "monospace:size=10" };
#endif // PANGO_PATCH
static const char dmenufont[]            = "monospace:size=10";

static char normfgcolor[]                = "#bbbbbb";
static char normbgcolor[]                = "#222222";
#if !VTCOLORS_PATCH
static char normbordercolor[]            = "#444444";
#if FLOAT_BORDER_COLOR_PATCH
static char normfloatcolor[]             = "#db8fd9";
#endif // FLOAT_BORDER_COLOR_PATCH
#endif // !VTCOLORS_PATCH

static char selfgcolor[]                 = "#eeeeee";
static char selbgcolor[]                 = "#005577";
#if !VTCOLORS_PATCH
static char selbordercolor[]             = "#005577";
#if FLOAT_BORDER_COLOR_PATCH
static char selfloatcolor[]              = "#005577";
#endif // FLOAT_BORDER_COLOR_PATCH

#if STATUSCOLORS_PATCH
static char warnfgcolor[]                 = "#000000";
static char warnbgcolor[]                 = "#ffff00";
static char warnbordercolor[]             = "#ff0000";
#if FLOAT_BORDER_COLOR_PATCH
static char warnfloatcolor[]              = "#ffffff";
#endif // FLOAT_BORDER_COLOR_PATCH
#endif // STATUSCOLORS_PATCH

#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
static char urgfgcolor[]                 = "#000000";
static char urgbgcolor[]                 = "#000000";
static char urgbordercolor[]             = "#ff0000"; // NB: patch only works with border color for now
#if FLOAT_BORDER_COLOR_PATCH
static char urgfloatcolor[]              = "#000000";
#endif // FLOAT_BORDER_COLOR_PATCH
#endif // URGENTBORDER_PATCH // STATUSCOLORS_PATCH

#if AWESOMEBAR_PATCH
static char hidfgcolor[]                 = "#005577";
static char hidbgcolor[]                 = "#222222";
static char hidbordercolor[]             = "#005577";
#if FLOAT_BORDER_COLOR_PATCH
static char hidfloatcolor[]              = "#f76e0c";
#endif // FLOAT_BORDER_COLOR_PATCH
#endif // AWESOMEBAR_PATCH

#if TITLECOLOR_PATCH
static char titlefgcolor[]               = "#eeeeee";
static char titlebgcolor[]               = "#005577";
static char titlebordercolor[]           = "#005577";
#if FLOAT_BORDER_COLOR_PATCH
static char titlefloatcolor[]            = "#005577";
#endif // FLOAT_BORDER_COLOR_PATCH
#endif // TITLECOLOR_PATCH
#endif // VTCOLORS_PATCH

#if ALPHA_PATCH
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3] = {
	/*                    fg      bg        border     */
	[SchemeNorm]      = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]       = { OPAQUE, baralpha, borderalpha },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]      = { OPAQUE, baralpha, borderalpha },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]       = { OPAQUE, baralpha, borderalpha },
	#endif // URGENTBORDER_PATCH / STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]       = { OPAQUE, baralpha, borderalpha },
	#endif // AWESOMEBAR_PATCH
	#if VTCOLORS_PATCH
	[SchemeTagsNorm]  = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsSel]   = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus]    = { OPAQUE, baralpha, borderalpha },
	#elif TITLECOLOR_PATCH
	[SchemeTitle]     = { OPAQUE, baralpha, borderalpha },
	#endif // VTCOLORS_PATCH / TITLECOLOR_PATCH
};
#endif // ALPHA_PATCH
#if VTCOLORS_PATCH && FLOAT_BORDER_COLOR_PATCH
static const char title_bg_dark[]   = "#303030";
static const char title_bg_light[]  = "#fdfdfd";
static const int color_ptrs[][ColCount] = {
	/*                              fg         bg         border    float */
	[SchemeNorm]                = { -1,        -1,        5,        12 },
	[SchemeSel]                 = { -1,        -1,        11,       13 },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]                = { -1,        1,         1,        14 },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]                 = { 7,         9,         9,        15 },
	#endif // URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]                 = { 5,         0,         0,        -1 },
	#endif // AWESOMEBAR_PATCH
	[SchemeTagsNorm]            = { 2,         0,         0,        -1 },
	[SchemeTagsSel]             = { 6,         5,         5,        -1 },
	[SchemeTitleNorm]           = { 6,         -1,        -1,       -1 },
	[SchemeTitleSel]            = { 6,         -1,        -1,       -1 },
	[SchemeStatus]              = { 2,         0,         0,        -1 },
};
static char colors[][ColCount][8] = {
	/*                              fg         bg         border     float     */
	[SchemeNorm]                = { "#000000", "#000000", "#000000", "#000000" },
	[SchemeSel]                 = { "#000000", "#000000", "#000000", "#000000" },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]                = { "#000000", "#000000", "#000000", "#000000" },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]                 = { "#000000", "#000000", "#000000", "#000000" },
	#endif // URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]                 = { "#000000", "#000000", "#000000", "#000000" },
	#endif // AWESOMEBAR_PATCH
	[SchemeTagsNorm]            = { "#000000", "#000000", "#000000", "#000000" },
	[SchemeTagsSel]             = { "#000000", "#000000", "#000000", "#000000" },
	[SchemeTitleNorm]           = { "#000000", "#000000", "#000000", "#000000" },
	[SchemeTitleSel]            = { "#000000", "#000000", "#000000", "#000000" },
	[SchemeStatus]              = { "#000000", "#000000", "#000000", "#000000" },
};
#elif VTCOLORS_PATCH
static const char title_bg_dark[]   = "#303030";
static const char title_bg_light[]  = "#fdfdfd";
static const int color_ptrs[][ColCount] = {
	/*                              fg         bg         border    */
	[SchemeNorm]                = { -1,        -1,        5 },
	[SchemeSel]                 = { -1,        -1,        11 },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]                = { -1,        1,         1 },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]                 = { 7,         9,         9 },
	#endif // URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]                 = { 5,         0,         0 },
	#endif // AWESOMEBAR_PATCH
	[SchemeTagsNorm]            = { 2,         0,         0 },
	[SchemeTagsSel]             = { 6,         5,         5 },
	[SchemeTitleNorm]           = { 6,         -1,        -1 },
	[SchemeTitleSel]            = { 6,         -1,        -1 },
	[SchemeStatus]              = { 2,         0,         0 },
};
static char colors[][ColCount][8] = {
	/*                              fg         bg         border    */
	[SchemeNorm]                = { "#000000", "#000000", "#000000" },
	[SchemeSel]                 = { "#000000", "#000000", "#000000" },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]                = { "#000000", "#000000", "#000000" },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]                 = { "#000000", "#000000", "#000000" },
	#endif // URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]                 = { "#000000", "#000000", "#000000" },
	#endif // AWESOMEBAR_PATCH
	[SchemeTagsNorm]            = { "#000000", "#000000", "#000000" },
	[SchemeTagsSel]             = { "#000000", "#000000", "#000000" },
	[SchemeTitleNorm]           = { "#000000", "#000000", "#000000" },
	[SchemeTitleSel]            = { "#000000", "#000000", "#000000" },
	[SchemeStatus]              = { "#000000", "#000000", "#000000" },
};
#elif FLOAT_BORDER_COLOR_PATCH
static
#if !XRDB_PATCH
const
#endif // XRDB_PATCH
char *colors[][ColCount] = {
	/*                fg            bg            border            float          */
	[SchemeNorm]  = { normfgcolor,  normbgcolor,  normbordercolor,  normfloatcolor },
	[SchemeSel]   = { selfgcolor,   selbgcolor,   selbordercolor,   selfloatcolor  },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]  = { warnfgcolor,  warnbgcolor,  warnbordercolor,  warnfloatcolor },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]   = { urgfgcolor,   urgbgcolor,   urgbordercolor,   urgfloatcolor },
	#endif // URGENTBORDER_PATCH / STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]   = { hidfgcolor,   hidbgcolor,   hidbordercolor,   hidfloatcolor },
	#endif // AWESOMEBAR_PATCH
	#if TITLECOLOR_PATCH
	[SchemeTitle] = { titlefgcolor, titlebgcolor, titlebordercolor, titlefloatcolor },
	#endif // TITLECOLOR_PATCH
};
#else // !VTCOLORS_PATCH && !FLOAT_BORDER_COLOR_PATCH
static
#if !XRDB_PATCH
const
#endif // XRDB_PATCH
char *colors[][ColCount] = {
	/*                fg            bg            border          */
	[SchemeNorm]  = { normfgcolor,  normbgcolor,  normbordercolor },
	[SchemeSel]   = { selfgcolor,   selbgcolor,   selbordercolor  },
	#if STATUSCOLORS_PATCH
	[SchemeWarn]  = { warnfgcolor,  warnbgcolor,  warnbordercolor },
	#endif // STATUSCOLORS_PATCH
	#if URGENTBORDER_PATCH || STATUSCOLORS_PATCH
	[SchemeUrg]   = { urgfgcolor,   urgbgcolor,   urgbordercolor },
	#endif // URGENTBORDER_PATCH / STATUSCOLORS_PATCH
	#if AWESOMEBAR_PATCH
	[SchemeHid]   = { hidfgcolor,   hidbgcolor,   hidbordercolor  },
	#endif // AWESOMEBAR_PATCH
	#if TITLECOLOR_PATCH
	[SchemeTitle] = { titlefgcolor, titlebgcolor, titlebordercolor },
	#endif // TITLECOLOR_PATCH
};
#endif // VTCOLORS_PATCH / FLOAT_BORDER_COLOR_PATCH

#if SCRATCHPADS_PATCH
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
   /* name          cmd  */
   {"spterm",      spcmd1},
   {"spranger",    spcmd2},
   {"keepassxc",   spcmd3},
};
#endif // SCRATCHPADS_PATCH

/* tagging */
#if EWMHTAGS_PATCH
static char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#else
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif // EWMHTAGS_PATCH
#if ALTERNATIVE_TAGS_PATCH
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif // ALTERNATIVE_TAGS_PATCH

#if TAGGRID_PATCH
/* grid of tags */
#define DRAWCLASSICTAGS             1 << 0
#define DRAWTAGGRID                 1 << 1

#define SWITCHTAG_UP                1 << 0
#define SWITCHTAG_DOWN              1 << 1
#define SWITCHTAG_LEFT              1 << 2
#define SWITCHTAG_RIGHT             1 << 3
#define SWITCHTAG_TOGGLETAG         1 << 4
#define SWITCHTAG_TAG               1 << 5
#define SWITCHTAG_VIEW              1 << 6
#define SWITCHTAG_TOGGLEVIEW        1 << 7

static const unsigned int drawtagmask = DRAWTAGGRID; /* | DRAWCLASSICTAGS to show classic row of tags */
static const int tagrows = 2;
#endif // TAGGRID_PATCH

/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	WM_WINDOW_ROLE(STRING) = role
	 *	_NET_WM_WINDOW_TYPE(ATOM) = wintype
	 */
	RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
	RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
	RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
	RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
	RULE(.class = "Gimp", .tags = 1 << 4)
	RULE(.class = "Firefox", .tags = 1 << 7)
	#if SCRATCHPADS_PATCH
	RULE(.instance = "spterm", .tags = SPTAG(0), .isfloating = 1)
	RULE(.instance = "spfm", .tags = SPTAG(1), .isfloating = 1)
	RULE(.instance = "keepassxc", .tags = SPTAG(2))
	#endif // SCRATCHPADS_PATCH
};

#if MONITOR_RULES_PATCH
#if PERTAG_PATCH
static const MonitorRule monrules[] = {
	/* monitor  tag   layout  mfact  nmaster  showbar  topbar */
	{  1,       -1,   2,      -1,    -1,      -1,      -1     }, // use a different layout for the second monitor
	{  -1,      -1,   0,      -1,    -1,      -1,      -1     }, // default
};
#else
static const MonitorRule monrules[] = {
	/* monitor  layout  mfact  nmaster  showbar  topbar */
	{  1,       2,      -1,    -1,      -1,      -1     }, // use a different layout for the second monitor
	{  -1,      0,      -1,    -1,      -1,      -1     }, // default
};
#endif // PERTAG_PATCH
#endif // MONITOR_RULES_PATCH

#if DWMC_PATCH
/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum                    function */
	{ "focusstack",              focusstack },
	{ "setmfact",                setmfact },
	{ "togglebar",               togglebar },
	{ "incnmaster",              incnmaster },
	{ "togglefloating",          togglefloating },
	{ "focusmon",                focusmon },
	#if STACKER_PATCH
	{ "pushstack",               pushstack },
	#endif // STACKER_PATCH
	#if FOCUSURGENT_PATCH
	{ "focusurgent",             focusurgent },
	#endif // FOCUSURGENT_PATCH
	#if FOCUSADJACENTTAG_PATCH
	{ "viewtoleft",              viewtoleft },
	{ "viewtoright",             viewtoright },
	{ "tagtoleft",               tagtoleft },
	{ "tagtoright",              tagtoright},
	{ "tagandviewtoleft",        tagandviewtoleft },
	{ "tagandviewtoright",       tagandviewtoright },
	#endif // FOCUSADJACENTTAG_PATCH
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	{ "swapfocus",               swapfocus },
	#endif // SWAPFOCUS_PATCH
	#if SWITCHCOL_PATCH
	{ "switchcol",               switchcol },
	#endif // SWITCHCOL_PATCH
	#if ROTATESTACK_PATCH
	{ "rotatestack",             rotatestack },
	#endif // ROTATESTACK_PATCH
	#if INPLACEROTATE_PATCH
	{ "inplacerotate",           inplacerotate },
	#endif // INPLACEROTATE_PATCH
	#if PUSH_PATCH || PUSH_NO_MASTER_PATCH
	{ "pushdown",                pushdown },
	{ "pushup",                  pushup },
	#endif // PUSH_PATCH / PUSH_NO_MASTER_PATCH
	#if FLEXTILE_DELUXE_LAYOUT
	{ "incnstack",               incnstack },
	{ "rotatelayoutaxis",        rotatelayoutaxis },
	{ "setlayoutaxisex",         setlayoutaxisex },
	{ "mirrorlayout",            mirrorlayout },
	#endif // FLEXTILE_DELUXE_LAYOUT
	#if CFACTS_PATCH
	{ "setcfact",                setcfact },
	#endif // CFACTS_PATCH
	#if MOVEPLACE_PATCH
	{ "moveplace",               moveplace },
	#endif // MOVEPLACE_PATCH
	#if EXRESIZE_PATCH
	{ "explace",                 explace },
	{ "togglehorizontalexpand",  togglehorizontalexpand },
	{ "toggleverticalexpand",    toggleverticalexpand },
	{ "togglemaximize",          togglemaximize },
	#endif // EXRESIZE_PATCH
	#if KEYMODES_PATCH
	{ "setkeymode",              setkeymode },
	#endif // KEYMODES_PATCH
	#if TRANSFER_PATCH
	{ "transfer",                transfer },
	#endif // TRANSFER_PATCH
	#if TRANSFER_ALL_PATCH
	{ "transferall",             transferall },
	#endif // TRANSFER_ALL_PATCH
	{ "tagmon",                  tagmon },
	{ "zoom",                    zoom },
	#if VANITYGAPS_PATCH
	{ "incrgaps",                incrgaps },
	{ "incrigaps",               incrigaps },
	{ "incrogaps",               incrogaps },
	{ "incrihgaps",              incrihgaps },
	{ "incrivgaps",              incrivgaps },
	{ "incrohgaps",              incrohgaps },
	{ "incrovgaps",              incrovgaps },
	{ "togglegaps",              togglegaps },
	{ "defaultgaps",             defaultgaps },
	{ "setgaps",                 setgapsex },
	#endif // VANITYGAPS_PATCH
	{ "view",                    view },
	{ "viewall",                 viewallex },
	{ "viewex",                  viewex },
	{ "toggleview",              view },
	#if SHIFTVIEW_PATCH
	{ "shiftview",               shiftview },
	#endif // SHIFTVIEW_PATCH
	#if SHIFTVIEW_CLIENTS_PATCH
	{ "shiftviewclients",        shiftviewclients },
	#endif // SHIFTVIEW_CLIENTS_PATCH
	#if SELFRESTART_PATCH
	{ "self_restart",            self_restart },
	#endif // SELFRESTART_PATCH
	#if TAGGRID_PATCH
	{ "switchtag",               switchtag },
	#endif // TAGGRID_PATCH
	#if STICKY_PATCH
	{ "togglesticky",            togglesticky },
	#endif // STICKY_PATCH
	{ "setborderpx",             setborderpx },
	#if SETBORDERPX_PATCH
	#endif // SETBORDERPX_PATCH
	#if CYCLELAYOUTS_PATCH
	{ "cyclelayout",             cyclelayout },
	#endif // CYCLELAYOUTS_PATCH
	#if MDPCONTROL_PATCH
	{ "mpdchange",               mpdchange },
	{ "mpdcontrol",              mpdcontrol },
	#endif // MDPCONTROL_PATCH
	{ "toggleviewex",            toggleviewex },
	{ "tag",                     tag },
	{ "tagall",                  tagallex },
	{ "tagex",                   tagex },
	{ "toggletag",               tag },
	{ "toggletagex",             toggletagex },
	#if TAGALLMON_PATCH
	{ "tagallmon",               tagallmon },
	#endif // TAGALLMON_PATCH
	#if TAGSWAPMON_PATCH
	{ "tagswapmon",              tagswapmon},
	#endif // TAGSWAPMON_PATCH
	#if ALTERNATIVE_TAGS_PATCH
	{ "togglealttag",            togglealttag },
	#endif // ALTERNATIVE_TAGS_PATCH
	#if TOGGLEFULLSCREEN_PATCH
	{ "togglefullscreen",        togglefullscreen },
	#endif // TOGGLEFULLSCREEN_PATCH
	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	{ "togglefakefullscreen",    togglefakefullscreen },
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#if FULLSCREEN_PATCH
	{ "fullscreen",              fullscreen },
	#endif // FULLSCREEN_PATCH
	#if MAXIMIZE_PATCH
	{ "togglehorizontalmax",     togglehorizontalmax },
	{ "toggleverticalmax",       toggleverticalmax },
	{ "togglemax",               togglemax },
	#endif // MAXIMIZE_PATCH
	#if SCRATCHPADS_PATCH
	{ "togglescratch",           togglescratch },
	#endif // SCRATCHPADS_PATCH
	#if UNFLOATVISIBLE_PATCH
	{ "unfloatvisible",          unfloatvisible },
	#endif // UNFLOATVISIBLE_PATCH
	{ "killclient",              killclient },
	#if WINVIEW_PATCH
	{ "winview",                 winview },
	#endif // WINVIEW_PATCH
	#if XRDB_PATCH && !VTCOLORS_PATCH
	{ "xrdb",                    xrdb },
	#endif // XRDB_PATCH
	{ "quit",                    quit },
	{ "setlayout",               setlayout },
	{ "setlayoutex",             setlayoutex },
};
#elif FSIGNAL_PATCH
/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
	/* signum       function        argument  */
	{ 1,            setlayout,      {.v = 0} },
};
#endif // DWMC_PATCH

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
#if FLEXTILE_DELUXE_LAYOUT
static const int nstack      = 0;    /* number of clients in primary stack area */
#endif // FLEXTILE_DELUXE_LAYOUT
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#if NROWGRID_LAYOUT
#define FORCE_VSPLIT 1
#endif

/* Position of the monocle layout in the layouts variable, used by warp and fullscreen patches */
#define MONOCLE_LAYOUT_POS 2

#if FLEXTILE_DELUXE_LAYOUT
static const Layout layouts[] = {
	/* symbol     arrange function, { nmaster, nstack, layout, master axis, stack axis, secondary stack axis } */
	{ "[]=",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0, NULL } }, // default tile layout
 	{ "><>",      NULL,             {0} },    /* no layout function means floating behavior */
	{ "[M]",      flextile,         { -1, -1, NO_SPLIT, MONOCLE, 0, 0, NULL } }, // monocle
	{ "|||",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // columns (col) layout
	{ ">M>",      flextile,         { -1, -1, FLOATING_MASTER, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // floating master
	{ "[D]",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, MONOCLE, 0, NULL } }, // deck
	{ "TTT",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // bstack
	{ "===",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // bstackhoriz
	{ "|M|",      flextile,         { -1, -1, SPLIT_CENTERED_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, TOP_TO_BOTTOM, NULL } }, // centeredmaster
	{ ":::",      flextile,         { -1, -1, NO_SPLIT, GAPPLESSGRID, 0, 0, NULL } }, // gappless grid
	{ "[\\]",     flextile,         { -1, -1, NO_SPLIT, DWINDLE, 0, 0, NULL } }, // fibonacci dwindle
	{ "(@)",      flextile,         { -1, -1, NO_SPLIT, SPIRAL, 0, 0, NULL } }, // fibonacci spiral
	#if TILE_LAYOUT
	{ "[]=",      tile,             {0} },
	#endif
	#if MONOCLE_LAYOUT
	{ "[M]",      monocle,          {0} },
	#endif
	#if BSTACK_LAYOUT
	{ "TTT",      bstack,           {0} },
	#endif
	#if BSTACKHORIZ_LAYOUT
	{ "===",      bstackhoriz,      {0} },
	#endif
	#if CENTEREDMASTER_LAYOUT
	{ "|M|",      centeredmaster,   {0} },
	#endif
	#if CENTEREDFLOATINGMASTER_LAYOUT
	{ ">M>",      centeredfloatingmaster, {0} },
	#endif
	#if COLUMNS_LAYOUT
	{ "|||",      col,              {0} },
	#endif
	#if DECK_LAYOUT
	{ "[D]",      deck,             {0} },
	#endif
	#if FIBONACCI_SPIRAL_LAYOUT
	{ "(@)",      spiral,           {0} },
	#endif
	#if FIBONACCI_DWINDLE_LAYOUT
	{ "[\\]",     dwindle,          {0} },
	#endif
	#if GRIDMODE_LAYOUT
	{ "HHH",      grid,             {0} },
	#endif
	#if HORIZGRID_LAYOUT
	{ "---",      horizgrid,        {0} },
	#endif
	#if GAPPLESSGRID_LAYOUT
	{ ":::",      gaplessgrid,      {0} },
	#endif
	#if NROWGRID_LAYOUT
	{ "###",      nrowgrid,         {0} },
	#endif
	#if CYCLELAYOUTS_PATCH
	{ NULL,       NULL,             {0} },
	#endif
};
#else
static const Layout layouts[] = {
	/* symbol     arrange function */
	#if TILE_LAYOUT
	{ "[]=",      tile },    /* first entry is default */
	#endif
	{ "><>",      NULL },    /* no layout function means floating behavior */
	#if MONOCLE_LAYOUT
	{ "[M]",      monocle },
	#endif
	#if BSTACK_LAYOUT
	{ "TTT",      bstack },
	#endif
	#if BSTACKHORIZ_LAYOUT
	{ "===",      bstackhoriz },
	#endif
	#if CENTEREDMASTER_LAYOUT
	{ "|M|",      centeredmaster },
	#endif
	#if CENTEREDFLOATINGMASTER_LAYOUT
	{ ">M>",      centeredfloatingmaster },
	#endif
	#if COLUMNS_LAYOUT
	{ "|||",      col },
	#endif
	#if DECK_LAYOUT
	{ "[D]",      deck },
	#endif
	#if FIBONACCI_SPIRAL_LAYOUT
	{ "(@)",      spiral },
	#endif
	#if FIBONACCI_DWINDLE_LAYOUT
	{ "[\\]",     dwindle },
	#endif
	#if GRIDMODE_LAYOUT
	{ "HHH",      grid },
	#endif
	#if HORIZGRID_LAYOUT
	{ "---",      horizgrid },
	#endif
	#if GAPPLESSGRID_LAYOUT
	{ ":::",      gaplessgrid },
	#endif
	#if NROWGRID_LAYOUT
	{ "###",      nrowgrid },
	#endif
	#if CYCLELAYOUTS_PATCH
	{ NULL,       NULL },
	#endif
};
#endif // FLEXTILE_DELUXE_LAYOUT

/* key definitions */
#define MODKEY Mod1Mask
#if COMBO_PATCH && SWAPTAGS_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} },
#elif COMBO_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#elif SWAPTAGS_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} },
#else
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#endif // COMBO_PATCH / SWAPTAGS_PATCH

#if STACKER_PATCH
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_s,     ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_w,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_e,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = -1 } },
#endif // STACKER_PATCH

#if HOLDBAR_PATCH
#define HOLDKEY 0 // replace 0 with the keysym to activate holdbar
#endif // HOLDBAR_PATCH

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#if !NODMENU_PATCH
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
#endif // NODMENU_PATCH
static const char *dmenucmd[] = {
	"dmenu_run",
	#if !NODMENU_PATCH
	"-m", dmenumon,
	#endif // NODMENU_PATCH
	"-fn", dmenufont,
	"-nb", normbgcolor,
	"-nf", normfgcolor,
	"-sb", selbgcolor,
	"-sf", selfgcolor,
	#if DMENUMATCHTOP_PATCH
	topbar ? NULL : "-b",
	#endif // DMENUMATCHTOP_PATCH
	NULL
};
static const char *termcmd[]  = { "st", NULL };

#if STATUSCMD_PATCH && !DWMBLOCKS_PATCH
/* commands spawned when clicking statusbar, the mouse button pressed is exported as BUTTON */
static const char *statuscmds[] = { "notify-send Mouse$BUTTON" };
static char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };
#endif // STATUSCMD_PATCH | DWMBLOCKS_PATCH

static Key keys[] = {
	/* modifier                     key            function                argument */
	#if KEYMODES_PATCH
	{ MODKEY,                       XK_Escape,     setkeymode,             {.ui = COMMANDMODE} },
	#endif // KEYMODES_PATCH
	{ MODKEY,                       XK_p,          spawn,                  {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = termcmd } },
	{ MODKEY,                       XK_b,          togglebar,              {0} },
	#if STACKER_PATCH
	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)
	#else
	{ MODKEY,                       XK_j,          focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,             {.i = -1 } },
	#endif // STACKER_PATCH
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	{ MODKEY,                       XK_s,          swapfocus,              {.i = -1 } },
	#endif // SWAPFOCUS_PATCH
	#if SWITCHCOL_PATCH
	{ MODKEY,                       XK_v,          switchcol,              {0} },
	#endif // SWITCHCOL_PATCH
	#if ROTATESTACK_PATCH
	{ MODKEY|Mod4Mask,              XK_j,          rotatestack,            {.i = +1 } },
	{ MODKEY|Mod4Mask,              XK_k,          rotatestack,            {.i = -1 } },
	#endif // ROTATESTACK_PATCH
	#if INPLACEROTATE_PATCH
	{ MODKEY|Mod4Mask|ShiftMask,    XK_j,          inplacerotate,          {.i = +1} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_k,          inplacerotate,          {.i = -1} },
	#endif // INPLACEROTATE_PATCH
	#if PUSH_PATCH || PUSH_NO_MASTER_PATCH
	{ MODKEY|ControlMask,           XK_j,          pushdown,               {0} },
	{ MODKEY|ControlMask,           XK_k,          pushup,                 {0} },
	#endif // PUSH_PATCH / PUSH_NO_MASTER_PATCH
	{ MODKEY,                       XK_i,          incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,             {.i = -1 } },
	#if FLEXTILE_DELUXE_LAYOUT
	{ MODKEY|ControlMask,           XK_i,          incnstack,              {.i = +1 } },
	{ MODKEY|ControlMask,           XK_u,          incnstack,              {.i = -1 } },
	#endif // FLEXTILE_DELUXE_LAYOUT
	{ MODKEY,                       XK_h,          setmfact,               {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,               {.f = +0.05} },
	#if CFACTS_PATCH
	{ MODKEY|ShiftMask,             XK_h,          setcfact,               {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,          setcfact,               {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,          setcfact,               {0} },
	#endif // CFACTS_PATCH
	#if MOVERESIZE_PATCH
	{ MODKEY|Mod4Mask,              XK_Down,       moveresize,             {.v = "0x 25y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Up,         moveresize,             {.v = "0x -25y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Right,      moveresize,             {.v = "25x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Left,       moveresize,             {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Down,       moveresize,             {.v = "0x 0y 0w 25h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Up,         moveresize,             {.v = "0x 0y 0w -25h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Right,      moveresize,             {.v = "0x 0y 25w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Left,       moveresize,             {.v = "0x 0y -25w 0h" } },
	#endif // MOVERESIZE_PATCH
	#if MOVESTACK_PATCH
	{ MODKEY|ShiftMask,             XK_j,          movestack,              {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,          movestack,              {.i = -1 } },
	#endif // MOVESTACK_PATCH
	#if TRANSFER_PATCH
	{ MODKEY,                       XK_x,          transfer,               {0} },
	#endif // TRANSFER_PATCH
	#if TRANSFER_ALL_PATCH
	{ MODKEY|ControlMask,           XK_x,          transferall,            {0} },
	#endif // TRANSFER_ALL_PATCH
	{ MODKEY,                       XK_Return,     zoom,                   {0} },
	#if VANITYGAPS_PATCH
	{ MODKEY|Mod4Mask,              XK_u,          incrgaps,               {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,          incrgaps,               {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,          incrigaps,              {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,          incrigaps,              {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,          incrogaps,              {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,          incrogaps,              {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,          incrihgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,          incrihgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,          incrivgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,          incrivgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,          incrohgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,          incrohgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,          incrovgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,          incrovgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,          togglegaps,             {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,          defaultgaps,            {0} },
	#endif // VANITYGAPS_PATCH
	{ MODKEY,                       XK_Tab,        view,                   {0} },
	#if SHIFTVIEW_PATCH
	{ MODKEY|ShiftMask,             XK_Tab,        shiftview,              { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_backslash,  shiftview,              { .i = +1 } },
  	#endif // SHIFTVIEW_PATCH
  	#if SHIFTVIEW_CLIENTS_PATCH
	{ MODKEY|Mod4Mask,              XK_Tab,        shiftviewclients,       { .i = -1 } },
	{ MODKEY|Mod4Mask,              XK_backslash,  shiftviewclients,       { .i = +1 } },
  	#endif // SHIFTVIEW_CLIENTS_PATCH
	#if AWESOMEBAR_PATCH
	{ MODKEY|ControlMask,           XK_z,          showhideclient,         {0} },
	#endif // AWESOMEBAR_PATCH
	{ MODKEY|ShiftMask,             XK_c,          killclient,             {0} },
	#if KILLUNSEL_PATCH
	{ MODKEY|ShiftMask,             XK_x,          killunsel,              {0} },
	#endif // KILLUNSEL_PATCH
	#if SELFRESTART_PATCH
	{ MODKEY|ShiftMask,             XK_r,          self_restart,           {0} },
	#endif // SELFRESTART_PATCH
	{ MODKEY|ShiftMask,             XK_q,          quit,                   {0} },
	#if RESTARTSIG_PATCH
	{ MODKEY|ControlMask|ShiftMask, XK_q,          quit,                   {1} },
	#endif // RESTARTSIG_PATCH
	#if FOCUSURGENT_PATCH
	{ MODKEY,                       XK_u,          focusurgent,            {0} },
	#endif // FOCUSURGENT_PATCH
	#if HOLDBAR_PATCH
	{ 0,                            HOLDKEY,       holdbar,                {0} },
	#endif // HOLDBAR_PATCH
	#if WINVIEW_PATCH
	{ MODKEY,                       XK_o,          winview,                {0} },
	#endif // WINVIEW_PATCH
	#if XRDB_PATCH && !VTCOLORS_PATCH
	{ MODKEY|ShiftMask,             XK_F5,         xrdb,                   {.v = NULL } },
	#endif // XRDB_PATCH
	{ MODKEY,                       XK_t,          setlayout,              {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,          setlayout,              {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,          setlayout,              {.v = &layouts[2]} },
	#if COLUMNS_LAYOUT
	{ MODKEY,                       XK_c,          setlayout,              {.v = &layouts[3]} },
	#endif // COLUMNS_LAYOUT
	#if FLEXTILE_DELUXE_LAYOUT
	{ MODKEY|ControlMask,           XK_t,          rotatelayoutaxis,       {.i = +1 } },   /* flextile, 1 = layout axis */
	{ MODKEY|ControlMask,           XK_Tab,        rotatelayoutaxis,       {.i = +2 } },   /* flextile, 2 = master axis */
	{ MODKEY|ControlMask|ShiftMask, XK_Tab,        rotatelayoutaxis,       {.i = +3 } },   /* flextile, 3 = stack axis */
	{ MODKEY|ControlMask|Mod1Mask,  XK_Tab,        rotatelayoutaxis,       {.i = +4 } },   /* flextile, 4 = secondary stack axis */
	{ MODKEY|Mod5Mask,              XK_t,          rotatelayoutaxis,       {.i = -1 } },   /* flextile, 1 = layout axis */
	{ MODKEY|Mod5Mask,              XK_Tab,        rotatelayoutaxis,       {.i = -2 } },   /* flextile, 2 = master axis */
	{ MODKEY|Mod5Mask|ShiftMask,    XK_Tab,        rotatelayoutaxis,       {.i = -3 } },   /* flextile, 3 = stack axis */
	{ MODKEY|Mod5Mask|Mod1Mask,     XK_Tab,        rotatelayoutaxis,       {.i = -4 } },   /* flextile, 4 = secondary stack axis */
	{ MODKEY|ControlMask,           XK_Return,     mirrorlayout,           {0} },         /* flextile, flip master and stack areas */
	#endif // FLEXTILE_DELUXE_LAYOUT
	{ MODKEY,                       XK_space,      setlayout,              {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating,         {0} },
	#if MAXIMIZE_PATCH
	{ MODKEY|ControlMask|ShiftMask, XK_h,          togglehorizontalmax,    {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_l,          togglehorizontalmax,    {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_j,          toggleverticalmax,      {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_k,          toggleverticalmax,      {0} },
	{ MODKEY|ControlMask,           XK_m,          togglemax,              {0} },
	#endif // MAXIMIZE_PATCH
	#if SCRATCHPADS_PATCH
	{ MODKEY,                       XK_grave,      togglescratch,          {.ui = 0 } },
	{ MODKEY|ControlMask,           XK_grave,      togglescratch,          {.ui = 1 } },
	{ MODKEY|ShiftMask,             XK_grave,      togglescratch,          {.ui = 2 } },
	#endif // SCRATCHPADS_PATCH
	#if UNFLOATVISIBLE_PATCH
	{ MODKEY|Mod4Mask,              XK_space,      unfloatvisible,         {0} },
	{ MODKEY|ShiftMask,             XK_t,          unfloatvisible,         {.v = &layouts[0]} },
	#endif // UNFLOATVISIBLE_PATCH
	#if TOGGLEFULLSCREEN_PATCH
	{ MODKEY,                       XK_y,          togglefullscreen,       {0} },
	#endif // TOGGLEFULLSCREEN_PATCH
	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	{ MODKEY|ShiftMask,             XK_y,          togglefakefullscreen,   {0} },
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#if FULLSCREEN_PATCH
	{ MODKEY|ShiftMask,             XK_f,          fullscreen,             {0} },
	#endif // FULLSCREEN_PATCH
	#if STICKY_PATCH
	{ MODKEY|ShiftMask,             XK_s,          togglesticky,           {0} },
	#endif // STICKY_PATCH
	#if SCRATCHPAD_ALT_1_PATCH
	{ MODKEY,                       XK_minus,      scratchpad_show,        {0} },
	{ MODKEY|ShiftMask,             XK_minus,      scratchpad_hide,        {0} },
	{ MODKEY,                       XK_equal,      scratchpad_remove,      {0} },
	#else
	{ MODKEY,                       XK_0,          view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~0 } },
	#endif // SCRATCHPAD_ALT_1_PATCH
	{ MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },
	#if FOCUSADJACENTTAG_PATCH
	{ MODKEY,                       XK_Left,       viewtoleft,             {0} },
	{ MODKEY,                       XK_Right,      viewtoright,            {0} },
	{ MODKEY|ShiftMask,             XK_Left,       tagtoleft,              {0} },
	{ MODKEY|ShiftMask,             XK_Right,      tagtoright,             {0} },
	{ MODKEY|ControlMask,           XK_Left,       tagandviewtoleft,       {0} },
	{ MODKEY|ControlMask,           XK_Right,      tagandviewtoright,      {0} },
	#endif // FOCUSADJACENTTAG_PATCH
	#if TAGALL_PATCH
	{ MODKEY|ShiftMask,             XK_F1,         tagall,                 {.v = "F1"} },
	{ MODKEY|ShiftMask,             XK_F2,         tagall,                 {.v = "F2"} },
	{ MODKEY|ShiftMask,             XK_F3,         tagall,                 {.v = "F3"} },
	{ MODKEY|ShiftMask,             XK_F4,         tagall,                 {.v = "F4"} },
	{ MODKEY|ShiftMask,             XK_F5,         tagall,                 {.v = "F5"} },
	{ MODKEY|ShiftMask,             XK_F6,         tagall,                 {.v = "F6"} },
	{ MODKEY|ShiftMask,             XK_F7,         tagall,                 {.v = "F7"} },
	{ MODKEY|ShiftMask,             XK_F8,         tagall,                 {.v = "F8"} },
	{ MODKEY|ShiftMask,             XK_F9,         tagall,                 {.v = "F9"} },
	{ MODKEY|ControlMask,           XK_F1,         tagall,                 {.v = "1"} },
	{ MODKEY|ControlMask,           XK_F2,         tagall,                 {.v = "2"} },
	{ MODKEY|ControlMask,           XK_F3,         tagall,                 {.v = "3"} },
	{ MODKEY|ControlMask,           XK_F4,         tagall,                 {.v = "4"} },
	{ MODKEY|ControlMask,           XK_F5,         tagall,                 {.v = "5"} },
	{ MODKEY|ControlMask,           XK_F6,         tagall,                 {.v = "6"} },
	{ MODKEY|ControlMask,           XK_F7,         tagall,                 {.v = "7"} },
	{ MODKEY|ControlMask,           XK_F8,         tagall,                 {.v = "8"} },
	{ MODKEY|ControlMask,           XK_F9,         tagall,                 {.v = "9"} },
	#endif // TAGALL_PATCH
	#if TAGALLMON_PATCH
	{ MODKEY|Mod4Mask|ShiftMask,    XK_comma,      tagallmon,              {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_period,     tagallmon,              {.i = -1 } },
	#endif // TAGALLMON_PATCH
	#if TAGSWAPMON_PATCH
	{ MODKEY|Mod4Mask|ControlMask,  XK_comma,      tagswapmon,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_period,     tagswapmon,             {.i = -1 } },
	#endif // TAGSWAPMON_PATCH
	#if ALTERNATIVE_TAGS_PATCH
	{ MODKEY,                       XK_n,          togglealttag,           {0} },
	#endif // ALTERNATIVE_TAGS_PATCH
	#if TAGGRID_PATCH
	{ MODKEY|ControlMask,           XK_Up,         switchtag,              { .ui = SWITCHTAG_UP    | SWITCHTAG_VIEW } },
	{ MODKEY|ControlMask,           XK_Down,       switchtag,              { .ui = SWITCHTAG_DOWN  | SWITCHTAG_VIEW } },
	{ MODKEY|ControlMask,           XK_Right,      switchtag,              { .ui = SWITCHTAG_RIGHT | SWITCHTAG_VIEW } },
	{ MODKEY|ControlMask,           XK_Left,       switchtag,              { .ui = SWITCHTAG_LEFT  | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Up,         switchtag,              { .ui = SWITCHTAG_UP    | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Down,       switchtag,              { .ui = SWITCHTAG_DOWN  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Right,      switchtag,              { .ui = SWITCHTAG_RIGHT | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Left,       switchtag,              { .ui = SWITCHTAG_LEFT  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	#endif // TAGGRID_PATCH
	#if MOVEPLACE_PATCH
	{ MODKEY,                       XK_KP_7,       moveplace,              {.ui = WIN_NW }},   /* XK_KP_Home,  */
	{ MODKEY,                       XK_KP_8,       moveplace,              {.ui = WIN_N  }},   /* XK_KP_Up,    */
	{ MODKEY,                       XK_KP_9,       moveplace,              {.ui = WIN_NE }},   /* XK_KP_Prior, */
	{ MODKEY,                       XK_KP_4,       moveplace,              {.ui = WIN_W  }},   /* XK_KP_Left,  */
	{ MODKEY,                       XK_KP_5,       moveplace,              {.ui = WIN_C  }},   /* XK_KP_Begin, */
	{ MODKEY,                       XK_KP_6,       moveplace,              {.ui = WIN_E  }},   /* XK_KP_Right, */
	{ MODKEY,                       XK_KP_1,       moveplace,              {.ui = WIN_SW }},   /* XK_KP_End,   */
	{ MODKEY,                       XK_KP_2,       moveplace,              {.ui = WIN_S  }},   /* XK_KP_Down,  */
	{ MODKEY,                       XK_KP_3,       moveplace,              {.ui = WIN_SE }},   /* XK_KP_Next,  */
	#endif // MOVEPLACE_PATCH
	#if EXRESIZE_PATCH
	{ MODKEY,                       XK_KP_7,       explace,                {.ui = EX_NW }},   /* XK_KP_Home,  */
	{ MODKEY,                       XK_KP_8,       explace,                {.ui = EX_N  }},   /* XK_KP_Up,    */
	{ MODKEY,                       XK_KP_9,       explace,                {.ui = EX_NE }},   /* XK_KP_Prior, */
	{ MODKEY,                       XK_KP_4,       explace,                {.ui = EX_W  }},   /* XK_KP_Left,  */
	{ MODKEY,                       XK_KP_5,       explace,                {.ui = EX_C  }},   /* XK_KP_Begin, */
	{ MODKEY,                       XK_KP_6,       explace,                {.ui = EX_E  }},   /* XK_KP_Right, */
	{ MODKEY,                       XK_KP_1,       explace,                {.ui = EX_SW }},   /* XK_KP_End,   */
	{ MODKEY,                       XK_KP_2,       explace,                {.ui = EX_S  }},   /* XK_KP_Down,  */
	{ MODKEY,                       XK_KP_3,       explace,                {.ui = EX_SE }},   /* XK_KP_Next,  */

	{ MODKEY|ShiftMask,             XK_KP_8,       exresize,               {.v = (int []){   0,  25 }}},  /* XK_KP_Up,    */
	{ MODKEY|ShiftMask,             XK_KP_2,       exresize,               {.v = (int []){   0, -25 }}},  /* XK_KP_Down,  */
	{ MODKEY|ShiftMask,             XK_KP_6,       exresize,               {.v = (int []){  25,   0 }}},  /* XK_KP_Right, */
	{ MODKEY|ShiftMask,             XK_KP_4,       exresize,               {.v = (int []){ -25,   0 }}},  /* XK_KP_Left,  */
	{ MODKEY|ShiftMask,             XK_KP_5,       exresize,               {.v = (int []){  25,  25 }}},  /* XK_KP_Begin, */
	{ MODKEY|ShiftMask|ControlMask, XK_KP_5,       exresize,               {.v = (int []){ -25, -25 }}},  /* XK_KP_Begin, */

	{ MODKEY|ControlMask,           XK_KP_6,       togglehorizontalexpand, {.i = +1} },  /* XK_KP_Right, */
	{ MODKEY|ControlMask,           XK_KP_3,       togglehorizontalexpand, {.i =  0} },  /* XK_KP_Next,  */
	{ MODKEY|ControlMask,           XK_KP_4,       togglehorizontalexpand, {.i = -1} },  /* XK_KP_Left,  */
	{ MODKEY|ControlMask,           XK_KP_8,       toggleverticalexpand,   {.i = +1} },  /* XK_KP_Up,    */
	{ MODKEY|ControlMask,           XK_KP_1,       toggleverticalexpand,   {.i =  0} },  /* XK_KP_End,   */
	{ MODKEY|ControlMask,           XK_KP_2,       toggleverticalexpand,   {.i = -1} },  /* XK_KP_Down,  */
	{ MODKEY|ControlMask,           XK_KP_9,       togglemaximize,         {.i = -1} },  /* XK_KP_Prior, */
	{ MODKEY|ControlMask,           XK_KP_7,       togglemaximize,         {.i = +1} },  /* XK_KP_Home,  */
	{ MODKEY|ControlMask,           XK_KP_5,       togglemaximize,         {.i =  0} },  /* XK_KP_Begin, */
	#endif // EXRESIZE_PATCH
	#if SETBORDERPX_PATCH
	{ MODKEY|ControlMask,           XK_minus,      setborderpx,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_plus,       setborderpx,            {.i = +1 } },
	{ MODKEY|ControlMask,           XK_numbersign, setborderpx,            {.i = 0 } },
	#endif // SETBORDERPX_PATCH
	#if CYCLELAYOUTS_PATCH
	{ MODKEY|ControlMask,           XK_comma,      cyclelayout,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,     cyclelayout,            {.i = +1 } },
	#endif // CYCLELAYOUTS_PATCH
	#if MDPCONTROL_PATCH
	{ MODKEY,                       XK_F1,         mpdchange,              {.i = -1} },
	{ MODKEY,                       XK_F2,         mpdchange,              {.i = +1} },
	{ MODKEY,                       XK_Escape,     mpdcontrol,             {0} },
	#endif // MDPCONTROL_PATCH
	TAGKEYS(                        XK_1,                                  0)
	TAGKEYS(                        XK_2,                                  1)
	TAGKEYS(                        XK_3,                                  2)
	TAGKEYS(                        XK_4,                                  3)
	TAGKEYS(                        XK_5,                                  4)
	TAGKEYS(                        XK_6,                                  5)
	TAGKEYS(                        XK_7,                                  6)
	TAGKEYS(                        XK_8,                                  7)
	TAGKEYS(                        XK_9,                                  8)
};

#if KEYMODES_PATCH
static Key cmdkeys[] = {
	/* modifier                    keys                     function         argument */
	{ 0,                           XK_Escape,               clearcmd,        {0} },
	{ ControlMask,                 XK_c,                    clearcmd,        {0} },
	{ 0,                           XK_i,                    setkeymode,      {.ui = INSERTMODE} },
};

static Command commands[] = {
	/* modifier (4 keys)                          keysyms (4 keys)                                function         argument */
	{ {ControlMask, ShiftMask,  0,         0},    {XK_w,      XK_h,     0,         0},            setlayout,       {.v = &layouts[0]} },
	{ {ControlMask, 0,          0,         0},    {XK_w,      XK_o,     0,         0},            setlayout,       {.v = &layouts[2]} },
	{ {ControlMask, ShiftMask,  0,         0},    {XK_w,      XK_o,     0,         0},            onlyclient,      {0} },
	{ {ControlMask, 0,          0,         0},    {XK_w,      XK_v,     0,         0},            setlayout,       {.v = &layouts[0]} },
	{ {ControlMask, 0,          0,         0},    {XK_w,      XK_less,  0,         0},            setmfact,        {.f = -0.05} },
	{ {ControlMask, ShiftMask,  0,         0},    {XK_w,      XK_less,  0,         0},            setmfact,        {.f = +0.05} },
	{ {ControlMask, ShiftMask,  0,         0},    {XK_w,      XK_0,     0,         0},            setmfact,        {.f = +1.50} },
	{ {ShiftMask,   0,          0,         0},    {XK_period, XK_e,     0,         0},            spawn,           {.v = dmenucmd} },
	{ {ShiftMask,   0,          0,         0},    {XK_period, XK_o,     0,         0},            spawn,           {.v = dmenucmd} },
	{ {ShiftMask,   0,          0,         0},    {XK_period, XK_q,     XK_Return, 0},            quit,            {0} },
	{ {ShiftMask,   0,          0,         0},    {XK_period, XK_b,     XK_d,      XK_Return},    killclient,      {0} },
	{ {ShiftMask,   0,          0,         0},    {XK_period, XK_b,     XK_n,      XK_Return},    focusstack,      {.i = +1} },
	{ {ShiftMask,   0,          ShiftMask, 0},    {XK_period, XK_b,     XK_n,      XK_Return},    focusstack,      {.i = -1} },
};
#endif // KEYMODES_PATCH

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
	#if AWESOMEBAR_PATCH
	{ ClkWinTitle,          0,                   Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,                   Button3,        showhideclient, {0} },
	#endif // AWESOMEBAR_PATCH
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	#if STATUSCMD_PATCH && DWMBLOCKS_PATCH
	{ ClkStatusText,        0,                   Button1,        sigdwmblocks,   {.i = 1 } },
	{ ClkStatusText,        0,                   Button2,        sigdwmblocks,   {.i = 2 } },
	{ ClkStatusText,        0,                   Button3,        sigdwmblocks,   {.i = 3 } },
	#elif STATUSCMD_PATCH
	{ ClkStatusText,        0,                   Button1,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,                   Button3,        spawn,          {.v = statuscmd } },
	#else
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = termcmd } },
	#endif // STATUSCMD_PATCH
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	#if DRAGCFACT_PATCH && CFACTS_PATCH
	{ ClkClientWin,         MODKEY|ShiftMask,    Button3,        dragcfact,      {0} },
	#endif // DRAGCFACT_PATCH
	#if DRAGMFACT_PATCH
	{ ClkClientWin,         MODKEY|ShiftMask,    Button1,        dragmfact,      {0} },
	#endif // DRAGMFACT_PATCH
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};
