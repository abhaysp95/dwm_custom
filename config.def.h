/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */

static unsigned int borderpx		= 2;        /* border pixel of windows */
static unsigned int snap            	= 32;       /* snap pixel */
static int swallowfloating		= 0;        /* 1 means swallow floating windows by default */
static unsigned int gappih    		= 20;       /* horiz inner gap between windows */
static unsigned int gappiv    		= 10;       /* vert inner gap between windows */
static unsigned int gappoh    		= 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    		= 30;       /* vert outer gap between windows and screen edge */
static int smartgaps          		= 0;        /* 1 means no outer gap when there is only one window */
static int showbar                  	= 1;        /* 0 means no bar */
static int topbar                   	= 1;        /* 0 means bottom bar */
static const char *fonts[]          	= { "Source Code Pro:size=10" };
static const char dmenufont[]       	= "monospace:size=10";
static char normbgcolor[]           	= "#222222";
static char normbordercolor[]       	= "#444444";
static char normfgcolor[]           	= "#bbbbbb";
static char selfgcolor[]            	= "#eeeeee";
static char selbordercolor[]        	= "#770000";
static char selbgcolor[]            	= "#005577";
static const unsigned int baralpha 	= 0xe9;
static const unsigned int borderalpha 	= OPAQUE;
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"


static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    		/* first entry is default, Master on left, slaves on right */
	{ "TTT",      bstack },			/* Master on top, slaves on bottom */
	{ "[@]",      spiral },			/* Fibonacci Sprial */
	{ "[\\]",     dwindle },		/* Decreasing in size right and leftward */
	{ "H[]",      deck },			/* Master on left, slaves in monocle-like mode oright */
	{ "[M]",      monocle },		/* All windows on top of each other */
	{ "|M|",      centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",      centeredfloatingmaster },	/* same as above, but master floates */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      toggleview,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      view,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *dmenucmd[] = { "dmenu_run", "-i", "-p", "exec:", "-c", "-l", "25", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/* User defined commnads */ 
static const char* sectermcmd[] = { "termite", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0", 		STRING,	&normbordercolor },
		{ "color8",		STRING, &selbordercolor },
		{ "color0",        	STRING,  &normbgcolor },
		{ "color4",        	STRING,  &normfgcolor },
		{ "color4",         	STRING,  &selbgcolor },
		{ "color0",         	STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	FLOAT,   &mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};


#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */

	/** Stack Keys */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)

	/** Tag switching keys */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	/** view and tag */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	/** spawn commands */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },

	/** scratch */
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },

	/** volume */
	{ MODKEY,			XK_minus,	spawn,		SHCMD("notifyChanges.sh --volume --up 5") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("notifyChanges.sh --volume --up 15") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("notifyChanges.sh --volume --down 5") },
	{ MODKEY|ShiftMask,		XK_equal,	spawn,		SHCMD("notifyChanges.sh --volume --down 15") },

	/** music related(home row) */
	{ MODKEY,			XK_p,			spawn,		SHCMD("music.sh") },
	{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("music_seek_next1.sh") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("music_seek_next2.sh") },
	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("music_seek_prev1.sh") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("music_seek_prev1.sh") },

	/** move up/down in stack */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },

	/** increase/decrease numbers of stack of master */
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },

	/** resize nodes left/right */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },

	/** rotate between tags in use */
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_backslash,    view,           {0} },

	/** make node swap with master and vice-versa */
	{ MODKEY,                       XK_Return, zoom,           {0} },

	/** switch layouts */
	{ MODKEY,			XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			XK_u,		setlayout,	{.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			XK_i,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_i,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY|ShiftMask,		XK_f,		setlayout,	{.v = &layouts[8]} }, /* fullscreen */

	/** toggle things */
	{ MODKEY,			XK_a,		togglegaps,	{0} },  // toggle gaps
	{ MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} },  // switch to default gaps
	{ MODKEY,			XK_s,		togglesticky,	{0} },  // toggle sticky mode
	{ MODKEY,                       XK_b,      	togglebar,      {0} },  // toggle dwm status bar
	{ MODKEY,			XK_f,		togglefullscr,	{0} },  // toggle fullscreen


	{ MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_w,      quit,           {0} },
	{ MODKEY|ControlMask,			XK_c,	   spawn,	   {.v = sectermcmd} },
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
	{ ClkTagBar,            0,              Button1,        toggleview,           {0} },
	{ ClkTagBar,            0,              Button3,        view,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

