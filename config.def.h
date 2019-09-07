/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
#if STATUSPADDING_PATCH
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 0;        /* vertical padding for statusbar */
#endif // STATUSPADDING_PATCH
#if SYSTRAY_PATCH
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
#endif // SYSTRAY_PATCH
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
#if ALPHA_PATCH
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};
#endif // ALPHA_PATCH
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	WM_WINDOW_ROLE(STRING) = role
	 */
	#if WINDOWROLERULE_PATCH && SWITCHTAG_PATCH && CENTER_PATCH
	/* class      role        instance    title         tags mask    switchtag    iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       NULL,         0,           1,           0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       NULL,         1 << 8,      1,           0,           0,           -1 },
	#elif WINDOWROLERULE_PATCH && !SWITCHTAG_PATCH && CENTER_PATCH
	/* class      role        instance    title         tags mask    iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       NULL,         0,           0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       NULL,         1 << 8,      0,           0,           -1 },
	#elif WINDOWROLERULE_PATCH && SWITCHTAG_PATCH && !CENTER_PATCH
	/* class      role        instance    title         tags mask    switchtag    isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       NULL,         0,           1,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       NULL,         1 << 8,      1,           0,           -1 },
	#elif !WINDOWROLERULE_PATCH && SWITCHTAG_PATCH && CENTER_PATCH
	/* class      instance    title       tags mask     switchtag    iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       1,           0,           0,           -1 },
	#elif WINDOWROLERULE_PATCH && !SWITCHTAG_PATCH && !CENTER_PATCH
	/* class      role        instance    title         tags mask    isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       NULL,         0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       NULL,         1 << 8,      0,           -1 },
	#elif !WINDOWROLERULE_PATCH && SWITCHTAG_PATCH && !CENTER_PATCH
	/* class      instance    title       tags mask     switchtag    isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       1,           0,           -1 },
	#elif !WINDOWROLERULE_PATCH && !SWITCHTAG_PATCH && CENTER_PATCH
	/* class      instance    title       tags mask     iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           0,           -1 },
	#else
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	#endif // granted the above will be confusing, do remember to delete rule entries for patches that you do not take
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	#if CYCLELAYOUTS_PATCH
	{ NULL,       NULL },
	#endif // CYCLELAYOUTS_PATCH
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	#if ROTATESTACK_PATCH
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	#endif // ROTATESTACK_PATCH
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	#if TOGGLEFULLSCREEN_PATCH
	{ MODKEY,                       XK_y,      togglefullscreen, {0} },
	#endif // TOGGLEFULLSCREEN_PATCH
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	#if TAGALLMON_PATCH
	{ MODKEY|Mod4Mask|ShiftMask,    XK_comma,  tagallmon,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_period, tagallmon,      {.i = -1 } },
	#endif // TAGALLMON_PATCH
	#if TAGSWAPMON_PATCH
	{ MODKEY|Mod4Mask|ControlMask,  XK_comma,  tagswapmon,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_period, tagswapmon,     {.i = -1 } },
	#endif // TAGSWAPMON_PATCH
	#if CYCLELAYOUTS_PATCH
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	#endif // CYCLELAYOUTS_PATCH
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
