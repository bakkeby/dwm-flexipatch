/* See LICENSE file for copyright and license details. */

/* appearance */
#if ROUNDED_CORNERS_PATCH
static const unsigned int borderpx       = 0;   /* border pixel of windows */
static const int corner_radius           = 10;
#else
static const unsigned int borderpx       = 1;   /* border pixel of windows */
#endif // ROUNDED_CORNERS_PATCH
static const unsigned int snap           = 32;  /* snap pixel */
#if SWALLOW_PATCH
static const int swallowfloating         = 0;   /* 1 means swallow floating windows by default */
#endif // SWALLOW_PATCH
#if NO_MOD_BUTTONS_PATCH
static int nomodbuttons                  = 1;   /* allow client mouse button bindings that have no modifier */
#endif // NO_MOD_BUTTONS_PATCH
#if VANITYGAPS_PATCH
static const unsigned int gappih         = 20;  /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;  /* vert inner gap between windows */
static const unsigned int gappoh         = 10;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 30;  /* vert outer gap between windows and screen edge */
static const int smartgaps_fact          = 1;   /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
#endif // VANITYGAPS_PATCH
#if AUTOSTART_PATCH
static const char autostartblocksh[]     = "autostart_blocking.sh";
static const char autostartsh[]          = "autostart.sh";
static const char dwmdir[]               = "dwm";
static const char localshare[]           = ".local/share";
#endif // AUTOSTART_PATCH
#if BAR_ANYBAR_PATCH
static const int usealtbar               = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass           = "Polybar"; /* Alternate bar class name */
static const char *altbarcmd             = "$HOME/bar.sh"; /* Alternate bar launch command */
#endif // BAR_ANYBAR_PATCH
#if BAR_HOLDBAR_PATCH
static const int showbar                 = 0;   /* 0 means no bar */
#else
static const int showbar                 = 1;   /* 0 means no bar */
#endif // BAR_HOLDBAR_PATCH
static const int topbar                  = 1;   /* 0 means bottom bar */
#if TAB_PATCH
/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab                 = showtab_auto;        /* Default tab bar show mode */
static const int toptab                  = False;               /* False means bottom tab bar */
#endif // TAB_PATCH
#if BAR_HEIGHT_PATCH
static const int bar_height              = 0;   /* 0 means derive from font, >= 1 explicit height */
#endif // BAR_HEIGHT_PATCH
#if BAR_PADDING_PATCH
static const int vertpad                 = 10;  /* vertical padding of bar */
static const int sidepad                 = 10;  /* horizontal padding of bar */
#endif // BAR_PADDING_PATCH
#if BAR_WINICON_PATCH
#define ICONSIZE 20    /* icon size */
#define ICONSPACING 5  /* space between icon and title */
#endif // BAR_WINICON_PATCH
#if FOCUSONCLICK_PATCH
static const int focusonwheel            = 0;
#endif // FOCUSONCLICK_PATCH
#if FLOATPOS_PATCH
static int floatposgrid_x                = 5;  /* float grid columns */
static int floatposgrid_y                = 5;  /* float grid rows */
#endif // FLOATPOS_PATCH
#if RIODRAW_PATCH
static const char slopspawnstyle[]       = "-t 0 -c 0.92,0.85,0.69,0.3 -o"; /* do NOT define -f (format) here */
static const char slopresizestyle[]      = "-t 0 -c 0.92,0.85,0.69,0.3"; /* do NOT define -f (format) here */
static const int riodraw_borders         = 0;  /* 0 or 1, indicates whether the area drawn using slop includes the window borders */
#if SWALLOW_PATCH
static const int riodraw_matchpid        = 1;  /* 0 or 1, indicates whether to match the PID of the client that was spawned with riospawn */
#endif // SWALLOW_PATCH
#endif // RIODRAW_PATCH
/* Status is to be shown on: -1 (all monitors), 0 (a specific monitor by index), 'A' (active monitor) */
#if BAR_STATUSALLMONS_PATCH
static const int statusmon               = -1;
#elif BAR_STATICSTATUS_PATCH
static const int statusmon               = 0;
#else
static const int statusmon               = 'A';
#endif // BAR_STATUSALLMONS_PATCH | BAR_STATICSTATUS_PATCH
#if BAR_STATUSPADDING_PATCH
static const int horizpadbar             = 2;   /* horizontal padding for statusbar */
static const int vertpadbar              = 0;   /* vertical padding for statusbar */
#endif // BAR_STATUSPADDING_PATCH
#if BAR_STATUSBUTTON_PATCH
static const char buttonbar[]            = "<O>";
#endif // BAR_STATUSBUTTON_PATCH
#if BAR_SYSTRAY_PATCH
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int showsystray             = 1;   /* 0 means no systray */
#endif // BAR_SYSTRAY_PATCH
#if BAR_TAGLABELS_PATCH
static const char ptagf[] = "[%s %s]";          /* format of a tag label */
static const char etagf[] = "[%s]";             /* format of an empty tag */
static const int lcaselbl = 0;                  /* 1 means make tag label lowercase */
#endif // BAR_TAGLABELS_PATCH
#if BAR_UNDERLINETAGS_PATCH
static const unsigned int ulinepad = 5;         /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke  = 2;     /* thickness / height of the underline */
static const unsigned int ulinevoffset = 0;     /* how far above the bottom of the bar the line should appear */
static const int ulineall = 0;                  /* 1 to show underline on all tags, 0 for just the active ones */
#endif // BAR_UNDERLINETAGS_PATCH

/* Indicators: see patch/bar_indicators.h for options */
static int tagindicatortype              = INDICATOR_TOP_LEFT_SQUARE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;
#if FAKEFULLSCREEN_CLIENT_PATCH && !FAKEFULLSCREEN_PATCH
static int fakefsindicatortype           = INDICATOR_PLUS;
static int floatfakefsindicatortype      = INDICATOR_PLUS_AND_LARGER_SQUARE;
#endif // FAKEFULLSCREEN_CLIENT_PATCH
#if ONLYQUITONEMPTY_PATCH
static const int quit_empty_window_count = 0;   /* only allow dwm to quit if no (<= count) windows are open */
#endif // ONLYQUITONEMPTY_PATCH
#if BAR_EXTRASTATUS_PATCH
static const char statussep              = ';'; /* separator between status bars */
#endif // BAR_EXTRASTATUS_PATCH
#if BAR_TABGROUPS_PATCH
#if MONOCLE_LAYOUT
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
#else
static void (*bartabmonfns[])(Monitor *) = { NULL /* , customlayoutfn */ };
#endif // MONOCLE_LAYOUT
#endif // BAR_TABGROUPS_PATCH
#if BAR_PANGO_PATCH
static const char font[]                 = "monospace 10";
#else
static const char *fonts[]               = { "monospace:size=10" };
#endif // BAR_PANGO_PATCH
static const char dmenufont[]            = "monospace:size=10";

static char c000000[]                    = "#000000"; // placeholder value

#if BAR_FLEXWINTITLE_PATCH
#endif // BAR_FLEXWINTITLE_PATCH
static char normfgcolor[]                = "#bbbbbb";
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfloatcolor[]             = "#db8fd9";

static char selfgcolor[]                 = "#eeeeee";
static char selbgcolor[]                 = "#005577";
static char selbordercolor[]             = "#005577";
static char selfloatcolor[]              = "#005577";

