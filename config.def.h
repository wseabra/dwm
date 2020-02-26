    /* See LICENSE file for copyright and license details. */
/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include <X11/XF86keysym.h>
#include "gaplessgrid.c"
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 4;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "RobotoMono Nerd Font:style=Bold,Regular:size=10" };
static const char dmenufont[]       = "RobotoMono Nerd Font:style=Bold:size=10";
static const char col_gray1[]       = "#040c1a";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#503645";
static const char col_border_sel[]  = "#bf8a91";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border_sel  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance    title       tags mask     isfloating   monitor */
	{ "Gimp",      NULL,       NULL,       0,            1,           -1 },
	{ "firefox",   NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Telegram",  NULL,       NULL,       1 << 6,       1,           -1 },
	{ "discord",   NULL,       NULL,       1 << 6,       1,           -1 },
	{ "Spotify",   NULL,       NULL,       1 << 4,       1,           -1 },
	{ "spotify",   NULL,       NULL,       1 << 4,       1,           -1 },
	{ "Steam",     NULL,       NULL,       1 << 3,       1,           -1 },
	{ "vlc",       NULL,       NULL,       1 << 5,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
    { "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
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
static const char *firefoxcmd[] = {"firefox", NULL};
static const char *thunarcmd[] = {"thunar", NULL};
static const char *brightnessUpcmd[] = {"xbacklight","-inc","10", NULL};
static const char *brightnessDowncmd[] = {"xbacklight","-dec","10", NULL};
static const char *volumeUpcmd[] = {"amixer","-q","set","\'Master\'","1%+", NULL};
static const char *volumeDowncmd[] = {"amixer","-q","set","\'Master\'","1%-", NULL};
static const char *volumemutecmd[] = {"amixer","-q","set","\'Master\'","toggle", NULL};
static const char *audioplaycmd[] = {"playerctl","play-pause", NULL};
static const char *audionextcmd[] = {"playerctl","next", NULL};
static const char *audioprevcmd[] = {"playerctl","previous", NULL};
static const char *restartpicomcmd[] = {"restartpicom", NULL};
static const char *lockscreen[] = {"light-locker-command", "-l", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
    { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
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
    { MODKEY,                       XK_b,      spawn,          {.v = firefoxcmd} },
    { MODKEY,                       XK_t,      spawn,          {.v = thunarcmd} },
    { 0,        XF86XK_MonBrightnessDown,      spawn,          {.v = brightnessDowncmd} },
    { 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = brightnessUpcmd} },
    { 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = volumeUpcmd} },
    { 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = volumeDowncmd} },
    { 0,                XF86XK_AudioMute,      spawn,          {.v = volumemutecmd} },
    { 0,                XF86XK_AudioPlay,      spawn,          {.v = audioplaycmd} },
    { 0,                XF86XK_AudioNext,      spawn,          {.v = audionextcmd} },
    { 0,                XF86XK_AudioPrev,      spawn,          {.v = audioprevcmd} },
    { 0,                        XK_Print,      spawn,          SHCMD("scrot ~/Pictures/Screenshots/screenshot-$(date '+%Y%m%d_%H%M%S').png") },
    { 0|ControlMask,            XK_Print,      spawn,          SHCMD("scrot -u ~/Pictures/Screenshots/screenshot-$(date '+%Y%m%d_%H%M%S').png") },
    { 0|ShiftMask,              XK_Print,      spawn,          SHCMD("sleep 0.2; scrot -s ~/Pictures/Screenshots/screenshot-$(date '+%Y%m%d_%H%M%S').png") },
    { MODKEY,                       XK_p,      spawn,          {.v = restartpicomcmd} },
    { ALTKEY|ControlMask,           XK_l,      spawn,          {.v = lockscreen} },
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

