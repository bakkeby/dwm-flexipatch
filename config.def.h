/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
#if VANITYGAPS_PATCH
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
#endif // VANITYGAPS_PATCH
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
static const unsigned int alphas[][3] = {
	/*                fg      bg        border     */
	[SchemeNorm]  = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]   = { OPAQUE, baralpha, borderalpha },
	#if AWESOMEBA R_PATCH
	[SchemeHid]   = { OPAQUE, baralpha, borderalpha },
	#endif // AWESOMEBAR_PATCH
	#if TITLECOLOR_PATCH
	[SchemeTitle] = { OPAQUE, baralpha, borderalpha },
	#endif // TITLECOLOR_PATCH
};
#endif // ALPHA_PATCH
#if FLOAT_BORDER_COLOR_PATCH
static const char *colors[][4] = {
	/*                fg         bg         border     float     */
	[SchemeNorm]  = { col_gray3, col_gray1, col_gray2, col_gray2 },
	[SchemeSel]   = { col_gray4, col_cyan,  col_cyan,  col_cyan  },
	#if AWESOMEBAR_PATCH
	[SchemeHid]   = { col_cyan,  col_gray1, col_cyan,  col_cyan  },
	#endif // AWESOMEBAR_PATCH
	#if TITLECOLOR_PATCH
	[SchemeTitle] = { col_gray4, col_cyan,  col_cyan,  col_cyan  },
	#endif // TITLECOLOR_PATCH
};
#else
static const char *colors[][3] = {
	/*                fg         bg         border   */
	[SchemeNorm]  = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]   = { col_gray4, col_cyan,  col_cyan  },
	#if AWESOMEBAR_PATCH
	[SchemeHid]   = { col_cyan,  col_gray1, col_cyan  },
	#endif // AWESOMEBAR_PATCH
	#if TITLECOLOR_PATCH
	[SchemeTitle] = { col_gray4, col_cyan,  col_cyan  },
	#endif // TITLECOLOR_PATCH
};
#endif // FLOAT_BORDER_COLOR_PATCH

/* tagging */
#if EWMHTAGS_PATCH
static char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#else
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif // EWMHTAGS_PATCH
#if ALTERNATIVE_TAGS_PATCH
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif // ALTERNATIVE_TAGS_PATCH

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

#if MONITOR_RULES_PATCH
static const MonitorRule monrules[] = {
	#if FLEXTILE_LAYOUT
	/* monitor layout axis              master            stack             */
	{  1,      0,     SPLIT_HORIZONTAL, LEFT_TO_RIGHT,    GRID,             }, // use a different layout for the second monitor
	{  -1,     0,     SPLIT_VERTICAL,   TOP_TO_BOTTOM,    TOP_TO_BOTTOM,    }, // default
	#else
	/* monitor layout */
	{  1,      2 }, // use a different layout for the second monitor
	{  -1,     0 }, // default
	#endif // FLEXTILE_LAYOUT
};
#elif FLEXTILE_LAYOUT
static const int layoutaxis[] = {
	SPLIT_VERTICAL,   /* layout axis: 1 = x, 2 = y; negative values mirror the layout, setting the master area to the right / bottom instead of left / top */
	TOP_TO_BOTTOM,    /* master axis: 1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle), 4 = grid */
	TOP_TO_BOTTOM,    /* stack axis:  1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle), 4 = grid */
};
#endif // MONITOR_RULES_PATCH

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#if NROWGRID_LAYOUT
#define FORCE_VSPLIT 1
#endif

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
	#if DECK_LAYOUT
	{ "[D]",      deck },
	#endif
	#if FIBONACCI_SPIRAL_LAYOUT
	{ "(@)",      spiral },
	#endif
	#if FIBONACCI_DWINDLE_LAYOUT
	{ "[\\]",     dwindle },
	#endif
	#if FLEXTILE_LAYOUT
	{ "[]=",	flextile },
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

