/*
 * This file contains patch control flags.
 *
 * In principle you should be able to mix and match any patches
 * you may want. In cases where patches are logically incompatible
 * one patch may take precedence over the other as noted in the
 * relevant descriptions.
 */

/* The alpha patch adds transparency for the status bar.
 * https://dwm.suckless.org/patches/alpha/
 */
#define ALPHA_PATCH 0

/* The systray patch adds systray for the status bar.
 * https://dwm.suckless.org/patches/systray/
 */
#define SYSTRAY_PATCH 0

/* The pertag patch adds nmaster, mfacts and layouts per tag rather
 * than per monitor (default).
 * https://dwm.suckless.org/patches/pertag/
 */
#define PERTAG_PATCH 0

/* This controls whether or not to also store bar position on a per
 * tag basis, or leave it as one bar per monitor.
 */
#define PERTAGBAR_PATCH 0

/* If you try to send a fullscreen window to an adjacent monitor using tagmon then
 * the window is moved behind the scenes, but it remains in fullscreen on the original
 * monitor until you exit fullscreen view (at which point it will appear on the adjacent
 * monitor). This patch allows a fullscreen window to be moved to an adjacent monitor
 * while remaining in fullscreen.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagmonfixfs-6.2.diff
 */
#define TAGMONFIXFS_PATCH 0

/* This patch allows you to toggle fullscreen on and off using a single shortcut key.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-togglefullscreen-6.2.diff
 */
#define TOGGLEFULLSCREEN_PATCH 0

/* The zoomswap patch allows a master and a stack window to swap places
 * rather than every window on the screen changing position.
 * https://dwm.suckless.org/patches/zoomswap/
 */
#define ZOOMSWAP_PATCH 0