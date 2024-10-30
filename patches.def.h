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

/**
 * Bar modules
 */

/* Enhanced taskbar that shows the titles of all visible windows in the status bar
 * and allows focus / hiding / unhiding of windows by clicking on the status bar.
 * Awesomebar takes precedence over fancybar.
 * https://dwm.suckless.org/patches/awesomebar/
 */
#define BAR_AWESOMEBAR_PATCH 0

/* This patch depends on statuscmd patch and adds integration with a (patched)
 * dwmblocks instance to give a clickable status bar. One must not necessarily
 * have to use dwmblocks for this feature, any status updater that has support
 * for real-time signals (SIGRTMIN) can be used.
 *
 * dwmblocks: https://github.com/torrinfail/dwmblocks
 * https://dwm.suckless.org/patches/statuscmd/
 */
#define BAR_DWMBLOCKS_PATCH 0

/* Originally the dwmblocks + statuscmd patch used a user defined signal (SIGUSR1)
 * for communicating with dwmblocks to indicate update signal and what button was
 * pressed. The signalling was later changed to SIGRTMIN instead.
 *
 * Ultimately this makes dwmblocks instances that were patched with the old patch
 * are incompatible with the new dwm patch and vice versa.
 *
 * This is a compatibility patch that makes dwm use SIGUSR1 instead of SIGRTMIN so
 * if button clicks are not working then you may want to try enabling this.
 *
 * If dwmblocks happen to die like this when clicking on a status
 *
 *    [1]    54355 user-defined signal 1  dwmblocks
 *
 * then it suggests that dwmblocks does not support user defined signals and this
 * patch should be left disabled.
 *
 * Patch: https://gist.github.com/danbyl/54f7c1d57fc6507242a95b71c3d8fdea
 * https://dwm.suckless.org/patches/statuscmd/
 */
#define BAR_DWMBLOCKS_SIGUSR1_PATCH 0

/* This patch shows the titles of all visible windows in the status bar
 * (as opposed to showing only the selected one).
 * Awesomebar takes precedence over fancybar. Fancybar takes precedence over
 * the centeredwindowname patch.
 * https://dwm.suckless.org/patches/fancybar/
 */
#define BAR_FANCYBAR_PATCH 0

/* Being an evolution of the bartabgroups patch the flexwintitle patch specifically
 * taps into the many layout options that flextile-deluxe offers to produce a window
 * title section in the bar that is representative of what is shown on screen.
 */
#define BAR_FLEXWINTITLE_PATCH 0

/* Adds buttons to the bar that can be used to launch applications.
 * https://dwm.suckless.org/patches/launcher/
 */
#define BAR_LAUNCHER_PATCH 0

/* This patch adds a context menu for layout switching.
 *   - xmenu needs to be installed.
 *   - Edit layoutmenu.sh with the installed layouts and with correct indexes.
 *   - Place layoutmenu.sh in PATH.
 *   - The text of the menu items is for display only. Name them however you want.
 * https://dwm.suckless.org/patches/layoutmenu/
 */
#define BAR_LAYOUTMENU_PATCH 0

/* Show layout symbol in bar */
#define BAR_LTSYMBOL_PATCH 1

/* Adds powerline arrows for the status.
 * This uses statuscolors logic for choosing colors for the powerline. As these markers
 * are also control characters there is no explicit statuscmd support for this patch.
 *
 * Powerline separators are defined as:
 *    |\xXX  (creates a hard edge)
 *    <\xXX  (creates a less than arrow)
 *    /\xXX  (creates a diagonal line)
 *
 * Examples:
 *    xsetroot -name "$(echo -e '<\x01a<\x02b<\x03c')"
 *    xsetroot -name "$(echo -e '/\x01d/\x02e/\x03f')"
 *
 * https://gitlab.com/udiboy1209-suckless/dwm/-/commit/071f5063e8ac4280666828179f92788d893eea40#4b1a539194be7467cefbda22f675a3b7c19ceca7
 * https://dwm.suckless.org/patches/statuscolors/
 */
#define BAR_POWERLINE_STATUS_PATCH 0

/* Adds powerline arrows for the tags.
 * https://gitlab.com/udiboy1209-suckless/dwm/-/commit/071f5063e8ac4280666828179f92788d893eea40#4b1a539194be7467cefbda22f675a3b7c19ceca7
 */
#define BAR_POWERLINE_TAGS_PATCH 0

/* Alters the tags powerline to use forward slash instead of arrows */
#define BAR_POWERLINE_TAGS_SLASH_PATCH 0

/* This patch turns the titlebar area into a mfact-respecting tabbar showing each client's title.
 * https://dwm.suckless.org/patches/bartabgroups/
 */
#define BAR_TABGROUPS_PATCH 0

/* This patch adds an option to place tags in rows like in many other window managers.
 * https://dwm.suckless.org/patches/taggrid/
 */
#define BAR_TAGGRID_PATCH 0

/* Hover tag icons to see a preview of the windows on that tag.
 *
 * The patch depends on Imlib2 for icon scaling.
 * You need to uncomment the corresponding line in config.mk to use the -lImlib2 library
 *
 * Arch Linux:
 *     sudo pacman -S imlib2
 * Debian:
 *     sudo apt install libimlib2-dev
 *
 * As with the winicon patch you may want to consider adding the compiler flags of -O3 and
 * -march=native to enable auto loop vectorize for better performance.
 *
 * https://dwm.suckless.org/patches/tag-previews/
 */
#define BAR_TAGPREVIEW_PATCH 0

/* Show status in bar */
#define BAR_STATUS_PATCH 1

/* This patch adds a clickable button to the left hand side of the statusbar.
 * https://dwm.suckless.org/patches/statusbutton/
 */
#define BAR_STATUSBUTTON_PATCH 0

/* This patch adds the ability to execute shell commands based on the mouse button and position
 * when clicking the status bar. Refer to the website for usage.
 * https://dwm.suckless.org/patches/statuscmd/
 */
#define BAR_STATUSCMD_PATCH 0

/* Status2d allows colors and rectangle drawing in your dwm status bar.
 * This patch is incompatible with the statuscolors patch which takes precedence.
 * This patch is incompatible with the extrabar patch.
 * https://dwm.suckless.org/patches/status2d/
 */
#define BAR_STATUS2D_PATCH 0

/* Supplementary patch should you want to disable alpha for the status2d section */
#define BAR_STATUS2D_NO_ALPHA_PATCH 0

/* Addition to the status2d patch that allows the use of terminal colors (color0 through color15)
 * from xrdb in the status, allowing programs like pywal to change statusbar colors.
 * This adds the C and B codes to use terminal foreground and background colors respectively.
 * E.g. ^B5^ would use color5 as the background color.
 * https://dwm.suckless.org/patches/status2d/
 */
