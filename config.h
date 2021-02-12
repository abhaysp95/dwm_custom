/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "termite"
#define TERMCLASS "Termite"

/* appearance */

static unsigned int borderpx		= 3;        /* border pixel of windows */
static unsigned int snap            	= 32;       /* snap pixel */
static int swallowfloating		= 0;        /* 1 means swallow floating windows by default */
static unsigned int gappih    		= 10;       /* horiz inner gap between windows */
static unsigned int gappiv    		= 10;       /* vert inner gap between windows */
static unsigned int gappoh    		= 8;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    		= 8;       /* vert outer gap between windows and screen edge */
static int smartgaps          		= 1;        /* 1 means no outer gap when there is only one window */
static int showbar                  	= 1;        /* 0 means no bar */
static int topbar                   	= 1;        /* 0 means bottom bar */
static const char *fonts[]          	= { "Fantasque Sans Mono:style=Regular:size=12" };
static const char dmenufont[]       	= "monospace:size=10";
static char normbgcolor[]           	= "#222222";
static char normbordercolor[]       	= "#444444";
static char normfgcolor[]           	= "#bbbbbb";
static char selfgcolor[]            	= "#eeeeee";
static char selbordercolor[]        	= "#770000";
static char selbgcolor[]            	= "#005577";
static const unsigned int baralpha 	= 0xff;
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

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
/** this will work for st */
//const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
//const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
/** this is for termite */
const char *spcmd1[] = {TERMINAL, "--name", "spterm", NULL};
const char *spcmd2[] = {TERMINAL, "--name", "spcalc", "-e", "'bc", "-lq'", NULL};
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "LibreWolf",    NULL,       NULL,       	    1 << 1,       0,           0,         0,        -1 },
	{ "TelegramDesktop",    NULL,       NULL,       	    1 << 6,       0,           0,         0,        -1 },
	{ "Lxappearance",    NULL,       NULL,       	    1 << 6,       0,           0,         0,        -1 },
	{ "Zathura",    NULL,       NULL,       	    1 << 8,       1,           0,         0,        -1 },
	{ "mpv",    NULL,       NULL,       	    1 << 3,       1,           0,         0,        -1 },
	{ TERMCLASS,   NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
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
#define MODKEYALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
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
static const char *dmenucmd[] = { "dmenu_run", "-i", "-p", "exec:", "-c", "-l", "25", NULL };
static const char *termcmd[]  = { "kitty" , NULL };

/* User defined commnads */
static const char* sectermcmd[] = { "termite", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0", 		STRING,	&normbordercolor },
		{ "color4",		STRING, &selbordercolor },
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


#include <X11/XF86keysym.h>  /* for extra keys */
#include "shiftview.c"


#include "movestack.c"
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
	{ MODKEY,		        XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,	        XK_Return, spawn,          {.v = sectermcmd } },

	/** volume */
	{ MODKEY,			XK_minus,	spawn,		SHCMD("notifyChanges.sh --volume --down 5") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("notifyChanges.sh --volume --down 15") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("notifyChanges.sh --volume --up 5") },
	{ MODKEY|ShiftMask,		XK_equal,	spawn,		SHCMD("notifyChanges.sh --volume --up 15") },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD("pulsemixer --toggle-mute; notify-send -i ~/.cache/notify-icons/mute.png -t 3000 'Mute Toggled'") },

	/** music related(home row) */
	{ MODKEY,			XK_p,			spawn,		SHCMD("music.sh") },
	{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("music_seek_prev1.sh") },
	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("music_seek_prev2.sh") },
	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("music_seek_next1.sh") },
	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("music_seek_next2.sh") },
	{ MODKEY,			XK_comma,		spawn,		SHCMD("music_prev.sh") },
	{ MODKEY|ShiftMask,		XK_comma,		spawn,		SHCMD("mpc seek 0%") },
	{ MODKEY,			XK_period,		spawn,		SHCMD("music_next.sh") },
	{ MODKEY|ShiftMask,		XK_period,		spawn,		SHCMD("mpc repeat") },

	/** brightness */
	{ MODKEY|ControlMask,		XK_minus,		spawn,		SHCMD("notifyChanges.sh --brightness --down 5") },
	{ MODKEY|ControlMask,		XK_equal,		spawn,		SHCMD("notifyChanges.sh --brightness --up 5") },

	/** call some other custom scripts */
	{ MODKEY|ControlMask,		XK_p,			spawn,		SHCMD("libdmenu pdfs") },
	{ MODKEY|ControlMask,		XK_v,			spawn,		SHCMD("libdmenu videos") },
	{ MODKEY|ControlMask,		XK_m,			spawn,		SHCMD("libdmenu man") },
	{ MODKEY|ControlMask,		XK_y,			spawn,		SHCMD("ympv.sh") },

	/** launch programs */
	{ MODKEY|ControlMask,		XK_b,			spawn,		SHCMD("librewolf") },
	{ MODKEY|ControlMask,		XK_t,			spawn,		SHCMD("telegram-desktop") },

	/** toggle screenkey */
	{ MODKEY,			XK_Scroll_Lock,		spawn,		SHCMD("killall screenkey || screenkey -s small --font-size small -g 400x750+900-0 &") },

	/** screenshot and screen recording */
	{ MODKEY,			XK_Print,		spawn,		SHCMD("sshots --full") },
	{ MODKEYALT|ControlMask,	XK_Print,		spawn,		SHCMD("sshots --window") },
	{ MODKEYALT|ShiftMask,		XK_Print,		spawn,		SHCMD("sshots --specific") },
	{ MODKEY|ShiftMask,		XK_Print,		spawn,		SHCMD("recscreen") },
	{ MODKEY|ControlMask,		XK_Print,		spawn,		SHCMD("recscreen --only-video") },
	{ MODKEY|MODKEYALT,		XK_Print,		spawn,		SHCMD("recscreen --only-audio") },

	/** move nodes up/down in stack */
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

	/** increase/decrease numbers of stack of master */
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incnmaster,     {.i = -1 } },

	/** resize nodes left/right */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_i,      setmfact,       {.f  =  0.0} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },

	/** rotate between tags in use */
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_backslash,    view,           {0} },

	/** make node swap with master and vice-versa */
	{ MODKEY|ControlMask,                       XK_Return, zoom,           {0} },

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
	{ MODKEY,                       XK_space,  	setlayout,      {0} },  /* something related to floating */

	/** toggle things */
	{ MODKEY,			XK_a,		togglegaps,	{0} },  // toggle gaps
	{ MODKEY,			XK_s,		togglesticky,	{0} },  // toggle sticky mode
	{ MODKEY,                       XK_b,      	togglebar,      {0} },  // toggle dwm status bar
	{ MODKEY,			XK_f,		togglefullscr,	{0} },  // toggle fullscreen
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },  // toggle floating mode
	{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 0} },
	{ MODKEY|ShiftMask,		XK_apostrophe,	togglescratch,	{.ui = 1} },

	/** movement within(for) tags*/
	{ MODKEY,			XK_g,		shiftview,	{ .i = -1 } },  // go to left tag
	{ MODKEY|ShiftMask,		XK_g,		shifttag,	{ .i = -1 } },  // move window to left tag
	{ MODKEY,			XK_semicolon,	shiftview,	{ .i = 1 } },	// go to right tag
	{ MODKEY|ShiftMask,		XK_semicolon,	shifttag,	{ .i = 1 } },	// move window to right tag

	/** manage gaps */
	{ MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} },  // switch to default gaps
	{ MODKEY|Mod4Mask,              XK_z,      	incrgaps,       {.i = +2 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_z,      	incrgaps,       {.i = -2 } },
	{ MODKEY|Mod4Mask,              XK_x,      	incrigaps,      {.i = +2 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_x,      	incrigaps,      {.i = -2 } },
	{ MODKEY|Mod4Mask,              XK_c,      	incrogaps,      {.i = +2 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_c,      	incrogaps,      {.i = -2 } },

	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

	/** not sure yet */
	{ MODKEY,                       XK_Left,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_Right, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Right, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_w,      quit,           {0} },

	{ MODKEY,			XK_Page_Up,	shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_Page_Up,	shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,	shiftview,	{ .i = +1 } },
	{ MODKEY|ShiftMask,		XK_Page_Down,	shifttag,	{ .i = +1 } },


	/** { MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } }, */
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
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        toggleview,           {0} },
	{ ClkTagBar,            0,              Button3,        view,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

