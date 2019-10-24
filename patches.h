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

/* This patch changes the rectangle indicating if a tag is used by a client into a bar
 * above the tag name for better visibility.
 * https://dwm.suckless.org/patches/activetagindicatorbar/
 */
#define ACTIVETAGINDICATORBAR_PATCH 0

/* The alpha patch adds transparency for the status bar.
 * https://dwm.suckless.org/patches/alpha/
 */
#define ALPHA_PATCH 0

/* This patch introduces alternative tags which can be switched on the fly for the
 * sole purpose of providing visual aid.
 * https://dwm.suckless.org/patches/alternativetags/
 */
#define ALTERNATIVE_TAGS_PATCH 0

/* This patch prevents the focus to drift from the active fullscreen client when
 * using focusstack().
 * https://dwm.suckless.org/patches/alwaysfullscreen/
 */
#define ALWAYSFULLSCREEN_PATCH 0

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

/* By default, windows that are not visible when requesting a resize/move will not
 * get resized/moved. With this patch, they will.
 * https://dwm.suckless.org/patches/autoresize/
 */
#define AUTORESIZE_PATCH 0

/* Enhanced taskbar that shows the titles of all visible windows in the status bar
 * and allows focus / hiding / unhiding of windows by clicking on the status bar.
 * Awesomebar takes precedence over fancybar.
 * https://dwm.suckless.org/patches/awesomebar/
 */
#define AWESOMEBAR_PATCH 0

/* This patch adds an iscentered rule to automatically center clients on the current monitor.
 * This patch takes precedence over centeredwindowname and fancybar patches.
 * https://dwm.suckless.org/patches/center/
 */
#define CENTER_PATCH 0

/* This patch centers the WM_NAME of the currently selected window on the status bar.
 * Both fancybar and awesomebar patches take precedence over this patch.
 * https://dwm.suckless.org/patches/centeredwindowname/
 */
#define CENTEREDWINDOWNAME_PATCH 0

/* This patch provides the ability to assign different weights to clients in their
 * respective stack in tiled layout.
 * https://dwm.suckless.org/patches/cfacts/
 */
#define CFACTS_PATCH 0

/* This patch allows color attributes to be set through the command line.
 * https://dwm.suckless.org/patches/cmdcustomize/
 */
#define CMDCUSTOMIZE 0

/* This patch tweaks the tagging interface so that you can select multiple tags for tag
 * or view by pressing all the right keys as a combo. For example to view tags 1 and 3,
 * hold MOD and then press and hold 1 and 3 together.
 * https://dwm.suckless.org/patches/combo/
 */
#define COMBO_PATCH 0

/* The cyclelayouts patch lets you cycle through all your layouts.
 * https://dwm.suckless.org/patches/cyclelayouts/
 */
#define CYCLELAYOUTS_PATCH 0

/* This patch lets you resize the split in the tile layout (i.e. modify mfact) by holding
 * the modkey and dragging the mouse.
 * This patch can be a bit wonky with other layouts, but generally works.
 * https://dwm.suckless.org/patches/dragmfact/
 */
#define DRAGMFACT_PATCH 0

/* Simple dwmc client using a fork of fsignal to communicate with dwm.
 * To use this either copy the patch/dwmc shell script to somewhere in your path or
 * uncomment the following line in Makefile: #cp -f patch/dwmc ${DESTDIR}${PREFIX}/bin
 * http://dwm.suckless.org/patches/dwmc/
 */
#define DWMC_PATCH 0

/* This patch allows no tag at all to be selected. The result is that dwm will start with
 * no tag selected and when you start a client with no tag rule and no tag selected then
 * it will be opened on the first tag.
 * https://dwm.suckless.org/patches/emptyview/
 */
#define EMPTYVIEW_PATCH 0