#define BAR_STATUS2D_XRDB_TERMCOLORS_PATCH 0

/* The systray patch adds systray for the status bar.
 * https://dwm.suckless.org/patches/systray/
 */
#define BAR_SYSTRAY_PATCH 0

/* Show tag symbols in the bar. */
#define BAR_TAGS_PATCH 1

/* Show tag symbols + class of master window in the bar.
 * https://dwm.suckless.org/patches/taglabels/
 */
#define BAR_TAGLABELS_PATCH 0

/* This patch underlines the selected tag, or optionally all tags.
 * https://dwm.suckless.org/patches/underlinetags/
 */
#define BAR_UNDERLINETAGS_PATCH 0

/* This patch adds the window icon next to the window title in the bar.
 *
 * The patch depends on Imlib2 for icon scaling.
 * You need to uncomment the corresponding line in config.mk to use the -lImlib2 library
 *
 * Arch Linux:
 *     sudo pacman -S imlib2
 * Debian:
 *     sudo apt install libimlib2-dev
 *
 * The author recommends adding the compiler flags of -O3 and -march=native to enable auto loop
 * vectorize for better performance.
 *
 * https://github.com/AdamYuan/dwm-winicon
 * https://dwm.suckless.org/patches/winicon
 */
#define BAR_WINICON_PATCH 0

/* Show window title in bar */
#define BAR_WINTITLE_PATCH 1

/* Shows window titles in the bar, but only for floating clients.
 * This depends on code from the flexwintitle patch.
 * Note that the configuration in config.def.h for this is merely an example. If combined
 * with the corresponding hidden patch then these two will overlap unless the width of the
 * modules are controlled.
 */
#define BAR_WINTITLE_FLOATING_PATCH 0

/* Shows window titles in the bar, but only for floating clients.
 * This depends on code from the flexwintitle patch.
 * Note that the configuration in config.def.h for this is merely an example. If combined
 * with the corresponding floating patch then these two will overlap unless the width of the
 * modules are controlled.
 */
#define BAR_WINTITLE_HIDDEN_PATCH 0

/* Title bar modules such as wintitle (default), fancybar and awesomebar
 * do not by default add left and/or right padding as they take up the
 * remaining space. These options allow you explicitly add padding should
 * you need it.
 */
#define BAR_TITLE_RIGHT_PAD_PATCH 0
#define BAR_TITLE_LEFT_PAD_PATCH 0

/**
 * Bar options
 */

/* This patch changes the rectangle indicating if a tag is used by a client into a bar
 * above the tag name for better visibility.
 * Set the tagindicatortype variable in config.h to INDICATOR_TOP_BAR to enable this.
 * https://dwm.suckless.org/patches/activetagindicatorbar/
 */
#define BAR_ACTIVETAGINDICATORBAR_PATCH N/A

/* Alternative patch to the activetagindicatorbar patch, adds the bar below the tag
 * icon rather than above.
 * Set the tagindicatortype variable in config.h to INDICATOR_BOTTOM_BAR to enable this.
 */
#define BAR_ACTIVETAGINDICATORBAR_ALT1_PATCH N/A

/* The alpha patch adds transparency for the status bar.
 * You need to uncomment the corresponding line in config.mk to use the -lXrender library
 * when including this patch.
 * https://dwm.suckless.org/patches/alpha/
 */
#define BAR_ALPHA_PATCH 0

/* This patch introduces alternative tags which can be switched on the fly for the
 * sole purpose of providing visual aid.
 * https://dwm.suckless.org/patches/alternativetags/
 */
#define BAR_ALTERNATIVE_TAGS_PATCH 0

/* This patches provides the ability to use alternative text for tags which contain at
 * least one window.
 * https://dwm.suckless.org/patches/alttagsdecoration/
 */
#define BAR_ALTTAGSDECORATION_PATCH 0

/* This patch enables dwm to manage external status bars such as lemonbar and polybar.
 * dwm treats the external bar as it would its own, so all regular dwm commands such as
 * togglebar affect the external bar in the same way.
 *
 * NB: Unless you want both anybar + dwm bar(s) then the recommendation is to disable all
 * bar modules and have { -2 } in the barrules.
 *
 * https://dwm.suckless.org/patches/anybar/
 */
#define BAR_ANYBAR_PATCH 0

/* Anybar option to place the next bar depending on previous bar's position (top or bottom) */
#define BAR_ANYBAR_TOP_AND_BOTTOM_BARS_PATCH 0

/* Anybar option to let dwm manage the width of the bar */
#define BAR_ANYBAR_MANAGE_WIDTH_PATCH 0

/* This patch adds a border around the status bar(s) just like the border of client windows.
 * https://codemadness.org/paste/dwm-border-bar.patch
 */
#define BAR_BORDER_PATCH 0

/* This patch centers the WM_NAME of the currently selected window on the status bar.
 * This is compatible with the wintitle, bartabgroups, flexwintitle and awesomebar bar
 * modules.
 * https://dwm.suckless.org/patches/centeredwindowname/
 */
#define BAR_CENTEREDWINDOWNAME_PATCH 0

/* Draws a dot indicator overlayed on each tag icon for each client. The selected client
 * is drawn as a larger horizontal line.
 * Set the tagindicatortype variable in config.h to INDICATOR_CLIENT_DOTS to enable this.
 * https://dwm.suckless.org/patches/clientindicators/
 */
#define BAR_CLIENTINDICATOR_PATCH N/A

/* Updates the position of dmenu to match that of the bar. I.e. if topbar is 0 then dmenu
 * will appear at the bottom and if 1 then dmenu will appear at the top.
 * https://dwm.suckless.org/patches/dmenumatchtop
 */
#define BAR_DMENUMATCHTOP_PATCH 0

/* Originally this was the extrabar patch, but as the handling of extra bars is now built-in
 * only the splitting of the status by a designated separator remains. As such this has been
 * renamed to more accurately reflect what it does - creating an extra status.
 * https://dwm.suckless.org/patches/extrabar/
 */
#define BAR_EXTRASTATUS_PATCH 0

/* Adds EWMH support for _NET_NUMBER_OF_DESKTOPS, _NET_CURRENT_DESKTOP, _NET_DESKTOP_NAMES
 * and _NET_DESKTOP_VIEWPORT, which allows for compatibility with other bars and programs
 * that request workspace information. For example polybar's xworkspaces module.
 *
 * This patch also includes support for adding the _IS_FLOATING property for floating windows
 * allowing for compositors to treat floating windows differently to tiled windows.
 *
 * E.g. this setting makes picom only render shadows for floating windows:
 *
 *     shadow-exclude = [ "! _IS_FLOATING@:32c = 1" ];
 *
 * https://github.com/bakkeby/dwm-flexipatch/issues/50 (_IS_FLOATING patch)
 * https://dwm.suckless.org/patches/ewmhtags/
 */