static char titlenormfgcolor[]           = "#bbbbbb";
static char titlenormbgcolor[]           = "#222222";
static char titlenormbordercolor[]       = "#444444";
static char titlenormfloatcolor[]        = "#db8fd9";

static char titleselfgcolor[]            = "#eeeeee";
static char titleselbgcolor[]            = "#005577";
static char titleselbordercolor[]        = "#005577";
static char titleselfloatcolor[]         = "#005577";

static char tagsnormfgcolor[]            = "#bbbbbb";
static char tagsnormbgcolor[]            = "#222222";
static char tagsnormbordercolor[]        = "#444444";
static char tagsnormfloatcolor[]         = "#db8fd9";

static char tagsselfgcolor[]             = "#eeeeee";
static char tagsselbgcolor[]             = "#005577";
static char tagsselbordercolor[]         = "#005577";
static char tagsselfloatcolor[]          = "#005577";

static char hidnormfgcolor[]             = "#005577";
static char hidselfgcolor[]              = "#227799";
static char hidnormbgcolor[]             = "#222222";
static char hidselbgcolor[]              = "#222222";

static char urgfgcolor[]                 = "#bbbbbb";
static char urgbgcolor[]                 = "#222222";
static char urgbordercolor[]             = "#ff0000";
static char urgfloatcolor[]              = "#db8fd9";

#if BAR_FLEXWINTITLE_PATCH
static char normTTBbgcolor[]             = "#330000";
static char normLTRbgcolor[]             = "#330033";
static char normMONObgcolor[]            = "#000033";
static char normGRIDbgcolor[]            = "#003300";
static char normGRD1bgcolor[]            = "#003300";
static char normGRD2bgcolor[]            = "#003300";
static char normGRDMbgcolor[]            = "#506600";
static char normHGRDbgcolor[]            = "#b96600";
static char normDWDLbgcolor[]            = "#003333";
static char normSPRLbgcolor[]            = "#333300";
static char normfloatbgcolor[]           = "#115577";
static char actTTBbgcolor[]              = "#440000";
static char actLTRbgcolor[]              = "#440044";
static char actMONObgcolor[]             = "#000044";
static char actGRIDbgcolor[]             = "#004400";
static char actGRD1bgcolor[]             = "#004400";
static char actGRD2bgcolor[]             = "#004400";
static char actGRDMbgcolor[]             = "#507711";
static char actHGRDbgcolor[]             = "#b97711";
static char actDWDLbgcolor[]             = "#004444";
static char actSPRLbgcolor[]             = "#444400";
static char actfloatbgcolor[]            = "#116688";
static char selTTBbgcolor[]              = "#550000";
static char selLTRbgcolor[]              = "#550055";
static char selMONObgcolor[]             = "#212171";
static char selGRIDbgcolor[]             = "#005500";
static char selGRD1bgcolor[]             = "#005500";
static char selGRD2bgcolor[]             = "#005500";
static char selGRDMbgcolor[]             = "#508822";
static char selHGRDbgcolor[]             = "#b98822";
static char selDWDLbgcolor[]             = "#005555";
static char selSPRLbgcolor[]             = "#555500";
static char selfloatbgcolor[]            = "#117799";
#endif // BAR_FLEXWINTITLE_PATCH

#if BAR_ALPHA_PATCH
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3] = {
	/*                       fg      bg        border     */
	[SchemeNorm]         = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]          = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleNorm]    = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleSel]     = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsNorm]     = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsSel]      = { OPAQUE, baralpha, borderalpha },
	[SchemeHidNorm]      = { OPAQUE, baralpha, borderalpha },
	[SchemeHidSel]       = { OPAQUE, baralpha, borderalpha },
	[SchemeUrg]          = { OPAQUE, baralpha, borderalpha },
	#if BAR_FLEXWINTITLE_PATCH
	[SchemeFlexActTTB]   = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActLTR]   = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActMONO]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActGRID]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActGRD1]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActGRD2]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActGRDM]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActHGRD]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActDWDL]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActSPRL]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexActFloat] = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaTTB]   = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaLTR]   = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaMONO]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaGRID]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaGRD1]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaGRD2]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaGRDM]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaHGRD]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaDWDL]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaSPRL]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexInaFloat] = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelTTB]   = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelLTR]   = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelMONO]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelGRID]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelGRD1]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelGRD2]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelGRDM]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelHGRD]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelDWDL]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelSPRL]  = { OPAQUE, baralpha, borderalpha },
	[SchemeFlexSelFloat] = { OPAQUE, baralpha, borderalpha },
	#endif // BAR_FLEXWINTITLE_PATCH
};
#endif // BAR_ALPHA_PATCH
#if BAR_VTCOLORS_PATCH
static const char title_bg_dark[]   = "#303030";
static const char title_bg_light[]  = "#fdfdfd";
static const int color_ptrs[][ColCount] = {
	/*                       fg      bg      border  float */
	[SchemeNorm]         = { -1,     -1,     5,      12 },
	[SchemeSel]          = { -1,     -1,     11,     13 },
	[SchemeTitleNorm]    = { 6,      -1,     -1,     -1 },
	[SchemeTitleSel]     = { 6,      -1,     -1,     -1 },
	[SchemeTagsNorm]     = { 2,      0,      0,      -1 },
	[SchemeTagsSel]      = { 6,      5,      5,      -1 },
	[SchemeHidNorm]      = { 5,      0,      0,      -1 },
	[SchemeHidSel]       = { 6,      -1,     -1,     -1 },
	[SchemeUrg]          = { 7,      9,      9,      15 },
};
#endif // BAR_VTCOLORS_PATCH