/* Adds EWMH support for _NET_NUMBER_OF_DESKTOPS, _NET_CURRENT_DESKTOP, _NET_DESKTOP_NAMES
 * and _NET_DESKTOP_VIEWPORT, which allows for compatibility with other bars and programs
 * that request workspace information. For example polybar's xworkspaces module.
 * https://dwm.suckless.org/patches/ewmhtags/
 */
#define EWMHTAGS_PATCH 0

/* This patch allows the user to change size and placement of floating windows using only the
 * keyboard. It also allows for temporary vertical and horizontal extension of windows similar
 * to other WMs fill command.
 * https://dwm.suckless.org/patches/exresize/
 */
#define EXRESIZE_PATCH 0

/* This patch will enable an extra status bar in dwm in a similar manner to the dualstatus
 * patch. If the primary status is at the top via topbar then the extra status bar will be
 * placed at the bottom and vice versa.
 * https://dwm.suckless.org/patches/extrabar/
 */
#define EXTRABAR_PATCH 0

/* Only allow clients to "fullscreen" into the space currently given to them.
 * As an example, this will allow you to view a fullscreen video in your browser on
 * one half of the screen, while having the other half available for other tasks.
 * https://dwm.suckless.org/patches/fakefullscreen/
 */
#define FAKEFULLSCREEN_PATCH 0

/* This patch shows the titles of all visible windows in the status bar
 * (as opposed to showing only the selected one).
 * Awesomebar takes precedence over fancybar. Fancybar takes precedence over
 * the centeredwindowname patch.
 * https://dwm.suckless.org/patches/fancybar/
 */
#define FANCYBAR_PATCH 0

/* This patch provides the ability to focus the tag on the immediate left or right of the
 * currently focused tag. It also allows to send the focused window either on the left or
 * the right tag.
 * http://dwm.suckless.org/patches/focusadjacenttag/
 */
#define FOCUSADJACENTTAG_PATCH 0

/* Switch focus only by mouse click and not sloppy (focus follows mouse pointer).
 * https://dwm.suckless.org/patches/focusonclick/
 */
#define FOCUSONCLICK_PATCH 0

/* Selects the next window having the urgent flag regardless of the tag it is on.
 * The urgent flag can be artificially set with the following xdotool command on any window:
 *   xdotool selectwindow -- set_window --urgency 1
 * https://dwm.suckless.org/patches/focusurgent/
 */
#define FOCUSURGENT_PATCH 0

/* This patch allows a different border color to be chosen for floating windows.
 * https://dwm.suckless.org/patches/float_border_color/
 */
#define FLOAT_BORDER_COLOR_PATCH 0

/* By default, dwm responds to _NET_ACTIVE_WINDOW client messages by setting
 * the urgency bit on the named window. This patch activates the window instead.
 * https://dwm.suckless.org/patches/focusonnetactive/
 */
#define FOCUSONNETACTIVE_PATCH 0

/* Applies the monocle layout with the focused client on top and hides the bar. When pressed
 * again it shows the bar and restores the layout that was active before going fullscreen.
 * NB: This patch assumes that the third layout is monocle and that the bar is shown.
 * https://dwm.suckless.org/patches/fullscreen/
 */
#define FULLSCREEN_PATCH 0

/* This patch prevents dwm from drawing tags with no clients (i.e. vacant) on the bar.
 * https://dwm.suckless.org/patches/hide_vacant_tags/
 */
#define HIDEVACANTTAGS_PATCH 0

/* With this patch dwm's built-in status bar is only shown when HOLDKEY is pressed
 * and the bar will now overlay the display.
 * http://dwm.suckless.org/patches/holdbar/
 */
#define HOLDBAR_PATCH 0

/* Adds rule option for clients to avoid accidental termination by killclient for sticky windows.
 * https://dwm.suckless.org/patches/ispermanent/
 */
#define ISPERMANENT_PATCH 0