#define BAR_EWMHTAGS_PATCH 0

/* Allows the bar height to be explicitly set rather than being derived from font.
 * https://dwm.suckless.org/patches/bar_height/
 */
#define BAR_HEIGHT_PATCH 0

/* This patch prevents dwm from drawing tags with no clients (i.e. vacant) on the bar.
 * https://dwm.suckless.org/patches/hide_vacant_tags/
 */
#define BAR_HIDEVACANTTAGS_PATCH 0

/* With this patch dwm's built-in status bar is only shown when HOLDKEY is pressed
 * and the bar will now overlay the display.
 * http://dwm.suckless.org/patches/holdbar/
 */
#define BAR_HOLDBAR_PATCH 0

/* Sometimes dwm crashes when it cannot render some glyphs in window titles (usually emoji).
 * This patch is essentially a hack to ignore any errors when drawing text on the status bar.
 * https://groups.google.com/forum/m/#!topic/wmii/7bncCahYIww
 * https://docs.google.com/viewer?a=v&pid=forums&srcid=MDAwODA2MTg0MDQyMjE0OTgzMzMBMDQ3ODQzODkyMTU3NTAyMTMxNTYBX2RUMVNtOUtDQUFKATAuMQEBdjI&authuser=0
 */
#define BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH 0

/* This patch adds back in the workaround for a BadLength error in the Xft library when color
 * glyphs are used. This is for systems that do not have an updated version of the Xft library
 * (or generally prefer monochrome fonts).
 */
#define BAR_NO_COLOR_EMOJI_PATCH 0

/* This patch adds vertical and horizontal space between the statusbar and the edge of the screen.
 * https://dwm.suckless.org/patches/barpadding/
 */
#define BAR_PADDING_PATCH 0

/* Same as barpadding patch but specifically tailored for the vanitygaps patch in that the outer
 * bar padding is derived from the vanitygaps settings. In addition to this the bar padding is
 * toggled in unison when vanitygaps are toggled. Increasing or decreasing gaps during runtime
 * will not affect the bar padding.
 */
#define BAR_PADDING_VANITYGAPS_PATCH 0

/* Smart bar padding patch that automatically adjusts the padding when there is
 * only one client on the monitor. Works well with vanitygaps and barpadding
 * patches.
 */
#define BAR_PADDING_SMART_PATCH 0

/* This patch adds simple markup for status messages using pango markup.
 * This depends on the pango library v1.44 or greater.
 * You need to uncomment the corresponding lines in config.mk to use the pango libraries
 * when including this patch.
 *
 * Note that the pango patch does not protect against the BadLength error from Xft
 * when color glyphs are used, which means that dwm will crash if color emoji is used.
 *
 * If you need color emoji then you may want to install this patched library from the AUR:
 * https://aur.archlinux.org/packages/libxft-bgra/
 *
 * A long term fix for the libXft library is pending approval of this pull request:
 * https://gitlab.freedesktop.org/xorg/lib/libxft/-/merge_requests/1
 *
 * Also see:
 * https://developer.gnome.org/pygtk/stable/pango-markup-language.html
 * https://lists.suckless.org/hackers/2004/17285.html
 * https://dwm.suckless.org/patches/pango/
 */
#define BAR_PANGO_PATCH 0

/* This patch allows the status text to be fixed to the bar on a specific
 * monitor rather than being drawn on the focused monitor.
 * The statusallmons patch takes precedence over this patch.
 * https://dwm.suckless.org/patches/staticstatus/
 */
#define BAR_STATICSTATUS_PATCH 0

/* This patch draws and updates the statusbar on all monitors.
 * https://dwm.suckless.org/patches/statusallmons/
 */
#define BAR_STATUSALLMONS_PATCH 0

/* This patch enables colored text in the status bar. It changes the way colors are defined
 * in config.h allowing multiple color combinations for use in the status script.
 * This patch is incompatible with and takes precedence over the status2d patch.
 *
 * This patch is compatible with the statuscmd patch with the caveat that the first 16 markers
 * are reserved for status colors restricting block signals to 17 through 31.
 *
 * https://dwm.suckless.org/patches/statuscolors/
 */
#define BAR_STATUSCOLORS_PATCH 0

/* This patch adds configuration options for horizontal and vertical padding in the status bar.
 * https://dwm.suckless.org/patches/statuspadding/
 */
#define BAR_STATUSPADDING_PATCH 0

/* This patch adds the ability for dwm to read colors from the linux virtual console.
 *    /sys/module/vt/parameters/default_{red,grn,blu}
 * Essentially this way the colors you use in your regular tty is "mirrored" to dwm.
 * https://dwm.suckless.org/patches/vtcolors/
 */
#define BAR_VTCOLORS_PATCH 0

/* This patch allows client windows to be hidden. This code was originally part of awesomebar,
 * but has been separated out so that other bar modules can take advantage of it.
 * Both awesomebar and bartabgroups patches depend on this patch and it will be auto-enabled
 * during compile time if it is needed. Note that if using flexipatch-finalizer this must be
 * explicitly enabled.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-barmodules-wintitleactions-6.2.diff
 */
#define BAR_WINTITLEACTIONS_PATCH BAR_AWESOMEBAR_PATCH || BAR_TABGROUPS_PATCH || BAR_FLEXWINTITLE_PATCH

/***
 * Other patches
 */

/* Adds a window task switcher toggled using alt-tab.
 * https://dwm.suckless.org/patches/alt-tab/
 */
#define ALT_TAB_PATCH 0

/* All floating windows are centered, like the center patch, but without a rule.
 * The center patch takes precedence over this patch.
 * This patch interferes with the center transient windows patches.
 * https://dwm.suckless.org/patches/alwayscenter/
 */
#define ALWAYSCENTER_PATCH 0

/* This patch allows windows to be resized with its aspect ratio remaining constant.
 * https://dwm.suckless.org/patches/aspectresize/
 */
#define ASPECTRESIZE_PATCH 0

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

/* This patch will make dwm run "~/.local/share/dwm/autostart_blocking.sh" and
 * "~/.local/share/dwm/autostart.sh &" before entering the handler loop. One or
 * both of these files can be ommited. Note the path inside .local/share rather
 * than the original ~/.dwm folder.
 * https://dwm.suckless.org/patches/autostart/
 */
#define AUTOSTART_PATCH 0

/* By default, windows that are not visible when requesting a resize/move will not
 * get resized/moved. With this patch, they will.
 * https://dwm.suckless.org/patches/autoresize/
 */
#define AUTORESIZE_PATCH 0

/* This patch adds proper support for Right-To-Left languages. (such as Farsi, Arabic or Hebrew).
 *
 * You need to uncomment the corresponding lines in config.mk to use the -lfribidi library
 * when including this patch.
 *
 * This patch depends on the following additional library:
 *    - fribidi
 *
 * https://dwm.suckless.org/patches/bidi/
 */