static char *colors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm]         = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor },
	[SchemeSel]          = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor },
	[SchemeTitleNorm]    = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
	[SchemeTitleSel]     = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor },
	[SchemeTagsNorm]     = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor },
	[SchemeTagsSel]      = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor },
	[SchemeHidNorm]      = { hidnormfgcolor,   hidnormbgcolor,   c000000,              c000000 },
	[SchemeHidSel]       = { hidselfgcolor,    hidselbgcolor,    c000000,              c000000 },
	[SchemeUrg]          = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor },
	#if BAR_FLEXWINTITLE_PATCH
	[SchemeFlexActTTB]   = { titleselfgcolor,  actTTBbgcolor,    actTTBbgcolor,        c000000 },
	[SchemeFlexActLTR]   = { titleselfgcolor,  actLTRbgcolor,    actLTRbgcolor,        c000000 },
	[SchemeFlexActMONO]  = { titleselfgcolor,  actMONObgcolor,   actMONObgcolor,       c000000 },
	[SchemeFlexActGRID]  = { titleselfgcolor,  actGRIDbgcolor,   actGRIDbgcolor,       c000000 },
	[SchemeFlexActGRD1]  = { titleselfgcolor,  actGRD1bgcolor,   actGRD1bgcolor,       c000000 },
	[SchemeFlexActGRD2]  = { titleselfgcolor,  actGRD2bgcolor,   actGRD2bgcolor,       c000000 },
	[SchemeFlexActGRDM]  = { titleselfgcolor,  actGRDMbgcolor,   actGRDMbgcolor,       c000000 },
	[SchemeFlexActHGRD]  = { titleselfgcolor,  actHGRDbgcolor,   actHGRDbgcolor,       c000000 },
	[SchemeFlexActDWDL]  = { titleselfgcolor,  actDWDLbgcolor,   actDWDLbgcolor,       c000000 },
	[SchemeFlexActSPRL]  = { titleselfgcolor,  actSPRLbgcolor,   actSPRLbgcolor,       c000000 },
	[SchemeFlexActFloat] = { titleselfgcolor,  actfloatbgcolor,  actfloatbgcolor,      c000000 },
	[SchemeFlexInaTTB]   = { titlenormfgcolor, normTTBbgcolor,   normTTBbgcolor,       c000000 },
	[SchemeFlexInaLTR]   = { titlenormfgcolor, normLTRbgcolor,   normLTRbgcolor,       c000000 },
	[SchemeFlexInaMONO]  = { titlenormfgcolor, normMONObgcolor,  normMONObgcolor,      c000000 },
	[SchemeFlexInaGRID]  = { titlenormfgcolor, normGRIDbgcolor,  normGRIDbgcolor,      c000000 },
	[SchemeFlexInaGRD1]  = { titlenormfgcolor, normGRD1bgcolor,  normGRD1bgcolor,      c000000 },
	[SchemeFlexInaGRD2]  = { titlenormfgcolor, normGRD2bgcolor,  normGRD2bgcolor,      c000000 },
	[SchemeFlexInaGRDM]  = { titlenormfgcolor, normGRDMbgcolor,  normGRDMbgcolor,      c000000 },
	[SchemeFlexInaHGRD]  = { titlenormfgcolor, normHGRDbgcolor,  normHGRDbgcolor,      c000000 },
	[SchemeFlexInaDWDL]  = { titlenormfgcolor, normDWDLbgcolor,  normDWDLbgcolor,      c000000 },
	[SchemeFlexInaSPRL]  = { titlenormfgcolor, normSPRLbgcolor,  normSPRLbgcolor,      c000000 },
	[SchemeFlexInaFloat] = { titlenormfgcolor, normfloatbgcolor, normfloatbgcolor,     c000000 },
	[SchemeFlexSelTTB]   = { titleselfgcolor,  selTTBbgcolor,    selTTBbgcolor,        c000000 },
	[SchemeFlexSelLTR]   = { titleselfgcolor,  selLTRbgcolor,    selLTRbgcolor,        c000000 },
	[SchemeFlexSelMONO]  = { titleselfgcolor,  selMONObgcolor,   selMONObgcolor,       c000000 },
	[SchemeFlexSelGRID]  = { titleselfgcolor,  selGRIDbgcolor,   selGRIDbgcolor,       c000000 },
	[SchemeFlexSelGRD1]  = { titleselfgcolor,  selGRD1bgcolor,   selGRD1bgcolor,       c000000 },
	[SchemeFlexSelGRD2]  = { titleselfgcolor,  selGRD2bgcolor,   selGRD2bgcolor,       c000000 },
	[SchemeFlexSelGRDM]  = { titleselfgcolor,  selGRDMbgcolor,   selGRDMbgcolor,       c000000 },
	[SchemeFlexSelHGRD]  = { titleselfgcolor,  selHGRDbgcolor,   selHGRDbgcolor,       c000000 },
	[SchemeFlexSelDWDL]  = { titleselfgcolor,  selDWDLbgcolor,   selDWDLbgcolor,       c000000 },
	[SchemeFlexSelSPRL]  = { titleselfgcolor,  selSPRLbgcolor,   selSPRLbgcolor,       c000000 },
	[SchemeFlexSelFloat] = { titleselfgcolor,  selfloatbgcolor,  selfloatbgcolor,      c000000 },
	#endif // BAR_FLEXWINTITLE_PATCH
};

#if BAR_POWERLINE_STATUS_PATCH
static char *statuscolors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm]         = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor },
	[SchemeSel]          = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor },
	[SchemeTitleNorm]    = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
	[SchemeTitleSel]     = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor },
	[SchemeTagsNorm]     = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor },
	[SchemeTagsSel]      = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor },
	[SchemeHidNorm]      = { hidnormfgcolor,   hidnormbgcolor,   c000000,              c000000 },
	[SchemeHidSel]       = { hidselfgcolor,    hidselbgcolor,    c000000,              c000000 },
	[SchemeUrg]          = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor },
};
#endif // BAR_POWERLINE_STATUS_PATCH

#if BAR_LAYOUTMENU_PATCH
static const char *layoutmenu_cmd = "layoutmenu.sh";
#endif

#if COOL_AUTOSTART_PATCH
static const char *const autostart[] = {
	"st", NULL,
	NULL /* terminate */
};
#endif // COOL_AUTOSTART_PATCH

#if SCRATCHPADS_PATCH
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
static Sp scratchpads[] = {
   /* name          cmd  */
   {"spterm",      spcmd1},
};
#endif // SCRATCHPADS_PATCH