/* Sometimes dwm crashes when it cannot render some glyphs in window titles (usually emoji).
 * This patch is essentially a hack to ignore any errors when drawing text on the status bar.
 * https://groups.google.com/forum/m/#!topic/wmii/7bncCahYIww
 * https://docs.google.com/viewer?a=v&pid=forums&srcid=MDAwODA2MTg0MDQyMjE0OTgzMzMBMDQ3ODQzODkyMTU3NTAyMTMxNTYBX2RUMVNtOUtDQUFKATAuMQEBdjI&authuser=0
 */
#define IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH 0

/* This patch adds a keybinding to kills all visible clients that are not selected.
 * https://dwm.suckless.org/patches/killunsel/
 */
#define KILLUNSEL_PATCH 0

/* Moves the layout symbol in the status bar to the left hand side.
 * http://dwm.suckless.org/patches/leftlayout/
 */
#define LEFTLAYOUT_PATCH 0

/* By default in dwm it is possible to make an application fullscreen, then use
 * the focusstack keybindings to focus on other windows beneath the current window.
 * It is also possible to spawn new windows (e.g. a terminal) that end up getting
 * focus while the previous window remains in fullscreen. This patch ensures that
 * in such scenarios the previous window loses fullscreen.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-losefullscreen-6.2.diff
 */
#define LOSEFULLSCREEN_PATCH 0

/* This patch adds helper functions for maximizing, horizontally and vertically, floating
 * windows using keybindings.
 * https://dwm.suckless.org/patches/maximize/
 */
#define MAXIMIZE_PATCH 0

/* Control Music Player Daemon via keybinds.
 * This patch depends on an additional library lmdpclient so if you want to enable this
 * then you will also have to append -lmpdclient to the LIBS configuration in config.mk.
 * A placeholder has been added there for reference.
 * https://dwm.suckless.org/patches/mpdcontrol/
 */
#define MDPCONTROL_PATCH 0

/* Adds rules per monitor, e.g. have default layouts per monitor.
 * The use case for this is if the second monitor is vertical (i.e. rotated) then
 * you may want to use a different default layout for this monitor than what is
 * used for the main monitor. E.g. normal vertical split for main monitor and
 * horizontal split for the second.
 */
#define MONITOR_RULES_PATCH 0

/* Always display the the monocle-symbol as defined in config.h if the monocle-layout
 * is activated. Do not display the number of open clients in the current tag.
 * https://dwm.suckless.org/patches/monoclesymbol/
 */
#define MONOCLESYMBOL_PATCH 0

/* This patch allows you to move clients around in the stack and swap them with the master.
 * https://dwm.suckless.org/patches/movestack/
 */
#define MOVESTACK_PATCH 0

/* Removes the border when there is only one window visible.
 * https://dwm.suckless.org/patches/noborder/
 */
#define NOBORDER_PATCH 0

/* Enable modifying or removing dmenu in config.def.h which resulted previously in a
 * compilation error because two lines of code hardcode dmenu into dwm.
 * https://dwm.suckless.org/patches/nodmenu/
 */
#define NODMENU_PATCH 0

/* This patch makes it so dwm will only exit via quit() if no windows are open.
 * This is to prevent you accidentally losing all your work.
 * https://dwm.suckless.org/patches/onlyquitonempty/
 */
#define ONLYQUITONEMPTY_PATCH 0

/* The pertag patch adds nmaster, mfacts and layouts per tag rather than per
 * monitor (default).
 * https://dwm.suckless.org/patches/pertag/
 */
#define PERTAG_PATCH 0

/* This controls whether or not to also store bar position on a per
 * tag basis, or leave it as one bar per monitor.
 */
#define PERTAGBAR_PATCH 0

/* This patch provides a way to move clients up and down inside the client list.
 * https://dwm.suckless.org/patches/push/
 */
#define PUSH_PATCH 0

/* This patch provides a way to move clients up and down inside the client list,
 * but does not push up or down into the master area (except that it does not take
 * nmaster into account).
 * This takes precedence over the push patch above.
 * https://dwm.suckless.org/patches/push/
 */