#define BIDI_PATCH 0

/* This patch adds a client rule option to allow the border width to be specified on a per
 * client basis.
 *
 * Example rule:
 *    RULE(.class = "Gimp", .bw = 0)
 *
 * https://dwm.suckless.org/patches/borderrule/
 */
#define BORDER_RULE_PATCH 0

/* This patch adds an iscentered rule to automatically center clients on the current monitor.
 * This patch takes precedence over centeredwindowname, alwayscenter and fancybar patches.
 * https://dwm.suckless.org/patches/center/
 */
#define CENTER_PATCH 0

/* A transient window is one that is meant to be short lived and is usually raised by a
 * parent window. Such windows are typically dialog boxes and the like.
 * It should be noted that in dwm transient windows are not subject to normal client rules
 * and they are always floating by default.
 * This patch centers transient windows on the screen like the center patch does. Note that
 * the 6.2 center patch piggy-backed on the updatewindowtype function to ensure that all
 * dialog boxes were centered, transient or not. This function was removed in relation to
 * adding wintype as a client rule filter, hence this no longer works out of the box. This
 * patch restores previous behaviour with the center patch.
 */
#define CENTER_TRANSIENT_WINDOWS_PATCH 0

/* As above, except that the transient window is centered within the position of the parent
 * window, rather than at the center of the screen. This takes precedence over the above patch.
 */
#define CENTER_TRANSIENT_WINDOWS_BY_PARENT_PATCH 0

/* This patch provides the ability to assign different weights to clients in their
 * respective stack in tiled layout.
 * https://dwm.suckless.org/patches/cfacts/
 */
#define CFACTS_PATCH 0

/* This patch allows color attributes to be set through the command line.
 * https://dwm.suckless.org/patches/cmdcustomize/
 */
#define CMDCUSTOMIZE_PATCH 0

/* This patch tweaks the tagging interface so that you can select multiple tags for tag
 * or view by pressing all the right keys as a combo. For example to view tags 1 and 3,
 * hold MOD and then press and hold 1 and 3 together.
 * https://dwm.suckless.org/patches/combo/
 */
#define COMBO_PATCH 0

/* Allow dwm to execute commands from autostart array in your config.h file. When dwm exits
 * then all processes from autostart array will be killed.
 * https://dwm.suckless.org/patches/cool_autostart/
 */
#define COOL_AUTOSTART_PATCH 0

/* The cyclelayouts patch lets you cycle through all your layouts.
 * https://dwm.suckless.org/patches/cyclelayouts/
 */
#define CYCLELAYOUTS_PATCH 0

/* Make dwm respect _MOTIF_WM_HINTS property, and not draw borders around windows requesting
 * for it. Some applications use this property to notify window managers to not draw window
 * decorations.
 * Not respecting this property leads to issues with applications that draw their own borders,
 * like chromium (with "Use system title bar and borders" turned off) or vlc in fullscreen mode.
 * https://dwm.suckless.org/patches/decoration_hints/
 */
#define DECORATION_HINTS_PATCH 0

/* This feature distributes all clients on the current monitor evenly across all tags.
 * It is a variant of the reorganizetags patch.
 * https://dwm.suckless.org/patches/reorganizetags/
 */
#define DISTRIBUTETAGS_PATCH 0

/* By default dwm will terminate on color allocation failure and the behaviour is intended to
 * catch and inform the user of color configuration issues.
 *
 * Some patches like status2d and xresources / xrdb can change colours during runtime, which
 * means that if a color can't be allocated at this time then the window manager will abruptly
 * terminate.
 *
 * This patch will ignore color allocation failures and continue on as normal. The effect of
 * this is that the existing color, that was supposed to be replaced, will remain as-is.
 */
#define DO_NOT_DIE_ON_COLOR_ALLOCATION_FAILURE_PATCH 0

/* Similarly to the dragmfact patch this allows you to click and drag clients to change the
 * cfact to adjust the client's size in the stack. This patch depends on the cfacts patch.
 */
#define DRAGCFACT_PATCH 0

/* This patch lets you resize the split in the tile layout (i.e. modify mfact) by holding
 * the modkey and dragging the mouse.
 * This patch can be a bit wonky with other layouts, but generally works.
 * https://dwm.suckless.org/patches/dragmfact/
 */
#define DRAGMFACT_PATCH 0

/* Simple dwmc client using a fork of fsignal to communicate with dwm.
 * To use this either copy the patch/dwmc shell script to somewhere in your path or
 * uncomment the following line in Makefile:
 *    #cp -f patch/dwmc ${DESTDIR}${PREFIX}/bin
 * http://dwm.suckless.org/patches/dwmc/
 */
#define DWMC_PATCH 0

/* This patch allows no tag at all to be selected. The result is that dwm will start with
 * no tag selected and when you start a client with no tag rule and no tag selected then
 * it will be opened on the first tag.
 * https://dwm.suckless.org/patches/emptyview/
 */
#define EMPTYVIEW_PATCH 0

/* This patch allows the user to change size and placement of floating windows using only the
 * keyboard. It also allows for temporary vertical and horizontal extension of windows similar
 * to other WMs fill command.
 * https://dwm.suckless.org/patches/exresize/
 */
#define EXRESIZE_PATCH 0

/* Only allow clients to "fullscreen" into the space currently given to them.
 * As an example, this will allow you to view a fullscreen video in your browser on
 * one half of the screen, while having the other half available for other tasks.
 * This patch takes precedence over the fakefullscreen client patch below.
 * https://dwm.suckless.org/patches/fakefullscreen/
 */
#define FAKEFULLSCREEN_PATCH 0

/* Similarly to the fakefullscreen patch this patch only allows clients to "fullscreen" into
 * the space currently given to them.
 * The "twist" with this patch is that fake fullscreen can be toggled on a per client basis
 * rather than applying to all clients globally.
 * Also see the selectivefakefullscreen option that adds a rule option to enabled this on client
 * startup.
 */
#define FAKEFULLSCREEN_CLIENT_PATCH 0

/* This patch adds a float rule allowing the size and position of floating windows to be specified
 * It also allows the size and position of floating windows to be controlled similar to the
 * exresize, moveresize, and moveplace patches.
 * The size and position can be specified using absolute, relative or fixed co-ordinates and
 * https://github.com/bakkeby/patches/wiki/floatpos/
 */
#define FLOATPOS_PATCH 0

/* Add-on functionality for the above: make the float positions respect outer (vanity)gaps. */
#define FLOATPOS_RESPECT_GAPS_PATCH 0

/* This patch provides the ability to focus the tag on the immediate left or right of the
 * currently focused tag. It also allows to send the focused window either on the left or
 * the right tag.
 * http://dwm.suckless.org/patches/focusadjacenttag/
 */