/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char *tagicons[][NUMTAGS] = {
	[DEFAULT_TAGS]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9" },
	[ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
	[ALT_TAGS_DECORATION] = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};

#if BAR_TAGGRID_PATCH
/* grid of tags */
#define SWITCHTAG_UP                1 << 0
#define SWITCHTAG_DOWN              1 << 1
#define SWITCHTAG_LEFT              1 << 2
#define SWITCHTAG_RIGHT             1 << 3
#define SWITCHTAG_TOGGLETAG         1 << 4
#define SWITCHTAG_TAG               1 << 5
#define SWITCHTAG_VIEW              1 << 6
#define SWITCHTAG_TOGGLEVIEW        1 << 7

static const int tagrows = 2;
#endif // BAR_TAGGRID_PATCH

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

#if INSETS_PATCH
static const Inset default_inset = {
	.x = 0,
	.y = 30,
	.w = 0,
	.h = 0,
};
#endif // INSETS_PATCH

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor   bar    alignment         widthfunc                drawfunc                clickfunc                name */
	#if BAR_STATUSBUTTON_PATCH
	{ -1,        0,     BAR_ALIGN_LEFT,   width_stbutton,          draw_stbutton,          click_stbutton,          "statusbutton" },
	#endif // BAR_STATUSBUTTON_PATCH
	#if BAR_POWERLINE_TAGS_PATCH
	{  0,        0,     BAR_ALIGN_LEFT,   width_pwrl_tags,         draw_pwrl_tags,         click_pwrl_tags,         "powerline_tags" },
	#endif // BAR_POWERLINE_TAGS_PATCH
	#if BAR_TAGS_PATCH
	{ -1,        0,     BAR_ALIGN_LEFT,   width_tags,              draw_tags,              click_tags,              "tags" },
	#endif // BAR_TAGS_PATCH
	#if BAR_TAGLABELS_PATCH
	{ -1,        0,     BAR_ALIGN_LEFT,   width_taglabels,         draw_taglabels,         click_taglabels,         "taglabels" },
	#endif // BAR_TAGLABELS_PATCH
	#if BAR_TAGGRID_PATCH
	{ -1,        0,     BAR_ALIGN_LEFT,   width_taggrid,           draw_taggrid,           click_taggrid,           "taggrid" },
	#endif // BAR_TAGGRID_PATCH
	#if BAR_SYSTRAY_PATCH
	{  0,        0,     BAR_ALIGN_RIGHT,  width_systray,           draw_systray,           click_systray,           "systray" },
	#endif // BAR_SYSTRAY_PATCH
	#if BAR_LTSYMBOL_PATCH
	{ -1,        0,     BAR_ALIGN_LEFT,   width_ltsymbol,          draw_ltsymbol,          click_ltsymbol,          "layout" },
	#endif // BAR_LTSYMBOL_PATCH
	#if BAR_STATUSCOLORS_PATCH && BAR_STATUSCMD_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_statuscolors,      draw_statuscolors,      click_statuscmd,         "statuscolors" },
	#elif BAR_STATUSCOLORS_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_statuscolors,      draw_statuscolors,      click_statuscolors,      "statuscolors" },
	#elif BAR_STATUS2D_PATCH && BAR_STATUSCMD_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_status2d,          draw_status2d,          click_statuscmd,         "status2d" },
	#elif BAR_STATUS2D_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_status2d,          draw_status2d,          click_status2d,          "status2d" },
	#elif BAR_POWERLINE_STATUS_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_pwrl_status,       draw_pwrl_status,       click_pwrl_status,       "powerline_status" },
	#elif BAR_STATUS_PATCH && BAR_STATUSCMD_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_status,            draw_status,            click_statuscmd,         "status" },
	#elif BAR_STATUS_PATCH
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_status,            draw_status,            click_status,            "status" },
	#endif // BAR_STATUS2D_PATCH | BAR_STATUSCMD_PATCH
	#if XKB_PATCH
	{  0,        0,     BAR_ALIGN_RIGHT,  width_xkb,               draw_xkb,               click_xkb,               "xkb" },
	#endif // XKB_PATCH
	#if BAR_FLEXWINTITLE_PATCH
	{ -1,        0,     BAR_ALIGN_NONE,   width_flexwintitle,      draw_flexwintitle,      click_flexwintitle,      "flexwintitle" },
	#elif BAR_TABGROUPS_PATCH
	{ -1,        0,     BAR_ALIGN_NONE,   width_bartabgroups,      draw_bartabgroups,      click_bartabgroups,      "bartabgroups" },
	#elif BAR_AWESOMEBAR_PATCH
	{ -1,        0,     BAR_ALIGN_NONE,   width_awesomebar,        draw_awesomebar,        click_awesomebar,        "awesomebar" },
	#elif BAR_FANCYBAR_PATCH
	{ -1,        0,     BAR_ALIGN_NONE,   width_fancybar,          draw_fancybar,          click_fancybar,          "fancybar" },
	#elif BAR_WINTITLE_PATCH
	{ -1,        0,     BAR_ALIGN_NONE,   width_wintitle,          draw_wintitle,          click_wintitle,          "wintitle" },
	#endif // BAR_TABGROUPS_PATCH | BAR_AWESOMEBAR_PATCH | BAR_FANCYBAR_PATCH | BAR_WINTITLE_PATCH
	#if BAR_EXTRASTATUS_PATCH
	#if BAR_STATUSCOLORS_PATCH && BAR_STATUSCMD_PATCH
	{ statusmon, 1,     BAR_ALIGN_CENTER, width_statuscolors_es,   draw_statuscolors_es,   click_statuscmd_es,      "statuscolors_es" },
	#elif BAR_STATUSCOLORS_PATCH
	{ statusmon, 1,     BAR_ALIGN_CENTER, width_statuscolors_es,   draw_statuscolors_es,   click_statuscolors,      "statuscolors_es" },
	#elif BAR_STATUS2D_PATCH && BAR_STATUSCMD_PATCH
	{ statusmon, 1,     BAR_ALIGN_CENTER, width_status2d_es,       draw_status2d_es,       click_statuscmd_es,      "status2d_es" },
	#elif BAR_STATUS2D_PATCH
	{ statusmon, 1,     BAR_ALIGN_CENTER, width_status2d_es,       draw_status2d_es,       click_status2d,          "status2d_es" },
	#elif BAR_POWERLINE_STATUS_PATCH
	{ statusmon, 1,     BAR_ALIGN_RIGHT,  width_pwrl_status_es,    draw_pwrl_status_es,    click_pwrl_status,       "powerline_status" },
	#elif BAR_STATUSCMD_PATCH && BAR_STATUS_PATCH
	{ statusmon, 1,     BAR_ALIGN_CENTER, width_status_es,         draw_status_es,         click_statuscmd_es,      "status_es" },
	#elif BAR_STATUS_PATCH
	{ statusmon, 1,     BAR_ALIGN_CENTER, width_status_es,         draw_status_es,         click_status,            "status_es" },
	#endif // BAR_STATUS2D_PATCH | BAR_STATUSCMD_PATCH
	#endif // BAR_EXTRASTATUS_PATCH
	#if BAR_FLEXWINTITLE_PATCH
	#if BAR_WINTITLE_HIDDEN_PATCH
	{ -1,        1,  BAR_ALIGN_RIGHT_RIGHT, width_wintitle_hidden, draw_wintitle_hidden,   click_wintitle_hidden,   "wintitle_hidden" },
	#endif
	#if BAR_WINTITLE_FLOATING_PATCH
	{ -1,        1,     BAR_ALIGN_LEFT,   width_wintitle_floating, draw_wintitle_floating, click_wintitle_floating, "wintitle_floating" },
	#endif // BAR_WINTITLE_FLOATING_PATCH
	#endif // BAR_FLEXWINTITLE_PATCH
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
#if FLEXTILE_DELUXE_LAYOUT
static const int nstack      = 0;    /* number of clients in primary stack area */
#endif // FLEXTILE_DELUXE_LAYOUT
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#if DECORATION_HINTS_PATCH
static const int decorhints  = 1;    /* 1 means respect decoration hints */
#endif // DECORATION_HINTS_PATCH

#if NROWGRID_LAYOUT
#define FORCE_VSPLIT 1
#endif

#if TAPRESIZE_PATCH
/* mouse scroll resize */
static const int scrollsensetivity = 30; /* 1 means resize window by 1 pixel for each scroll event */
/* resizemousescroll direction argument list */
static const int scrollargs[][2] = {
	/* width change         height change */
	{ +scrollsensetivity,	0 },
	{ -scrollsensetivity,	0 },
	{ 0, 				  	+scrollsensetivity },
	{ 0, 					-scrollsensetivity },
};
#endif // TAPRESIZE_PATCH