#define PUSH_NO_MASTER_PATCH 0

/* By default, windows only resize from the bottom right corner. With this
 * patch the mouse is warped to the nearest corner and you resize from there.
 * https://dwm.suckless.org/patches/resizecorners/
 */
#define RESIZECORNERS_PATCH 0

/* Adds a keyboard shortcut to restart dwm or alternatively by using kill -HUP dwmpid.
 * Additionally dwm can quit cleanly by using kill -TERM dwmpid.
 * https://dwm.suckless.org/patches/restartsig/
 */
#define RESTARTSIG_PATCH 0

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

/* The scratchpad patch allows you to spawn or restore a floating terminal window.
 * It is typically useful when one need to do some short typing.
 * https://dwm.suckless.org/patches/scratchpad/
 */
#define SCRATCHPAD_PATCH 0

/* Allows restarting dwm without the dependency of an external script.
 * https://dwm.suckless.org/patches/selfrestart/
 */
#define SELFRESTART_PATCH 0

/* This patch allows border pixels to be changed during runtime.
 * https://dwm.suckless.org/patches/setborderpx/
 */
#define SETBORDERPX_PATCH 0

/* In a multi-head setup monitor 0 is by default the primary screen, with the left and right
 * screen being monitor 1 and 2 respectively. This patch sorts screens left to right (or
 * top to bottom in a vertical layout) which aims to address some inconsistencies when it
 * comes to focusmon, tagmon and similar functionality.
 * https://www.mail-archive.com/hackers@suckless.org/msg09400.html
 */
#define SORTSCREENS_PATCH 0

/* Spawns programs from currently focused client's working directory.
 * https://dwm.suckless.org/patches/spawn_cwd/
 */
#define SPAWNCMD_PATCH 0

/* This patch draws and updates the statusbar on all monitors.
 * https://dwm.suckless.org/patches/statusallmons/
 */
#define STATUSALLMONS_PATCH 0

/* This patch enables colored text in the status bar. It changes the way colors are defined
 * in config.h allowing multiple color combinations for use in the status script.
 * https://dwm.suckless.org/patches/statuscolors/
 */
#define STATUSCOLORS_PATCH 0

/* This patch adds configuration options for horizontal and vertical padding in the status bar.
 * https://dwm.suckless.org/patches/statuspadding/
 */
#define STATUSPADDING_PATCH 0

/* Adds toggleable keyboard shortcut to make a client 'sticky', i.e. visible on all tags.
 * https://dwm.suckless.org/patches/sticky/
 */
#define STICKY_PATCH 0

/* The systray patch adds systray for the status bar.
 * https://dwm.suckless.org/patches/systray/
 */
#define SYSTRAY_PATCH 0

/* This patch adds "window swallowing" to dwm as known from Plan 9's windowing system rio.
 * Clients marked with isterminal in config.h swallow a window opened by any child process,
 * e.g. running xclock in a terminal. Closing the xclock window restores the terminal window
 * in the current position.
 * https://dwm.suckless.org/patches/swallow/
 */
#define SWALLOW_PATCH 0

/* Switch focus between the master and stack columns using a single keybinding.
 * https://dwm.suckless.org/patches/switchcol/
 */
#define SWITCHCOL_PATCH 0

/* By default dwm allow you to set application specific rules so that you can have your browser,
 * for example, start up on tag 9 optionally on a given monitor when you open your browser it is
 * then automatically moved to the configured tag, but you have to manually enable the tag to see
 * the newly opened application.
 * This patch adds an extra configuration option for individual rules where:
 *   0 is default behaviour
 *   1 automatically moves you to the tag of the newly opened application and
 *   2 enables the tag of the newly opened application in addition to your existing enabled tags
 *
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-switchtag-6.2.diff
 */
#define SWITCHTAG_PATCH 0

/* Adds keyboard shortcuts to move all (or only floating) windows from one tag to another.
 * https://dwm.suckless.org/patches/tagall/
 */