#define FOCUSADJACENTTAG_PATCH 0

/* Allows focusing on clients based on direction (up, down, left, right) instead of client order.
 * https://github.com/bakkeby/patches/wiki/focusdir/
 */
#define FOCUSDIR_PATCH 0

/* When changing tags, closing windows or moving clients out of view then focus will revert to the
 * client window that remains under the mouse cursor rather than the most recently focused window.
 * https://github.com/bakkeby/patches/wiki/focusfollowmouse
 */
#define FOCUSFOLLOWMOUSE_PATCH 0

/* A simple patch that just puts focus back to the master client.
 * https://dwm.suckless.org/patches/focusmaster/
 */
#define FOCUSMASTER_PATCH 0

/* A variant of the focusmaster patch that additionally allows the focus to be returned to the
 * previously focused client
 * https://dwm.suckless.org/patches/focusmaster/
 */
#define FOCUSMASTER_RETURN_PATCH 0

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

/* By default, dwm responds to _NET_ACTIVE_WINDOW client messages by setting
 * the urgency bit on the named window. This patch activates the window instead.
 * https://dwm.suckless.org/patches/focusonnetactive/
 */
#define FOCUSONNETACTIVE_PATCH 0

/* Send "fake signals" to dwm for handling, using xsetroot. This will not conflict with the
 * status bar, which also is managed using xsetroot.
 * Also see the dwmc patch, which takes precedence over this patch.
 * https://dwm.suckless.org/patches/fsignal/
 */
#define FSIGNAL_PATCH 0

/* Applies the monocle layout with the focused client on top and hides the bar. When pressed
 * again it shows the bar and restores the layout that was active before going fullscreen.
 * https://dwm.suckless.org/patches/fullscreen/
 */
#define FULLSCREEN_PATCH 0

/* This patch provides a keybinding to rotate all clients in the currently selected
 * area (master or stack) without affecting the other area.
 * https://dwm.suckless.org/patches/inplacerotate/
 */
#define INPLACEROTATE_PATCH 0

/* This patch lets you define custom insets from each edge of the screen. One use case would be
 * to arrange space for an external bar.
 * https://dwm.suckless.org/patches/insets/
 */
#define INSETS_PATCH 0

/* This patch (v1.5.7) implements inter-process communication through a UNIX socket for dwm. This
 * allows for the window manager to be queried for information, e.g. listen for events such as tag
 * or layout changes, as well as send commands to control the window manager via other programs.
 *
 * You need to uncomment the corresponding lines in config.mk to use the -lyajl library
 * when including this patch.
 * This patch depends on the following additional library:
 *    - yajl
 *
 * https://github.com/mihirlad55/dwm-ipc
 * https://dwm.suckless.org/patches/ipc/
 */
#define IPC_PATCH 0

/* Adds rule option for clients to avoid accidental termination by killclient for sticky windows.
 * https://dwm.suckless.org/patches/ispermanent/
 */
#define ISPERMANENT_PATCH 0

/* This patch adds key modes (like in vim or emacs) where chains of keyboard shortcuts
 * can be performed.
 * https://dwm.suckless.org/patches/keymodes/
 */
#define KEYMODES_PATCH 0

/* This patch adds a keybinding to kills all visible clients that are not selected.
 * https://dwm.suckless.org/patches/killunsel/
 */
#define KILLUNSEL_PATCH 0

/* This changes the window manager name to LG3d instead of dwm as a workaround for Java
 * applications that assume that the window manager is using window reparenting.
 * Refer to the ISSUES secton of the dwm man page for more details.
 */
#define LG3D_PATCH 0

/* By default in dwm it is possible to make an application fullscreen, then use
 * the focusstack keybindings to focus on other windows beneath the current window.
 * It is also possible to spawn new windows (e.g. a terminal) that end up getting
 * focus while the previous window remains in fullscreen. This patch ensures that
 * in such scenarios the previous window loses fullscreen.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-losefullscreen-6.2.diff
 */
#define LOSEFULLSCREEN_PATCH 0

/* This patch adds helper functions for maximizing, horizontally and vertically, floating
 * windows using keybindings.
 * https://dwm.suckless.org/patches/maximize/
 */
#define MAXIMIZE_PATCH 0

/* Control Music Player Daemon via keybinds.
 * You need to uncomment the corresponding line in config.mk to use the -lmpdclient library
 * when including this patch.
 * This patch depends on the following additional library:
 *    - libmpdclient
 * https://dwm.suckless.org/patches/mpdcontrol/
 */
#define MPDCONTROL_PATCH 0

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

/* Makes a window floating and 1/3rd the height and 1/3rd the width of the screen and is
 * positioned in either the center or one of the 8 cardinal directions depending on which
 * key is pressed.
 * https://dwm.suckless.org/patches/moveplace/
 */
#define MOVEPLACE_PATCH 0

/* This patch allows you to move and resize dwm's clients using keyboard bindings.
 * https://dwm.suckless.org/patches/moveresize/
 */
#define MOVERESIZE_PATCH 0

/* This patch allows you to move clients around in the stack and swap them with the master.
 * https://dwm.suckless.org/patches/movestack/
 */
#define MOVESTACK_PATCH 0

/* This patch allows you to change the names of tags during runtime.
 *
 * This is a bespoke version implemented specifically in relation to tagicons, which is integrated
 * into dwm-flexipatch. By default it uses dmenu to retrieve the new name, but this can be
 * customised via config along with the maximum text length and the format string.
 *
 * Special behaviour:
 *    - if more than one tag is selected then the name change applies to all selected tags
 *    - if tagicons is configured to have unique tags per monitor then the change only applies
 *      for the current monitor
 *    - the name change applies to the tag set that is active for the current tag:
 *       * if used in combination with BAR_ALTTAGSDECORATION_PATCH and there are clients on the
 *         given tag then the name change only applies to the ALT_TAGS_DECORATION tag set
 *       * if used in combination with the BAR_ALTERNATIVE_TAGS_PATCH and alternative tags are
 *         shown then the name change only applies to the ALTERNATIVE_TAGS tag set
 *       * if used in combination with both then BAR_ALTTAGSDECORATION_PATCH takes precedence
 *       * otherwise the name change applies to the DEFAULT_TAGS tag set
 *
 * https://dwm.suckless.org/patches/nametag/
 */
#define NAMETAG_PATCH 0

/* Variant of the above which prepends the tag number to the given string.
 * The toggle does nothing on its own and need to be enabled in combination with the above. */
#define NAMETAG_PREPEND_PATCH 0

/* Adds support for the _NET_CLIENT_LIST_STACKING atom, needed by certain applications like the
 * Zoom video conferencing application.
 * https://github.com/bakkeby/patches/wiki/netclientliststacking/
 */
