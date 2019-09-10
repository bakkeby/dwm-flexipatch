/*
 * This file contains patch control flags.
 *
 * In principle you should be able to mix and match any patches
 * you may want. In cases where patches are logically incompatible
 * one patch may take precedence over the other as noted in the
 * relevant descriptions.
 *
 * Although layouts typically come as patches they are differentiated
 * here for grouping purposes.
 */

/* Patches */

/* The alpha patch adds transparency for the status bar.
 * https://dwm.suckless.org/patches/alpha/
 */
#define ALPHA_PATCH 0

/* This patch introduces alternative tags which can be switched on the fly for the
 * sole purpose of providing visual aid.
 * https://dwm.suckless.org/patches/alternativetags/
 */
#define ALTERNATIVE_TAGS_PATCH 0

/* This patch adds new clients above the selected client, instead of always
 * becoming the new master. This behaviour is known from Xmonad.
 * This patch takes precedence over ATTACHASIDE_PATCH.
 * https://dwm.suckless.org/patches/attachabove/
 */
#define ATTACHABOVE_PATCH 0

/* This patch adds new clients on top of the stack.
 * This patch takes precedence over ATTACHBELOW_PATCH.
 * https://dwm.suckless.org/patches/attachaside/
 */
#define ATTACHASIDE_PATCH 0

/* This patch adds new clients below the selected client.
 * This patch takes precedence over ATTACHBOTTOM_PATCH.
 * https://dwm.suckless.org/patches/attachbelow/
 */
#define ATTACHBELOW_PATCH 0

/* This patch adds new clients at the bottom of the stack.
 * https://dwm.suckless.org/patches/attachbottom/
 */
#define ATTACHBOTTOM_PATCH 0

/* This patch will make dwm run "~/.config/dwm/autostart_blocking.sh" and
 * "~/.config/dwm/autostart.sh &" before entering the handler loop. One or
 * both of these files can be ommited. Note the path inside .config rather
 * than the original ~/.dwm folder.
 * https://dwm.suckless.org/patches/autostart/
 */
#define AUTOSTART_PATCH 0

/* Enhanced taskbar that shows the titles of all visible windows in the status bar
 * and allows focus / hiding / unhiding of windows by clicking on the status bar.
 * Awesomebar takes precedence over fancybar.
 * https://dwm.suckless.org/patches/awesomebar/
 */
#define AWESOMEBAR_PATCH 0

/* This patch adds an iscentered rule to automatically center clients on the current monitor.
 * https://dwm.suckless.org/patches/center/
 */
#define CENTER_PATCH 0

/* This patch provides the ability to assign different weights to clients in their
 * respective stack in tiled layout.
 * https://dwm.suckless.org/patches/cfacts/
 */
#define CFACTS_PATCH 0

/* The cyclelayouts patch lets you cycle through all your layouts.
 * https://dwm.suckless.org/patches/cyclelayouts/
 */
#define CYCLELAYOUTS_PATCH 0

/* This patch shows the titles of all visible windows in the status bar
 * (as opposed to showing only the selected one).
 * Awesomebar takes precedence over fancybar.
 * https://dwm.suckless.org/patches/fancybar/
 */
#define FANCYBAR_PATCH 0

/* By default, dwm responds to _NET_ACTIVE_WINDOW client messages by setting
 * the urgency bit on the named window. This patch activates the window instead.
 * https://dwm.suckless.org/patches/focusonnetactive/
 */
#define FOCUSONNETACTIVE_PATCH 0

/* By default in dwm it is possible to make an application fullscreen, then use
 * the focusstack keybindings to focus on other windows beneath the current window.
 * It is also possible to spawn new windows (e.g. a terminal) that end up getting
 * focus while the previous window remains in fullscreen. This patch ensures that
 * in such scenarios the previous window loses fullscreen.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-losefullscreen-6.2.diff
 */
#define LOSEFULLSCREEN_PATCH 0

/* The pertag patch adds nmaster, mfacts and layouts per tag rather
 * than per monitor (default).
 * https://dwm.suckless.org/patches/pertag/
 */
#define PERTAG_PATCH 0

/* This controls whether or not to also store bar position on a per
 * tag basis, or leave it as one bar per monitor.
 */
#define PERTAGBAR_PATCH 0

/* By default, windows only resize from the bottom right corner. With this
 * patch the mouse is warped to the nearest corner and you resize from there.
 * https://dwm.suckless.org/patches/resizecorners/
 */
#define RESIZECORNERS_PATCH 0

/* This patch let's you rotate through the stack using keyboard shortcuts.
 * https://dwm.suckless.org/patches/rotatestack/
 */
#define ROTATESTACK_PATCH 0

/* This patch aves size and position of every floating window before it is forced
 * into tiled mode. If the window is made floating again then the old dimensions
 * will be restored.
 * https://dwm.suckless.org/patches/save_floats/
 */
#define SAVEFLOATS_PATCH 0