/* key definitions */
#define MODKEY Mod1Mask
#if COMBO_PATCH
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#else
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#endif // COMBO_PATCH

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function           argument */
	{ MODKEY,                       XK_p,      spawn,             {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,             {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,         {0} },
	{ MODKEY,                       XK_j,      focusstack,        {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,        {.i = -1 } },
	#if ROTATESTACK_PATCH
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,       {.i = -1 } },
	#endif // ROTATESTACK_PATCH
	#if PUSH_PATCH
	{ MODKEY|ControlMask,           XK_j,      pushdown,          {0} },
	{ MODKEY|ControlMask,           XK_k,      pushup,            {0} },
	#endif // PUSH_PATCH
	{ MODKEY,                       XK_i,      incnmaster,        {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,        {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,          {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,          {.f = +0.05} },
	#if CFACTS_PATCH
	{ MODKEY|ShiftMask,             XK_h,      setcfact,          {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,          {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,          {.f =  0.00} },
	#endif // CFACTS_PATCH
	{ MODKEY,                       XK_Return, zoom,              {0} },
	#if VANITYGAPS_PATCH
	{ MODKEY|Mod4Mask,              XK_u,      incrgaps,          {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,          {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,         {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,         {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,         {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,         {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,        {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,        {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,        {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,        {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,        {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,        {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,        {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,        {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,      togglegaps,        {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,       {0} },
	#endif // VANITYGAPS_PATCH
	{ MODKEY,                       XK_Tab,    view,              {0} },
	#if AWESOMEBAR_PATCH
	{ MODKEY,                       XK_z,      showhideclient,    {0} },
	#endif // AWESOMEBAR_PATCH
	{ MODKEY|ShiftMask,             XK_c,      killclient,        {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,              {0} },
	{ MODKEY,                       XK_t,      setlayout,         {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,         {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,         {.v = &layouts[2]} },
	#if FLEXTILE_LAYOUT
	{ MODKEY,                       XK_w,      setflexlayout,     {.i = 293 } }, // centered master
	{ MODKEY,                       XK_e,      setflexlayout,     {.i = 273 } }, // bstackhoriz layout
	{ MODKEY,                       XK_r,      setflexlayout,     {.i = 272 } }, // bstack layout
	{ MODKEY,                       XK_v,      setflexlayout,     {.i = 261 } }, // default tile layout
	{ MODKEY,                       XK_g,      setflexlayout,     {.i = 263 } }, // tile + grid layout
	{ MODKEY|ControlMask,           XK_w,      setflexlayout,     {.i =   7 } }, // grid
	{ MODKEY|ControlMask,           XK_e,      setflexlayout,     {.i = 262 } }, // deck layout
	{ MODKEY|ControlMask,           XK_r,      setflexlayout,     {.i =   6 } }, // monocle
	{ MODKEY|ControlMask,           XK_g,      setflexlayout,     {.i = 257 } }, // columns (col) layout
	{ MODKEY|ControlMask,           XK_t,      rotatelayoutaxis,  {.i = 0 } },   /* flextile, 0 = layout axis */
	{ MODKEY|ControlMask,           XK_Tab,    rotatelayoutaxis,  {.i = 1 } },   /* flextile, 1 = master axis */
	{ MODKEY|ControlMask|ShiftMask, XK_Tab,    rotatelayoutaxis,  {.i = 2 } },   /* flextile, 2 = stack axis */
	{ MODKEY|ControlMask,           XK_Return, mirrorlayout,      {0} },         /* flextile, flip master and stack areas */
	#endif // FLEXTILE_LAYOUT
	{ MODKEY,                       XK_space,  setlayout,         {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,    {0} },
	#if TOGGLEFULLSCREEN_PATCH
	{ MODKEY,                       XK_y,      togglefullscreen,  {0} },
	#endif // TOGGLEFULLSCREEN_PATCH
	{ MODKEY,                       XK_0,      view,              {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,               {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,          {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,          {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,            {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,            {.i = +1 } },
	#if TAGALLMON_PATCH
	{ MODKEY|Mod4Mask|ShiftMask,    XK_comma,  tagallmon,         {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_period, tagallmon,         {.i = -1 } },
	#endif // TAGALLMON_PATCH
	#if TAGSWAPMON_PATCH
	{ MODKEY|Mod4Mask|ControlMask,  XK_comma,  tagswapmon,        {.i = +1 } },
	{ MODKEY|Mod4Mask|ControlMask,  XK_period, tagswapmon,        {.i = -1 } },
	#endif // TAGSWAPMON_PATCH
	#if ALTERNATIVE_TAGS_PATCH
	{ MODKEY,                       XK_n,      togglealttag,      {0} },
	#endif // ALTERNATIVE_TAGS_PATCH
	#if CYCLELAYOUTS_PATCH
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,       {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,       {.i = +1 } },
	#endif // CYCLELAYOUTS_PATCH
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	#if AWESOMEBAR_PATCH
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button3,        showhideclient, {0} },
	#endif // AWESOMEBAR_PATCH
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
