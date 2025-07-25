/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Outfit:size=12",
                                        "JetBrainsMono Nerd Font:size=12" };
static const char dmenufont[]       = "Outfit:size=16";
static const char col_gray1[]       = "#1d2021";
static const char col_gray2[]       = "#3c3836";
static const char col_gray3[]       = "#a89984";
static const char col_white[]       = "#ebdbb2";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_white, col_gray2, col_gray3 },
};

/* tagging */
static const char *tags[] = { "   1", "   2", "   3", "   4", "    5", "   6", "  7" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance         title       tags mask     isfloating   monitor */
	{ "st-256color", "zsh-fastfetch", NULL,       0,            1,           -1 },
	{ "st-256color", "zsh-qalq",      NULL,       0,            1,           -1 },
	{ "thunderbird", NULL,            NULL,       1 << 1,       0,           -1 },
	{ "ncmpcpp-7",   NULL,            NULL,       1 << 6,       0,           -1 },
	{ "mpv",         "mpv-7",         NULL,       1 << 6,       0,           -1 },
	{ "yt-7",        NULL,            NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󱂫 ",      tile },    /* first entry is default */
	{ " ",      NULL },    /* no layout function means floating behavior */
	{ "󱣴 ",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]  = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]   = { "st", NULL };

static const char *upvol[]     = { "/bin/sh", "-c", "/usr/bin/wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+ && ~/src/scripts/statusbar.sh", NULL };
static const char *downvol[]   = { "/bin/sh", "-c", "/usr/bin/wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- && ~/src/scripts/statusbar.sh", NULL };
static const char *mutevol[]   = { "/bin/sh", "-c", "/usr/bin/wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle && ~/src/scripts/statusbar.sh", NULL };

static const char *mutemic[]   = { "/bin/sh", "-c", "/usr/bin/wpctl set-mute @DEFAULT_AUDIO_SOURCE@ toggle && ~/src/scripts/statusbar.sh", NULL };

static const char *uplum[]     = { "/bin/sh", "-c", "~/src/scripts/brightness.sh + && ~/src/scripts/statusbar.sh", NULL };
static const char *downlum[]   = { "/bin/sh", "-c", "~/src/scripts/brightness.sh - && ~/src/scripts/statusbar.sh", NULL };

static const char *status[]    = { "/bin/sh", "-c", "sleep 0.5 && ~/src/scripts/statusbar.sh && sleep 8 && ~/src/scripts/statusbar.sh", NULL };

static const char *scrotfull[] = { "/bin/sh", "-c", "~/src/scripts/screenshot.sh full", NULL };
static const char *scrotsel[]  = { "/bin/sh", "-c", "~/src/scripts/screenshot.sh select", NULL };
static const char *scrotclip[] = { "/bin/sh", "-c", "~/src/scripts/screenshot.sh clipboard", NULL };

static const char *qalq[]      = { "/bin/sh", "-c", "~/src/scripts/calculator.sh", NULL };

static const char *toggaudio[] = { "/bin/sh", "-c", "mpc toggle && ~/src/scripts/statusbar.sh", NULL };
static const char *stopaudio[] = { "/bin/sh", "-c", "mpc stop && ~/src/scripts/statusbar.sh", NULL };
static const char *prevaudio[] = { "/bin/sh", "-c", "mpc prev && ~/src/scripts/statusbar.sh", NULL };
static const char *nextaudio[] = { "/bin/sh", "-c", "mpc next && ~/src/scripts/statusbar.sh", NULL };
static const char *randaudio[] = { "/bin/sh", "-c", "mpc random && ~/src/scripts/statusbar.sh", NULL };

static const Key keys[] = {
	/* modifier                     key                       function        argument */
	{ MODKEY,                       XK_p,                     spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,                     togglebar,      {0} },
	{ MODKEY,                       XK_j,                     focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                     incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                     setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                     setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,                zoom,           {0} },
	{ MODKEY,                       XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                     killclient,     {0} },
	{ MODKEY,                       XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                 setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                 togglefloating, {0} },
	{ MODKEY,                       XK_agrave,                view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_agrave,                tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_semicolon,             focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_semicolon,             tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_ampersand,                             0)
	TAGKEYS(                        XK_eacute,                                1)
	TAGKEYS(                        XK_quotedbl,                              2)
	TAGKEYS(                        XK_apostrophe,                            3)
	TAGKEYS(                        XK_parenleft,                             4)
	TAGKEYS(                        XK_minus,                                 5)
	TAGKEYS(                        XK_egrave,                                6)
	TAGKEYS(                        XK_underscore,                            7)
	TAGKEYS(                        XK_ccedilla,                              8)
	{ MODKEY|ShiftMask,             XK_q,                     quit,           {0} },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = upvol } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = mutevol } },
	{ 0,                            XF86XK_AudioMicMute,      spawn,          {.v = mutemic } },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn,          {.v = uplum } },
	{ 0,                            XF86XK_MonBrightnessDown, spawn,          {.v = downlum } },
	{ 0,                            XF86XK_RFKill,            spawn,          {.v = status } },
	{ MODKEY|ShiftMask,             XK_s,                     spawn,          {.v = scrotfull } },
	{ MODKEY|ShiftMask|ControlMask, XK_s,                     spawn,          {.v = scrotsel } },
	{ MODKEY|ShiftMask|Mod1Mask,    XK_s,                     spawn,          {.v = scrotclip } },
	{ 0,                            XK_Print,                 spawn,          {.v = scrotfull } },
	{ ControlMask,                  XK_Print,                 spawn,          {.v = scrotsel } },
	{ Mod1Mask,                     XK_Print,                 spawn,          {.v = scrotclip } },
	{ ControlMask|Mod1Mask,         XK_Tab,                   view,           {0} },
	{ 0,                            XF86XK_Calculator,        spawn,          {.v = qalq } },
	{ 0,                            XF86XK_AudioPlay,         spawn,          {.v = toggaudio } },
	{ 0,                            XF86XK_AudioStop,         spawn,          {.v = stopaudio } },
	{ 0,                            XF86XK_AudioPrev,         spawn,          {.v = prevaudio } },
	{ 0,                            XF86XK_AudioNext,         spawn,          {.v = nextaudio } },
	{ ControlMask,                  XF86XK_AudioPlay,         spawn,          {.v = randaudio } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