/* This patch adds configuration options for horizontal and vertical padding in the status bar.
 * https://dwm.suckless.org/patches/statuspadding/
 */
#define STATUSPADDING_PATCH 0

/* The systray patch adds systray for the status bar.
 * https://dwm.suckless.org/patches/systray/
 */
#define SYSTRAY_PATCH 0

/* By default dwm allow you to set application specific rules so that you can have your browser,
 * for example, start up on tag 9 optionally on a given monitor when you open your browser it is
 * then automatically moved to the configured tag, but you have to manually enable the tag to see
 * the newly opened application.
 * This patch adds an extra configuration option for individual rules where:
 *   0 is default behaviour
 *   1 automatically moves you to the tag of the newly opened application and
 *   2 enables the tag of the newly opened application in addition to your existing enabled tags

 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-switchtag-6.2.diff
 */
#define SWITCHTAG_PATCH 0

/* This patch allows you to move all visible windows on a monitor to an adjacent monitor.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagallmon-6.2.diff
 */
#define TAGALLMON_PATCH 0

/* If you try to send a fullscreen window to an adjacent monitor using tagmon then
 * the window is moved behind the scenes, but it remains in fullscreen on the original
 * monitor until you exit fullscreen view (at which point it will appear on the adjacent
 * monitor). This patch allows a fullscreen window to be moved to an adjacent monitor
 * while remaining in fullscreen.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagmonfixfs-6.2.diff
 */
#define TAGMONFIXFS_PATCH 0

/* This patch allows you to swap all visible windows on one monitor with those of an
 * adjacent monitor.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagswapmon-6.2.diff
 */
#define TAGSWAPMON_PATCH 0

/* This patch allows you to toggle fullscreen on and off using a single shortcut key.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-togglefullscreen-6.2.diff
 */
#define TOGGLEFULLSCREEN_PATCH 0

/* This patch adds configurable gaps between windows differentiating between outer, inner,
 * horizontal and vertical gaps.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-vanitygaps-6.2.diff
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-vanitygaps-cfacts-6.2.diff
 */
#define VANITYGAPS_PATCH 0

/* Sometimes a single application opens different windows depending on the task
 * at hand and this is often reflected in the WM_WINDOW_ROLE(STRING) x property.
 * This patch adds the role field to the rule configuration so that one can
 * differentiate between, say, Firefox "browser" vs "Preferences" vs "Manager"
 * or Google-chrome "browser" vs "pop-up".
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-windowrolerule-6.2.diff
 */
#define WINDOWROLERULE_PATCH 0

/* The zoomswap patch allows a master and a stack window to swap places
 * rather than every window on the screen changing position.
 * https://dwm.suckless.org/patches/zoomswap/
 */
#define ZOOMSWAP_PATCH 0

/* Layouts */

/* Bottomstack layout.
 * https://dwm.suckless.org/patches/bottomstack/
 */
#define BSTACK_LAYOUT 0

/* Bottomstack horizontal layout.
 * https://dwm.suckless.org/patches/bottomstack/
 */
#define BSTACKHORIZ_LAYOUT 0

/* Centered master layout.
 * https://dwm.suckless.org/patches/centeredmaster/
 */
#define CENTEREDMASTER_LAYOUT 0

/* Centered floating master layout.
 * https://dwm.suckless.org/patches/centeredmaster/
 */
#define CENTEREDFLOATINGMASTER_LAYOUT 0

/* Deck layout.
 * https://dwm.suckless.org/patches/deck/
 */
#define DECK_LAYOUT 0

/* Fibonacci dwindle layout.
 * https://dwm.suckless.org/patches/fibonacci/
 */
#define FIBONACCI_DWINDLE_LAYOUT 0

/* Fibonacci spiral layout.
 * https://dwm.suckless.org/patches/fibonacci/
 */
#define FIBONACCI_SPIRAL_LAYOUT 0

/* Flextile layout.
 * https://dwm.suckless.org/patches/flextile/
 */
#define FLEXTILE_LAYOUT 0

/* Gappless grid layout.
 * https://dwm.suckless.org/patches/gaplessgrid/
 */
#define GAPPLESSGRID_LAYOUT 0

/* Gridmode (grid) layout.
 * https://dwm.suckless.org/patches/gridmode/
 */
#define GRIDMODE_LAYOUT 0

/* Horizontal grid (horizgrid) layout.
 * https://dwm.suckless.org/patches/horizgrid/
 */
#define HORIZGRID_LAYOUT 0

/* Grid layout where nmaster controls the number of rows.
 * https://dwm.suckless.org/patches/nrowgrid/
 */
#define NROWGRID_LAYOUT 0

/* The default tile layout.
 * This can be optionally disabled in favour of other layouts.
 */
#define TILE_LAYOUT 1

/* Monocle layout (default).
 * This can be optionally disabled in favour of other layouts.
 */
#define MONOCLE_LAYOUT 1