#define NET_CLIENT_LIST_STACKING_PATCH 0

/* Removes the border when there is only one window visible.
 * https://dwm.suckless.org/patches/noborder/
 */
#define NOBORDER_PATCH 0

/* Enable modifying or removing dmenu in config.def.h which resulted previously in a
 * compilation error because two lines of code hardcode dmenu into dwm.
 * https://dwm.suckless.org/patches/nodmenu/
 */
#define NODMENU_PATCH 0

/* This patch allows for toggleable client button bindings that have no modifiers.
 * This can, for example, allow you to move or resize using the mouse alone without holding
 * down a modifier key. This can be practical if you have extra buttons on your mouse.
 * While you can use button bindings with no modifiers without this patch in a bare dwm,
 * those buttons are then unavailable for use within the application itself so being able to
 * toggle these on and off can be necessary in certain situations (e.g. being able to use
 * back and forward buttons in a browser).

 * Example bindings:
 *     { ClkClientWin,              0,              Button8,        movemouse,      {0} },
 *     { ClkClientWin,              0,              Button9,        resizemouse,    {0} },
 */
#define NO_MOD_BUTTONS_PATCH 0

/* When terminals have transparency then their borders also become transparent.
 * This patch ensures that borders have no transparency. Note that this patch is
 * only relevant if you are not using the alpha patch.
 * https://github.com/szatanjl/dwm/commit/1529909466206016f2101457bbf37c67195714c8
 * https://dwm.suckless.org/patches/alpha/dwm-fixborders-6.2.diff
 */
#define NO_TRANSPARENT_BORDERS_PATCH 0

/* Port of InstantWM's on_empty_keys functionality allowing keybindings that apply only when
 * a tag is empty. An example use case is being able to launch applications with first hand
 * keys like "f" to launch firefox.
 *
 * https://github.com/instantOS/instantWM/
 * https://github.com/bakkeby/dwm-flexipatch/issues/51
 */
#define ON_EMPTY_KEYS_PATCH 0

/* Minor patch that prevents more than one rule being matched for a given client. */
#define ONLY_ONE_RULE_MATCH_PATCH 0

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

/* Option to enable gaps on a per tag basis rather than globally.
 * Depends on both pertag and vanitygaps patches being enabled.
 */
#define PERTAG_VANITYGAPS_PATCH 0

/* This patch allows configuring vanity gaps on a per-monitor basis rather than
 * all monitors (default).
 */
#define PERMON_VANITYGAPS_PATCH 0

/* This controls whether or not to also store bar position on a per
 * tag basis, or leave it as one bar per monitor.
 */
#define PERTAGBAR_PATCH 0

/* Similar to the focusdir patch this patch allow users to move a window in any direction
 * in the tiled stack (up, down, left, right).
 * https://github.com/bakkeby/patches/wiki/placedir
 */
#define PLACEDIR_PATCH 0

/* This patch lets you change the position of a client in the stack using the mouse.
 * https://github.com/bakkeby/patches/wiki/placemouse
 */
#define PLACEMOUSE_PATCH 0

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

/* Variant of the named scratchpads patch allowing scratch keys to be added or removed
 * on demand, allowing multiple scratchpad windows to be toggled into and out of view
 * in unison, as well as including multi-monitor support.
 *
 * https://github.com/bakkeby/patches/wiki/renamedscratchpads
 */
#define RENAMED_SCRATCHPADS_PATCH 0

/* Renamed scratchpads option to auto-hide scratchpads when moving to a different tag.
 * This behaviour is similar to that of the (multiple) scratchpads patch. */
#define RENAMED_SCRATCHPADS_AUTO_HIDE_PATCH 0

/* Shifts all clients per tag to leftmost unoccupied tags.
 *
 * For example, if clients A, B, C are tagged on tags 1, 5, 9 respectively, when
 * this function is called, they will now be on 1, 2, and 3. The focused client
 * will also remain focused.
 *
 * Clients on multiple tags will be treated as if they only were only on their
 * leftmost tag, and will be reduced to one tag after the operation is complete.
 * https://dwm.suckless.org/patches/reorganizetags/
 */
#define REORGANIZETAGS_PATCH 0

/* By default, windows only resize from the bottom right corner. With this
 * patch the mouse is warped to the nearest corner and you resize from there.
 * https://dwm.suckless.org/patches/resizecorners/
 */
#define RESIZECORNERS_PATCH 0

/* Practically the same as resizecorners, but the cursor does not warp to corners.
 * This takes precedence over the resizecorners patch.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-resizepoint-6.2.diff
 */
#define RESIZEPOINT_PATCH 0

/* Adds a keyboard shortcut to restart dwm or alternatively by using kill -HUP dwmpid.
 * Additionally dwm can quit cleanly by using kill -TERM dwmpid.
 * https://dwm.suckless.org/patches/restartsig/
 */
#define RESTARTSIG_PATCH 0

/* Adds rio-like drawing to resize the selected client.
 * This depends on an external tool slop being installed.
 * This patch was backported from instantWM.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-riodraw-6.2.diff
 */
#define RIODRAW_PATCH 0

/* This patch let's you rotate through the stack using keyboard shortcuts.
 * https://dwm.suckless.org/patches/rotatestack/
 */
#define ROTATESTACK_PATCH 0

/* This patch adds rounded corners to client windows in dwm.
 * You need to uncomment the corresponding line in config.mk to use the -lXext library
 * when including this patch. You will also want to set "borderpx = 0;" in your config.h.
 * https://github.com/mitchweaver/suckless/blob/master/dwm/patches/mitch-06-rounded_corners-f04cac6d6e39cd9e3fc4fae526e3d1e8df5e34b2.patch
 */
#define ROUNDED_CORNERS_PATCH 0

/* This patch saves size and position of every floating window before it is forced
 * into tiled mode. If the window is made floating again then the old dimensions
 * will be restored.
 * https://dwm.suckless.org/patches/save_floats/
 */
#define SAVEFLOATS_PATCH 0

/* The scratchpad patch allows you to spawn or restore floating terminal windows.
 * It is typically useful when one need to do some short typing.
 *
 * Note that this patch changes TAGMASK to make room for special scratchpad tags,
 * so ~0 does more than select all tags with this patch. Code that relies on ~0 to
 * represent all tags should use ~SPTAGMASK instead.
 *
 * Upgraded to Christian Tenllado's multiple scratchpad version.
 * https://lists.suckless.org/hackers/2004/17205.html
 * https://dwm.suckless.org/patches/scratchpads/
 */
#define SCRATCHPADS_PATCH 0

/* Minor alteration of the above allowing clients to keep their size and position when shown */
#define SCRATCHPADS_KEEP_POSITION_AND_SIZE_PATCH 0