#if FLEXTILE_DELUXE_LAYOUT
static const Layout layouts[] = {
	/* symbol     arrange function, { nmaster, nstack, layout, master axis, stack axis, secondary stack axis, symbol func } */
	{ "[]=",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, TOP_TO_BOTTOM, 0, NULL } }, // default tile layout
 	{ "><>",      NULL,             {0} },    /* no layout function means floating behavior */
	{ "[M]",      flextile,         { -1, -1, NO_SPLIT, MONOCLE, MONOCLE, 0, NULL } }, // monocle
	{ "|||",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // columns (col) layout
	{ ">M>",      flextile,         { -1, -1, FLOATING_MASTER, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // floating master
	{ "[D]",      flextile,         { -1, -1, SPLIT_VERTICAL, TOP_TO_BOTTOM, MONOCLE, 0, NULL } }, // deck
	{ "TTT",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, LEFT_TO_RIGHT, 0, NULL } }, // bstack
	{ "===",      flextile,         { -1, -1, SPLIT_HORIZONTAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, 0, NULL } }, // bstackhoriz
	{ "|M|",      flextile,         { -1, -1, SPLIT_CENTERED_VERTICAL, LEFT_TO_RIGHT, TOP_TO_BOTTOM, TOP_TO_BOTTOM, NULL } }, // centeredmaster
	{ "-M-",      flextile,         { -1, -1, SPLIT_CENTERED_HORIZONTAL, TOP_TO_BOTTOM, LEFT_TO_RIGHT, LEFT_TO_RIGHT, NULL } }, // centeredmaster horiz
	{ ":::",      flextile,         { -1, -1, NO_SPLIT, GAPPLESSGRID, GAPPLESSGRID, 0, NULL } }, // gappless grid
	{ "[\\]",     flextile,         { -1, -1, NO_SPLIT, DWINDLE, DWINDLE, 0, NULL } }, // fibonacci dwindle
	{ "(@)",      flextile,         { -1, -1, NO_SPLIT, SPIRAL, SPIRAL, 0, NULL } }, // fibonacci spiral
	{ "[T]",      flextile,         { -1, -1, SPLIT_VERTICAL, LEFT_TO_RIGHT, TATAMI, 0, NULL } }, // tatami mats
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

#if XKB_PATCH
/* xkb frontend */
static const char *xkb_layouts[]  = {
	"en",
	"ru",
};
#endif // XKB_PATCH

/* key definitions */
#define MODKEY Mod1Mask
#if COMBO_PATCH && SWAPTAGS_PATCH && TAGOTHERMONITOR_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,              KEY,      tagnextmon,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ControlMask,  KEY,      tagprevmon,     {.ui = 1 << TAG} },
#elif COMBO_PATCH && SWAPTAGS_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} },
#elif COMBO_PATCH && TAGOTHERMONITOR_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,              KEY,      tagnextmon,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ControlMask,  KEY,      tagprevmon,     {.ui = 1 << TAG} },
#elif COMBO_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#elif SWAPTAGS_PATCH && TAGOTHERMONITOR_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,              KEY,      tagnextmon,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ControlMask,  KEY,      tagprevmon,     {.ui = 1 << TAG} },
#elif SWAPTAGS_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ShiftMask,    KEY,      swaptags,       {.ui = 1 << TAG} },
#elif TAGOTHERMONITOR_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask,              KEY,      tagnextmon,     {.ui = 1 << TAG} }, \
	{ MODKEY|Mod4Mask|ControlMask,  KEY,      tagprevmon,     {.ui = 1 << TAG} },
#else
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#endif // COMBO_PATCH / SWAPTAGS_PATCH / TAGOTHERMONITOR_PATCH

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

#if BAR_HOLDBAR_PATCH
#define HOLDKEY 0 // replace 0 with the keysym to activate holdbar
#endif // BAR_HOLDBAR_PATCH

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
	#if BAR_DMENUMATCHTOP_PATCH
	topbar ? NULL : "-b",
	#endif // BAR_DMENUMATCHTOP_PATCH
	NULL
};
static const char *termcmd[]  = { "st", NULL };

#if BAR_STATUSCMD_PATCH
#if BAR_DWMBLOCKS_PATCH
/* This defines the name of the executable that handles the bar (used for signalling purposes) */
#define STATUSBAR "dwmblocks"
#else
/* commands spawned when clicking statusbar, the mouse button pressed is exported as BUTTON */
static const StatusCmd statuscmds[] = {
	{ "notify-send Volume$BUTTON", 1 },
	{ "notify-send CPU$BUTTON", 2 },
	{ "notify-send Battery$BUTTON", 3 },
};
/* test the above with: xsetroot -name "$(printf '\x01Volume |\x02 CPU |\x03 Battery')" */
static const char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };
#endif // BAR_DWMBLOCKS_PATCH
#endif // BAR_STATUSCMD_PATCH

#if ON_EMPTY_KEYS_PATCH
static const char* firefoxcmd[] = {"firefox", NULL};
static Key on_empty_keys[] = {
	/* modifier key            function                argument */
	{ 0,        XK_f,          spawn,                  {.v = firefoxcmd } },
};
#endif // ON_EMPTY_KEYS_PATCH

