/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const char *light_up[] = {"/usr/bin/light", "-A", "5", NULL};
static const char *light_down[] = {"/usr/bin/light", "-U", "5", NULL};
static const char *upvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "+5%",
                              NULL};
static const char *downvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "-5%",
                                NULL};
static const char *mutevol[] = {"/usr/bin/pactl", "set-sink-mute", "0",
                                "toggle", NULL};
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int gappx = 3;    /* gaps between windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *fonts[] = {"AnonymicePro Nerd Font:style=bold:size=13"};
static const char dmenufont[] = "AnonymicePro Nerd Font:style=bold:size=13";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#ff8939";
static const char col_gray3[] = "#000000";
static const char col_gray4[] = "#eeeeee";
static const char col_gray5[] = "#341f1c";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray4, col_gray5, col_gray2},
    [SchemeSel] = {col_gray4, col_gray2, col_gray5},
};

/* tagging */
static const char *tags[] = {"󰣇", "", "", "󱇧", "5",
                             "6",    "7",   "8",   "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"brave", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",  dmenumon,  "-fn", dmenufont, "-nb", col_gray5, "-nf",
    col_gray4,   "-sb", col_gray2, "-sf", col_gray4, NULL};
static const char *termcmd[] = {"st", NULL};
static const char *web[] = {"brave", NULL};
static const char *poweroff[] = {"poweroff", NULL};
static const char *reboot[] = {"reboot", NULL};
static const char *lock[] = {"slock", NULL};
static const char *filemanager[] = {"thunar", NULL};
static const char *code[] = {"st", "-e", "nvim", NULL};
static const char *notes[] = {"obsidian", NULL};
static const Key keys[] = {
    /* modifier                     key        function        argument */
    {0, XF86XK_MonBrightnessUp, spawn, {.v = light_up}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = light_down}},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
    {0, XF86XK_AudioMute, spawn, {.v = mutevol}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
    {MODKEY, XK_s, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_w, spawn, {.v = web}},
    {MODKEY | ShiftMask, XK_p, spawn, {.v = poweroff}},
    {MODKEY | ShiftMask, XK_r, spawn, {.v = reboot}},
    {MODKEY | ShiftMask, XK_l, spawn, {.v = lock}},
    {MODKEY, XK_f, spawn, {.v = filemanager}},
    {MODKEY, XK_c, spawn, {.v = code}},
    {MODKEY, XK_n, spawn, {.v = notes}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_x, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