/* This alternative patch enables a scratchpad feature in dwm similar to the scratchpad
 * feature in i3wm.
 * https://github.com/GasparVardanyan/dwm-scratchpad
 */
#define SCRATCHPAD_ALT_1_PATCH 0

/* This patch persists some settings across window manager restarts. These include but are not
 * limited to:
 *    - client's assigned tag(s) on which monitor
 *    - the order of clients
 *    - nmaster
 *    - selected layout
 *    - plus various additions depending on what other patches are used
 *
 * The above is not persisted across reboots, however.
 */
#define SEAMLESS_RESTART_PATCH 0

/* As opposed to the original patch this only adds a rule option allowing fake fullscreen
 * to be enabled for applications when they start. This is intended to be used in combination
 * with the fakefullscreenclient patch and offers no practical functionality without it.
 * https://dwm.suckless.org/patches/selectivefakefullscreen/
 */
#define SELECTIVEFAKEFULLSCREEN_PATCH 0

/* Allows restarting dwm without the dependency of an external script.
 * https://dwm.suckless.org/patches/selfrestart/
 */
#define SELFRESTART_PATCH 0

/* Floating windows being sent to another monitor will be centered.
 * https://dwm.suckless.org/patches/sendmoncenter/
 */
#define SENDMON_CENTER_PATCH 0

/* This patch allow clients to keep focus when being sent to another monitor.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-sendmon_keepfocus-6.2.diff
 */
#define SENDMON_KEEPFOCUS_PATCH 0

/* This patch allows border pixels to be changed during runtime.
 * https://dwm.suckless.org/patches/setborderpx/
 */
#define SETBORDERPX_PATCH 0

/* Combines shifttag and shiftview. Basically moves the window to the next/prev tag and follows it.
 * Also see the focusadjacenttag patch.
 * https://dwm.suckless.org/patches/shift-tools/
 */
#define SHIFTBOTH_PATCH 0

/* Swaps all the clients on the current tag with all the client on the next/prev tag.
 * Depends on the swaptags patch.
 * https://dwm.suckless.org/patches/shift-tools/
 */
#define SHIFTSWAPTAGS_PATCH 0

/* Moves the current selected client to the adjacent tag.
 * Also see the focusadjacenttag patch.
 * https://dwm.suckless.org/patches/shift-tools/
 */
#define SHIFTTAG_PATCH 0

/* Moves the current selected client to the adjacent tag that has at least one client, if none
 * then it acts as shifttag.
 * https://dwm.suckless.org/patches/shift-tools/
 */
#define SHIFTTAGCLIENTS_PATCH 0

/* This patch adds keybindings for left and right circular shift through tags.
 * https://github.com/chau-bao-long/dotfiles/blob/master/suckless/dwm/shiftview.diff
 */
#define SHIFTVIEW_PATCH 0

/* This variant of the shiftview patch adds left and right circular shift through tags,
 * but skips tags where there are no clients.
 */
#define SHIFTVIEW_CLIENTS_PATCH 0

/* This patch makes dwm obey even "soft" sizehints for new clients. Any window
 * that requests a specific initial size will be floated and set to that size.
 * Unlike with "fixed size" windows, you are able to resize and/or unfloat these
 * windows freely - only the initial state is affected.
 * This version of the patch is honestly of limited utility since there are many
 * clients that will abuse it.
 * https://dwm.suckless.org/patches/sizehints/
 */
#define SIZEHINTS_PATCH 0

/* This patch makes dwm obey even "soft" sizehints for new clients. This ruled
 * version is essentially the same patch except it obeys the "isfloating" rule
 * if it is available in config.h for the given client.
 * https://dwm.suckless.org/patches/sizehints/
 */
#define SIZEHINTS_RULED_PATCH 0

/* This patch makes dwm obey even "soft" sizehints for new clients. The isfreesize
 * version is similar to the sizehints ruled patch except it allows you to specify
 * via client rules which clients this should apply to. Soft sizehints applies by
 * default to clients that are not ruled, and will be disabled by default for clients
 * that are.
 *
 * Example client rule enabling soft sizehints:
 *    - RULE(.wintype = WTYPE "DIALOG", .isfloating = 1, .isfreesize = 1)
 *
 * https://dwm.suckless.org/patches/sizehints/
 */
#define SIZEHINTS_ISFREESIZE_PATCH 0

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

/* This patch provides comprehensive utilities for managing the client stack, providing
 * keyboard shortcuts for focusing or placing a client at specific positions in the stack.
 * Note that the default keybindings for this patch have been changed in dwm-flexipatch
 * due to the many conflicts with other patches. As it provides similar functionality to the
 * swapfocus patch it also uses the MOD+s shortcut to focus the previously selected client,
 * thus note a conflict between these two patches.
 * https://dwm.suckless.org/patches/stacker/
 */
#define STACKER_PATCH 0

/* Steam, and steam windows (games), trigger a ConfigureNotify request every time the window
 * gets focus. More so, the configure event passed along from Steam tends to have the wrong
 * x and y co-ordinates which can make the window, if floating, jump around the screen.
 *
 * This patch works around this age-old issue by ignoring the x and y co-ordinates for
 * ConfigureNotify requests relating to Steam windows.
 *
 * https://github.com/bakkeby/patches/wiki/steam
 */
#define STEAM_PATCH 0

/* Adds toggleable keyboard shortcut to make a client 'sticky', i.e. visible on all tags.
 * https://dwm.suckless.org/patches/sticky/
 */
#define STICKY_PATCH 0

/* This patch adds "window swallowing" to dwm as known from Plan 9's windowing system rio.
 * Clients marked with isterminal in config.h swallow a window opened by any child process,
 * e.g. running xclock in a terminal. Closing the xclock window restores the terminal window
 * in the current position.
 *
 * This patch depends on the following additional libraries:
 *    - libxcb
 *    - Xlib-libxcb
 *    - xcb-res
 *
 * You need to uncomment the corresponding line in config.mk to use the above libraries when
 * including this patch.
 *
 * https://dwm.suckless.org/patches/swallow/
 */
#define SWALLOW_PATCH 0

/* This patch depends on the pertag patch and makes it possible to switch focus with a single
 * shortcut (MOD+s) instead of having to think if you should use mod-j or mod-k for reaching
 * the previously used window.
 * https://dwm.suckless.org/patches/swapfocus/
 */
#define SWAPFOCUS_PATCH 0

/* This patch allows swapping the contents of the currently selected tag with another tag using
 * keyboard shortcuts.
 * https://dwm.suckless.org/patches/swaptags/
 */
#define SWAPTAGS_PATCH 0

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
 *   3 as 1, but closing that window reverts the view back to what it was previously (*)
 *   4 as 2, but closing that window reverts the view back to what it was previously (*)
 *
 * (*) except if the client has been moved between tags or to another monitor
 *
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-switchtag-6.2.diff
 * Also see https://dwm.suckless.org/patches/switchtotag
 */