static Key keys[] = {
	/* modifier                     key            function                argument */
	#if KEYMODES_PATCH
	{ MODKEY,                       XK_Escape,     setkeymode,             {.ui = COMMANDMODE} },
	#endif // KEYMODES_PATCH
	{ MODKEY,                       XK_p,          spawn,                  {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = termcmd } },
	#if RIODRAW_PATCH
	{ MODKEY|ControlMask,           XK_p,          riospawnsync,           {.v = dmenucmd } },
	{ MODKEY|ControlMask,           XK_Return,     riospawn,               {.v = termcmd } },
	{ MODKEY,                       XK_s,          rioresize,              {0} },
	#endif // RIODRAW_PATCH
	{ MODKEY,                       XK_b,          togglebar,              {0} },
	#if TAB_PATCH
	{ MODKEY|ControlMask,           XK_b,          tabmode,                {-1} },
	#endif // TAB_PATCH
	#if FOCUSMASTER_PATCH
	{ MODKEY|ControlMask,           XK_space,      focusmaster,            {0} },
	#endif // FOCUSMASTER_PATCH
	#if STACKER_PATCH
	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)
	#else
	{ MODKEY,                       XK_j,          focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,             {.i = -1 } },
	#endif // STACKER_PATCH
	#if FOCUSDIR_PATCH
	{ MODKEY,                       XK_Left,       focusdir,               {.i = 0 } }, // left
	{ MODKEY,                       XK_Right,      focusdir,               {.i = 1 } }, // right
	{ MODKEY,                       XK_Up,         focusdir,               {.i = 2 } }, // up
	{ MODKEY,                       XK_Down,       focusdir,               {.i = 3 } }, // down
	#endif // FOCUSDIR_PATCH
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
	{ MODKEY|Mod4Mask,              XK_j,          inplacerotate,          {.i = +2 } }, // same as rotatestack
	{ MODKEY|Mod4Mask,              XK_k,          inplacerotate,          {.i = -2 } }, // same as reotatestack
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
	#if ASPECTRESIZE_PATCH
	{ MODKEY|ControlMask|ShiftMask, XK_e,          aspectresize,           {.i = +24} },
	{ MODKEY|ControlMask|ShiftMask, XK_r,          aspectresize,           {.i = -24} },
	#endif // ASPECTRESIZE_PATCH
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
	#if REORGANIZETAGS_PATCH
	{ MODKEY|ControlMask,           XK_r,          reorganizetags,         {0} },
	#endif // REORGANIZETAGS_PATCH
	#if DISTRIBUTETAGS_PATCH
	{ MODKEY|ControlMask,           XK_d,          distributetags,         {0} },
	#endif // DISTRIBUTETAGS_PATCH
	#if INSETS_PATCH
	{ MODKEY|ShiftMask|ControlMask, XK_a,          updateinset,            {.v = &default_inset } },
	#endif // INSETS_PATCH
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
	#if BAR_WINTITLEACTIONS_PATCH
	{ MODKEY|ControlMask,           XK_z,          showhideclient,         {0} },
	#endif // BAR_WINTITLEACTIONS_PATCH
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
	#if BAR_HOLDBAR_PATCH
	{ 0,                            HOLDKEY,       holdbar,                {0} },
	#endif // BAR_HOLDBAR_PATCH
	#if WINVIEW_PATCH
	{ MODKEY,                       XK_o,          winview,                {0} },
	#endif // WINVIEW_PATCH
	#if XRDB_PATCH && !BAR_VTCOLORS_PATCH
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
	{ MODKEY|ControlMask,           XK_Return,     mirrorlayout,           {0} },          /* flextile, flip master and stack areas */
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
	#if NO_MOD_BUTTONS_PATCH
	{ MODKEY|ShiftMask,             XK_Escape,     togglenomodbuttons,     {0} },
	#endif // NO_MOD_BUTTONS_PATCH
	#if SCRATCHPADS_PATCH
	{ MODKEY,                       XK_grave,      togglescratch,          {.ui = 0 } },
	{ MODKEY|ControlMask,           XK_grave,      setscratch,             {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_grave,      removescratch,          {.ui = 0 } },
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
	#elif SCRATCHPADS_PATCH
	{ MODKEY,                       XK_0,          view,                   {.ui = ~SPTAGMASK } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~SPTAGMASK } },
	#else
	{ MODKEY,                       XK_0,          view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~0 } },
	#endif // SCRATCHPAD_ALT_1_PATCH
	{ MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },
	#if FOCUSADJACENTTAG_PATCH
	{ MODKEY,                       XK_Left,       viewtoleft,             {0} }, // note keybinding conflict with focusdir
	{ MODKEY,                       XK_Right,      viewtoright,            {0} }, // note keybinding conflict with focusdir
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
	#if BAR_ALTERNATIVE_TAGS_PATCH
	{ MODKEY,                       XK_n,          togglealttag,           {0} },
	#endif // BAR_ALTERNATIVE_TAGS_PATCH
	#if BAR_TAGGRID_PATCH
	{ MODKEY|ControlMask,           XK_Up,         switchtag,              { .ui = SWITCHTAG_UP    | SWITCHTAG_VIEW } },
	{ MODKEY|ControlMask,           XK_Down,       switchtag,              { .ui = SWITCHTAG_DOWN  | SWITCHTAG_VIEW } },
	{ MODKEY|ControlMask,           XK_Right,      switchtag,              { .ui = SWITCHTAG_RIGHT | SWITCHTAG_VIEW } },
	{ MODKEY|ControlMask,           XK_Left,       switchtag,              { .ui = SWITCHTAG_LEFT  | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Up,         switchtag,              { .ui = SWITCHTAG_UP    | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Down,       switchtag,              { .ui = SWITCHTAG_DOWN  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Right,      switchtag,              { .ui = SWITCHTAG_RIGHT | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{ MODKEY|Mod4Mask,              XK_Left,       switchtag,              { .ui = SWITCHTAG_LEFT  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	#endif // BAR_TAGGRID_PATCH
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
	#if FLOATPOS_PATCH
	/* Note that due to key limitations the below example kybindings are defined with a Mod3Mask,
	 * which is not always readily available. Refer to the patch wiki for more details. */
	/* Client position is limited to monitor window area */
	{ Mod3Mask,                     XK_u,            floatpos,               {.v = "-26x -26y" } }, // ↖
	{ Mod3Mask,                     XK_i,            floatpos,               {.v = "  0x -26y" } }, // ↑
	{ Mod3Mask,                     XK_o,            floatpos,               {.v = " 26x -26y" } }, // ↗
	{ Mod3Mask,                     XK_j,            floatpos,               {.v = "-26x   0y" } }, // ←
	{ Mod3Mask,                     XK_l,            floatpos,               {.v = " 26x   0y" } }, // →
	{ Mod3Mask,                     XK_m,            floatpos,               {.v = "-26x  26y" } }, // ↙
	{ Mod3Mask,                     XK_comma,        floatpos,               {.v = "  0x  26y" } }, // ↓
	{ Mod3Mask,                     XK_period,       floatpos,               {.v = " 26x  26y" } }, // ↘
	/* Absolute positioning (allows moving windows between monitors) */
	{ Mod3Mask|ControlMask,         XK_u,            floatpos,               {.v = "-26a -26a" } }, // ↖
	{ Mod3Mask|ControlMask,         XK_i,            floatpos,               {.v = "  0a -26a" } }, // ↑
	{ Mod3Mask|ControlMask,         XK_o,            floatpos,               {.v = " 26a -26a" } }, // ↗
	{ Mod3Mask|ControlMask,         XK_j,            floatpos,               {.v = "-26a   0a" } }, // ←
	{ Mod3Mask|ControlMask,         XK_l,            floatpos,               {.v = " 26a   0a" } }, // →
	{ Mod3Mask|ControlMask,         XK_m,            floatpos,               {.v = "-26a  26a" } }, // ↙
	{ Mod3Mask|ControlMask,         XK_comma,        floatpos,               {.v = "  0a  26a" } }, // ↓
	{ Mod3Mask|ControlMask,         XK_period,       floatpos,               {.v = " 26a  26a" } }, // ↘
	/* Resize client, client center position is fixed which means that client expands in all directions */
	{ Mod3Mask|ShiftMask,           XK_u,            floatpos,               {.v = "-26w -26h" } }, // ↖
	{ Mod3Mask|ShiftMask,           XK_i,            floatpos,               {.v = "  0w -26h" } }, // ↑
	{ Mod3Mask|ShiftMask,           XK_o,            floatpos,               {.v = " 26w -26h" } }, // ↗
	{ Mod3Mask|ShiftMask,           XK_j,            floatpos,               {.v = "-26w   0h" } }, // ←
	{ Mod3Mask|ShiftMask,           XK_k,            floatpos,               {.v = "800W 800H" } }, // ·
	{ Mod3Mask|ShiftMask,           XK_l,            floatpos,               {.v = " 26w   0h" } }, // →
	{ Mod3Mask|ShiftMask,           XK_m,            floatpos,               {.v = "-26w  26h" } }, // ↙
	{ Mod3Mask|ShiftMask,           XK_comma,        floatpos,               {.v = "  0w  26h" } }, // ↓
	{ Mod3Mask|ShiftMask,           XK_period,       floatpos,               {.v = " 26w  26h" } }, // ↘
	/* Client is positioned in a floating grid, movement is relative to client's current position */
	{ Mod3Mask|Mod1Mask,            XK_u,            floatpos,               {.v = "-1p -1p" } }, // ↖
	{ Mod3Mask|Mod1Mask,            XK_i,            floatpos,               {.v = " 0p -1p" } }, // ↑
	{ Mod3Mask|Mod1Mask,            XK_o,            floatpos,               {.v = " 1p -1p" } }, // ↗
	{ Mod3Mask|Mod1Mask,            XK_j,            floatpos,               {.v = "-1p  0p" } }, // ←
	{ Mod3Mask|Mod1Mask,            XK_k,            floatpos,               {.v = " 0p  0p" } }, // ·
	{ Mod3Mask|Mod1Mask,            XK_l,            floatpos,               {.v = " 1p  0p" } }, // →
	{ Mod3Mask|Mod1Mask,            XK_m,            floatpos,               {.v = "-1p  1p" } }, // ↙
	{ Mod3Mask|Mod1Mask,            XK_comma,        floatpos,               {.v = " 0p  1p" } }, // ↓
	{ Mod3Mask|Mod1Mask,            XK_period,       floatpos,               {.v = " 1p  1p" } }, // ↘
	#endif // FLOATPOS_PATCH
	#if SETBORDERPX_PATCH
	{ MODKEY|ControlMask,           XK_minus,      setborderpx,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_plus,       setborderpx,            {.i = +1 } },
	{ MODKEY|ControlMask,           XK_numbersign, setborderpx,            {.i = 0 } },
	#endif // SETBORDERPX_PATCH
	#if CYCLELAYOUTS_PATCH
	{ MODKEY|ControlMask,           XK_comma,      cyclelayout,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,     cyclelayout,            {.i = +1 } },
	#endif // CYCLELAYOUTS_PATCH
	#if MPDCONTROL_PATCH
	{ MODKEY,                       XK_F1,         mpdchange,              {.i = -1} },
	{ MODKEY,                       XK_F2,         mpdchange,              {.i = +1} },
	{ MODKEY,                       XK_Escape,     mpdcontrol,             {0} },
	#endif // MPDCONTROL_PATCH
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
#if STATUSBUTTON_PATCH
/* click can be ClkButton, ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
#else
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
#endif //
static Button buttons[] = {
	/* click                event mask           button          function        argument */
	#if BAR_STATUSBUTTON_PATCH
	{ ClkButton,            0,                   Button1,        spawn,          {.v = dmenucmd } },
	#endif // BAR_STATUSBUTTON_PATCH
	{ ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
	#if BAR_LAYOUTMENU_PATCH
	{ ClkLtSymbol,          0,                   Button3,        layoutmenu,     {0} },
	#else
	{ ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
	#endif // BAR_LAYOUTMENU_PATCH
	#if BAR_WINTITLEACTIONS_PATCH
	{ ClkWinTitle,          0,                   Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,                   Button3,        showhideclient, {0} },
	#endif // BAR_WINTITLEACTIONS_PATCH
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	#if BAR_STATUSCMD_PATCH && BAR_DWMBLOCKS_PATCH
	{ ClkStatusText,        0,                   Button1,        sigstatusbar,   {.i = 1 } },
	{ ClkStatusText,        0,                   Button2,        sigstatusbar,   {.i = 2 } },
	{ ClkStatusText,        0,                   Button3,        sigstatusbar,   {.i = 3 } },
	#elif BAR_STATUSCMD_PATCH
	{ ClkStatusText,        0,                   Button1,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,                   Button3,        spawn,          {.v = statuscmd } },
	#else
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = termcmd } },
	#endif // BAR_STATUSCMD_PATCH
	#if PLACEMOUSE_PATCH
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,              Button1,        moveorplace,    {.i = 1} },
	#else
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	#endif // PLACEMOUSE_PATCH
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	#if TAPRESIZE_PATCH
	{ ClkClientWin,         MODKEY,              Button4,        resizemousescroll, {.v = &scrollargs[0]} },
	{ ClkClientWin,         MODKEY,              Button5,        resizemousescroll, {.v = &scrollargs[1]} },
	{ ClkClientWin,         MODKEY,              Button6,        resizemousescroll, {.v = &scrollargs[2]} },
	{ ClkClientWin,         MODKEY,              Button7,        resizemousescroll, {.v = &scrollargs[3]} },
	#endif // TAPRESIZE_PATCH
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
	#if TAB_PATCH
	{ ClkTabBar,            0,                   Button1,        focuswin,       {0} },
	#endif // TAB_PATCH
};

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
	{ "toggleview",              toggleview },
	#if SHIFTVIEW_PATCH
	{ "shiftview",               shiftview },
	#endif // SHIFTVIEW_PATCH
	#if SHIFTVIEW_CLIENTS_PATCH
	{ "shiftviewclients",        shiftviewclients },
	#endif // SHIFTVIEW_CLIENTS_PATCH
	#if SELFRESTART_PATCH
	{ "self_restart",            self_restart },
	#endif // SELFRESTART_PATCH
	#if BAR_TAGGRID_PATCH
	{ "switchtag",               switchtag },
	#endif // BAR_TAGGRID_PATCH
	#if STICKY_PATCH
	{ "togglesticky",            togglesticky },
	#endif // STICKY_PATCH
	#if SETBORDERPX_PATCH
	{ "setborderpx",             setborderpx },
	#endif // SETBORDERPX_PATCH
	#if CYCLELAYOUTS_PATCH
	{ "cyclelayout",             cyclelayout },
	#endif // CYCLELAYOUTS_PATCH
	#if MPDCONTROL_PATCH
	{ "mpdchange",               mpdchange },
	{ "mpdcontrol",              mpdcontrol },
	#endif // MPDCONTROL_PATCH
	{ "toggleviewex",            toggleviewex },
	{ "tag",                     tag },
	{ "tagall",                  tagallex },
	{ "tagex",                   tagex },
	{ "toggletag",               toggletag },
	{ "toggletagex",             toggletagex },
	#if TAGALLMON_PATCH
	{ "tagallmon",               tagallmon },
	#endif // TAGALLMON_PATCH
	#if TAGSWAPMON_PATCH
	{ "tagswapmon",              tagswapmon},
	#endif // TAGSWAPMON_PATCH
	#if BAR_ALTERNATIVE_TAGS_PATCH
	{ "togglealttag",            togglealttag },
	#endif // BAR_ALTERNATIVE_TAGS_PATCH
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
	#if XRDB_PATCH && !BAR_VTCOLORS_PATCH
	{ "xrdb",                    xrdb },
	#endif // XRDB_PATCH
	#if TAGOTHERMONITOR_PATCH
	{ "tagnextmonex",            tagnextmonex },
	{ "tagprevmonex",            tagprevmonex },
	#endif // TAGOTHERMONITOR_PATCH
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

#if IPC_PATCH
static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
	IPCCOMMAND( focusmon, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( focusstack, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incnmaster, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( killclient, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( quit, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( setlayoutsafe, 1, {ARG_TYPE_PTR} ),
	IPCCOMMAND( setmfact, 1, {ARG_TYPE_FLOAT} ),
	IPCCOMMAND( setstatus, 1, {ARG_TYPE_STR} ),
	IPCCOMMAND( tag, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( tagmon, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( togglebar, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( togglefloating, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( toggletag, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( toggleview, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( view, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( zoom, 1, {ARG_TYPE_NONE} ),
	#if BAR_ALTERNATIVE_TAGS_PATCH
	IPCCOMMAND( togglealttag, 1, {ARG_TYPE_NONE} ),
	#endif // BAR_ALTERNATIVE_TAGS_PATCH
	#if BAR_TAGGRID_PATCH
	IPCCOMMAND( switchtag, 1, {ARG_TYPE_UINT} ),
	#endif // BAR_TAGGRID_PATCH
	#if CFACTS_PATCH
	IPCCOMMAND( setcfact, 1, {ARG_TYPE_FLOAT} ),
	#endif // CFACTS_PATCH
	#if CYCLELAYOUTS_PATCH
	IPCCOMMAND( cyclelayout, 1, {ARG_TYPE_SINT} ),
	#endif // CYCLELAYOUTS_PATCH
	#if EXRESIZE_PATCH
	IPCCOMMAND( explace, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( togglehorizontalexpand, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( toggleverticalexpand, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( togglemaximize, 1, {ARG_TYPE_SINT} ),
	#endif // EXRESIZE_PATCH
	#if !FAKEFULLSCREEN_PATCH && FAKEFULLSCREEN_CLIENT_PATCH
	IPCCOMMAND( togglefakefullscreen, 1, {ARG_TYPE_NONE} ),
	#endif // FAKEFULLSCREEN_CLIENT_PATCH
	#if FLOATPOS_PATCH
	IPCCOMMAND( floatpos, 1, {ARG_TYPE_STR} ),
	#endif // FLOATPOS_PATCH
	#if FULLSCREEN_PATCH
	IPCCOMMAND( fullscreen, 1, {ARG_TYPE_NONE} ),
	#endif // FULLSCREEN_PATCH
	#if FLEXTILE_DELUXE_LAYOUT
	IPCCOMMAND( incnstack, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( rotatelayoutaxis, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( setlayoutaxisex, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( mirrorlayout, 1, {ARG_TYPE_NONE} ),
	#endif // FLEXTILE_DELUXE_LAYOUT
	#if FOCUSURGENT_PATCH
	IPCCOMMAND( focusurgent, 1, {ARG_TYPE_NONE} ),
	#endif // FOCUSURGENT_PATCH
	#if FOCUSADJACENTTAG_PATCH
	IPCCOMMAND( viewtoleft, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( viewtoright, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( tagtoleft, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( tagtoright, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( tagandviewtoleft, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( tagandviewtoright, 1, {ARG_TYPE_NONE} ),
	#endif // FOCUSADJACENTTAG_PATCH
	#if INPLACEROTATE_PATCH
	IPCCOMMAND( inplacerotate, 1, {ARG_TYPE_SINT} ),
	#endif // INPLACEROTATE_PATCH
	#if KEYMODES_PATCH
	IPCCOMMAND( setkeymode, 1, {ARG_TYPE_UINT} ),
	#endif // KEYMODES_PATCH
	#if MAXIMIZE_PATCH
	IPCCOMMAND( togglehorizontalmax, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( toggleverticalmax, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( togglemax, 1, {ARG_TYPE_NONE} ),
	#endif // MAXIMIZE_PATCH
	#if MPDCONTROL_PATCH
	IPCCOMMAND( mpdchange, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( mpdcontrol, 1, {ARG_TYPE_NONE} ),
	#endif // MPDCONTROL_PATCH
	#if MOVEPLACE_PATCH
	IPCCOMMAND( moveplace, 1, {ARG_TYPE_UINT} ),
	#endif // MOVEPLACE_PATCH
	#if MOVERESIZE_PATCH
	IPCCOMMAND( moveresize, 1, {ARG_TYPE_STR} ),
	#endif // MOVERESIZE_PATCH
	#if RIODRAW_PATCH
	IPCCOMMAND( rioresize, 1, {ARG_TYPE_NONE} ),
	#endif // RIODRAW_PATCH
	#if PUSH_PATCH || PUSH_NO_MASTER_PATCH
	IPCCOMMAND( pushdown, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( pushup, 1, {ARG_TYPE_NONE} ),
	#endif // PUSH_PATCH / PUSH_NO_MASTER_PATCH
	#if ROTATESTACK_PATCH
	IPCCOMMAND( rotatestack, 1, {ARG_TYPE_SINT} ),
	#endif // ROTATESTACK_PATCH
	#if SCRATCHPADS_PATCH
	IPCCOMMAND( togglescratch, 1, {ARG_TYPE_UINT} ),
	#endif // SCRATCHPADS_PATCH
	#if SELFRESTART_PATCH
	IPCCOMMAND( self_restart, 1, {ARG_TYPE_NONE} ),
	#endif // SELFRESTART_PATCH
	#if SETBORDERPX_PATCH
	IPCCOMMAND( setborderpx, 1, {ARG_TYPE_SINT} ),
	#endif // SETBORDERPX_PATCH
	#if SHIFTVIEW_PATCH
	IPCCOMMAND( shiftview, 1, {ARG_TYPE_SINT} ),
	#endif // SHIFTVIEW_PATCH
	#if SHIFTVIEW_CLIENTS_PATCH
	IPCCOMMAND( shiftviewclients, 1, {ARG_TYPE_SINT} ),
	#endif // SHIFTVIEW_CLIENTS_PATCH
	#if STACKER_PATCH
	IPCCOMMAND( pushstack, 1, {ARG_TYPE_SINT} ),
	#endif // STACKER_PATCH
	#if STICKY_PATCH
	IPCCOMMAND( togglesticky, 1, {ARG_TYPE_NONE} ),
	#endif // STICKY_PATCH
	#if SWAPFOCUS_PATCH && PERTAG_PATCH
	IPCCOMMAND( swapfocus, 1, {ARG_TYPE_SINT} ),
	#endif // SWAPFOCUS_PATCH
	#if SWITCHCOL_PATCH
	IPCCOMMAND( switchcol, 1, {ARG_TYPE_NONE} ),
	#endif // SWITCHCOL_PATCH
	#if TAGALLMON_PATCH
	IPCCOMMAND( tagallmon, 1, {ARG_TYPE_SINT} ),
	#endif // TAGALLMON_PATCH
	#if TAGOTHERMONITOR_PATCH
	IPCCOMMAND( tagnextmonex, 1, {ARG_TYPE_UINT} ),
	IPCCOMMAND( tagprevmonex, 1, {ARG_TYPE_UINT} ),
	#endif // TAGOTHERMONITOR_PATCH
	#if TAGSWAPMON_PATCH
	IPCCOMMAND( tagswapmon, 1, {ARG_TYPE_SINT} ),
	#endif // TAGSWAPMON_PATCH
	#if TOGGLEFULLSCREEN_PATCH
	IPCCOMMAND( togglefullscreen, 1, {ARG_TYPE_NONE} ),
	#endif // TOGGLEFULLSCREEN_PATCH
	#if TRANSFER_PATCH
	IPCCOMMAND( transfer, 1, {ARG_TYPE_NONE} ),
	#endif // TRANSFER_PATCH
	#if TRANSFER_ALL_PATCH
	IPCCOMMAND( transferall, 1, {ARG_TYPE_NONE} ),
	#endif // TRANSFER_ALL_PATCH
	#if UNFLOATVISIBLE_PATCH
	IPCCOMMAND( unfloatvisible, 1, {ARG_TYPE_NONE} ),
	#endif // UNFLOATVISIBLE_PATCH
	#if VANITYGAPS_PATCH
	IPCCOMMAND( incrgaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incrigaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incrogaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incrihgaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incrivgaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incrohgaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( incrovgaps, 1, {ARG_TYPE_SINT} ),
	IPCCOMMAND( togglegaps, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( defaultgaps, 1, {ARG_TYPE_NONE} ),
	IPCCOMMAND( setgapsex, 1, {ARG_TYPE_SINT} ),
	#endif // VANITYGAPS_PATCH
	#if WINVIEW_PATCH
	IPCCOMMAND( winview, 1, {ARG_TYPE_NONE} ),
	#endif // WINVIEW_PATCH
	#if XRDB_PATCH && !BAR_VTCOLORS_PATCH
	IPCCOMMAND( xrdb, 1, {ARG_TYPE_NONE} ),
	#endif // XRDB_PATCH
};
#endif // IPC_PATCH