#define TAGALL_PATCH 0

/* This patch allows you to move all visible windows on a monitor to an adjacent monitor.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagallmon-6.2.diff
 */
#define TAGALLMON_PATCH 0

/* This patch adds an option to place tags in rows like in many other window managers.
 * https://dwm.suckless.org/patches/taggrid/
 */
#define TAGGRID_PATCH 0

/* This patch makes new clients attach into the stack area when you toggle a new tag into
 * view. This means your master area will remain unchanged when toggling views.
 * The allmaster patch will cause all clients in the master area to be left alone. This patch
 * takes precedence over the onemaster tagintostack patch.
 * https://dwm.suckless.org/patches/tagintostack/
 */
#define TAGINTOSTACK_ALLMASTER_PATCH 0

/* This patch makes new clients attach into the stack area when you toggle a new tag into
 * view. This means your master area will remain unchanged when toggling views.
 * The onemaster patch will cause the first client in the master area to be left alone.
 * https://dwm.suckless.org/patches/tagintostack/
 */
#define TAGINTOSTACK_ONEMASTER_PATCH 0

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

/* Adds a new color scheme used by the (selected) window title in the bar.
 * https://dwm.suckless.org/patches/titlecolor/
 */
#define TITLECOLOR_PATCH 0

/* This patch allows you to toggle fullscreen on and off using a single shortcut key.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-togglefullscreen-6.2.diff
 */
#define TOGGLEFULLSCREEN_PATCH 0

/* This patch resets isfloating on any visible windows that have it set.
 * Optionally also applies a layout.
 * https://dwm.suckless.org/patches/unfloatvisible/
 */
#define UNFLOATVISIBLE_PATCH 0

/* This patch makes "urgent" windows have different colors.
 * https://dwm.suckless.org/patches/urgentborder/
 */
#define URGENTBORDER_PATCH 0

/* This patch adds configurable gaps between windows differentiating between outer, inner,
 * horizontal and vertical gaps.
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-vanitygaps-6.2.diff
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-vanitygaps-cfacts-6.2.diff
 */
#define VANITYGAPS_PATCH 0

/* Follow a window to the tag it is being moved to.
 * https://dwm.suckless.org/patches/viewontag/
 */
#define VIEWONTAG_PATCH 0

/* This patch warps the mouse cursor to the center of the currently focused window or screen
 * when the mouse cursor is (a) on a different screen or (b) on top of a different window.
 * https://dwm.suckless.org/patches/warp/
 */
#define WARP_PATCH 0

/* Sometimes a single application opens different windows depending on the task
 * at hand and this is often reflected in the WM_WINDOW_ROLE(STRING) x property.
 * This patch adds the role field to the rule configuration so that one can
 * differentiate between, say, Firefox "browser" vs "Preferences" vs "Manager"
 * or Google-chrome "browser" vs "pop-up".
 * https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-windowrolerule-6.2.diff
 */
#define WINDOWROLERULE_PATCH 0

/* The winview patch allows switching the view to that of a given client from the all-window
 * view (Mod-0) using a keyboard shortcut.
 * http://dwm.suckless.org/patches/winview/
 */
#define WINVIEW_PATCH 0

/* Allows dwm to read colors from xrdb (.Xresources) during runtime. Compatible with
 * the float border color, awesomebar, urgentborder and titlecolor patches.
 * https://dwm.suckless.org/patches/xrdb/
 */
#define XRDB_PATCH 0

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

/* Same as the default tile layout except clients in the master area are arranged in
 * columns (i.e. left to right).
 * https://dwm.suckless.org/patches/columns/
 */
#define COLUMNS_LAYOUT 0

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

/* Flextile deluxe layout.
 * A revamped, more flexible, and over-the-top version of the original flextile layout.
 * https://dwm.suckless.org/patches/flextile/ (original)
 */
#define FLEXTILE_DELUXE_LAYOUT 0

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