#define SWITCHTAG_PATCH 0

/* This patch transforms the monocle layout into a "tabbed" layout if more than one window is
 * present on the monocle view. This patch has been added for demonstration purposes only and has
 * limited compatibility with other patches. It will conflict space-wise with a second bar.
 * Note that fancybar, awesomebar, bartabgroups and similar patches make the tab patch redundant.
 * https://dwm.suckless.org/patches/tab/
 */
#define TAB_PATCH 0

/* Adds keyboard shortcuts to move all (or only floating) windows from one tag to another.
 * https://dwm.suckless.org/patches/tagall/
 */
#define TAGALL_PATCH 0

/* This patch allows you to move all visible windows on a monitor to an adjacent monitor.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-tagallmon-6.2.diff
 */
#define TAGALLMON_PATCH 0

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
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-tagmonfixfs-6.2.diff
 */
#define TAGMONFIXFS_PATCH 0

/* Add functions and keybindings to tag a window to a desired tag on the next (right)
 * or previous (left) monitor from the currently selected monitor.
 * https://dwm.suckless.org/patches/tagothermonitor/
 */
#define TAGOTHERMONITOR_PATCH 0

/* This patch allows you to swap all visible windows on one monitor with those of an
 * adjacent monitor.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-tagswapmon-6.2.diff
 */
#define TAGSWAPMON_PATCH 0

/* Sync tag actions across all monitors.
 * This is comparable to a sort of pseudo-desktop environment.
 * Also refer to the desktop patch:
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-desktop-6.3.diff
 */
#define TAGSYNC_PATCH 0

/* This patch can be useful to the touchpad users because it allows to
 * resize windows using Mod + two-finger scroll. It is useful when
 * two-finger scrolling is configured in libinput.
 * https://dwm.suckless.org/patches/tapresize/
 */
#define TAPRESIZE_PATCH 0

/* This patch allows you to toggle fullscreen on and off using a single shortcut key.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-togglefullscreen-6.2.diff
 */
#define TOGGLEFULLSCREEN_PATCH 0

/* This patch allows for the bar position (top or bottom) to be toggled during runtime.
 * https://dwm.suckless.org/patches/toggletopbar/
 */
#define TOGGLETOPBAR_PATCH 0

/* Minor patch that lets you use the same keyboard shortcut to toggle to the previous layout if the
 * designated layout is already active.
 *
 * This allows you to use e.g. MOD+m to change to the monocle layout and use the same keybinding to
 * toggle back to what it was previously. The default behaviour in dwm forces you to use either
 * MOD+space or MOD+t to change back to tiled layout.
 *
 * https://github.com/bakkeby/patches/wiki/togglelayout
 */

#define TOGGLELAYOUT_PATCH 0

/* Minor patch that lets you use the same keyboard shortcut to toggle to the previous tag if the
 * designated tag is already active.
 *
 * This allows you to use e.g. MOD+4 to quickly view the 4th tag and use the same keybinding to
 * toggle back to what it was previously. The default behaviour in dwm forces you to use either
 * MOD+tab or MOD+1 to change back to the previous tag.
 *
 * Idea ref.
 * https://www.reddit.com/r/suckless/comments/ik27vd/key_toggle_between_next_and_previous_tag_dwm/
 * https://github.com/bakkeby/patches/wiki/toggletag
 */
#define TOGGLETAG_PATCH 0

/* Lets you transfer the currently focused client between the master and stack area
 * while increasing or decreasing the master area (nmaster) accordingly.
 * https://dwm.suckless.org/patches/transfer/
 */
#define TRANSFER_PATCH 0

/* Lets you transfer all clients between the master and stack area
 * while increasing or decreasing the master area (nmaster) accordingly.
 * https://dwm.suckless.org/patches/transfer/
 */
#define TRANSFER_ALL_PATCH 0

/* This patch resets isfloating on any visible windows that have it set.
 * Optionally also applies a layout.
 * https://dwm.suckless.org/patches/unfloatvisible/
 */
#define UNFLOATVISIBLE_PATCH 0

/* This patch adds a client rule that allows for windows that do not specify the override-redirect
 * to not be managed by the window manager. This can be useful for external bars, widgets,
 * launchers, docks, desktop icons and more.
 * https://github.com/bakkeby/patches/wiki/unmanaged
 */
#define UNMANAGED_PATCH 0

/* This patch adds configurable gaps between windows differentiating between outer, inner,
 * horizontal and vertical gaps.
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-vanitygaps-6.2.diff
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-cfacts-vanitygaps-6.2.diff
 */
#define VANITYGAPS_PATCH 0

/* This patch adds outer gaps for the monocle layout.
 * Most gaps patches tries to avoid gaps on the monocle layout, as it is often used as a
 * fullscreen mode, hence this is enabled separately from the main vanitygaps patch.
 */
#define VANITYGAPS_MONOCLE_PATCH 0

/* By default MOD+Tab will take the user back to the previous tag only. If the user keeps
 * using MOD+Tab then the view will switch back and forth between the current and previous tag.
 * This patch allows dwm to keep a longer history of previous tag changes such that MOD+Tab can
 * be pressed multiple times to go further back to earlier tag selections.
 *
 * The number of history elements is defined by the NUMVIEWHIST macro in dwm.c and defaults to
 * the number of tags in the system.
 */
#define VIEW_HISTORY_PATCH 0

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
 * https://github.com/bakkeby/patches/blob/master/dwm/dwm-windowrolerule-6.2.diff
 */
#define WINDOWROLERULE_PATCH 0

/* The winview patch allows switching the view to that of a given client from the all-window
 * view (Mod-0) using a keyboard shortcut.
 * http://dwm.suckless.org/patches/winview/
 */
#define WINVIEW_PATCH 0

/* Remember keyboard layout per client.
 * It is recommended that you configure xkb before using this patch as described in
 * https://www.x.org/archive/X11R7.5/doc/input/XKB-Config.html
 * https://dwm.suckless.org/patches/xkb/
 */
#define XKB_PATCH 0

/* Allows dwm to read colors from xrdb (.Xresources) during runtime. Compatible with
 * the float border color, awesomebar, urgentborder and titlecolor patches.
 * https://dwm.suckless.org/patches/xrdb/
 */
#define XRDB_PATCH 0

/* Simple patch that allows floating windows to be zoomed into the master stack position.
 * https://www.reddit.com/r/suckless/comments/ie5fe3/zoomfloating_my_own_simple_original_patch/
 */
#define ZOOMFLOATING_PATCH 0

/* The zoomswap patch allows a master and a stack window to swap places
 * rather than every window on the screen changing position.
 * https://dwm.suckless.org/patches/zoomswap/
 */
#define ZOOMSWAP_PATCH 0

/**
 * Layouts
 */

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
