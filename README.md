This dwm 6.2 side project has a different take on dwm patching. It uses preprocessor directives to decide whether or not to include a patch during build time. Essentially this means that this build, for better or worse, contains both the patched _and_ the original code. The aim being that you can select which patches to include and the build will contain that code and nothing more.

For example to include the `alpha` patch then you would only need to flip this setting from 0 to 1 in [patches.h](https://github.com/bakkeby/dwm-flexipatch/blob/master/patches.h):
```c
#define ALPHA_PATCH 1
```

So if you have ever been curious about trying out dwm, but have been discouraged by manual patching, then this may be a good starting point to see what a "fully fledged" dwm can look like. Want to try out the `pertag` patch? Just flip a config and recompile. Once you have found out what works for you and what doesn't then you should be in a better position to choose patches should you want to start patching from scratch.

Refer to [https://dwm.suckless.org/](https://dwm.suckless.org/) for details on the dwm window manager, how to install it and how it works.

---

### Changelog:

2019-10-05 - Added killunsel, taggrid and hidevacanttags patches

2019-10-04 - Added maximize, movestack, monoclesymbol, noborder, tagall and tagintostack  patches

2019-10-03 - Added onlyquitonempty and switchcol patches

2019-10-02 - Added restartsig, emptyview, focusurgent and focusadjacenttag patches

2019-10-01 - Added leftlayout, fullscreen, holdbar and unfloatvisible patches

2019-09-30 - Replaced flextile with flextile-deluxe, refactored monitor rules to support predetermined layouts per tag

2019-09-15 - Added focusonclick, xrdb, viewontag, urgentborder and winview patches

2019-09-14 - Added setborderpx, selfrestart and push (no master variant), sticky and warp patches

2019-09-13 - Added titlecolor and push patches

2019-09-12 - Added activetagindicatorbar, alwaysfullscreen and autoresize patches

2019-09-11 - Added monitor rules, combo and ewmhtags patches

2019-09-10 - Minor tweaks to awesomebar patch (incl. alpha and systray compatibility). Added floatbordercolor patch.

2019-09-09 - Added deck, fibonacci (dwindle and spiral), gridmode, gapplessgrid, horizgrid, nrowgrid, centeredmaster and flextile layouts. Added alternativetags and awesomebar patches.

2019-09-08 - Added cfacts and vanitygaps patches, added bstack and bstackhoriz layouts

2019-09-07 - Added cyclelayouts, resizecorners, rotatestack, savefloats, statuspadding, switchtag, center and windowrolerule patches

2019-09-06 - Added attachabove, attachaside, attachbelow, attachbottom, autostart, fancybar, focusonnetactive and losefullscreen patches

2019-09-05 - Alpha, systray, togglefullscreen, tagallmon, tagmonfixfs, tagswapmon, pertag and zoomswap patches added

### Patches included:

   - [activetagindicatorbar](https://dwm.suckless.org/patches/activetagindicatorbar/)
      - this patch changes the rectangle indicating if a tag is used by a client into a bar above the tag name

   - [alpha](https://dwm.suckless.org/patches/alpha/)
      - adds transparency for the status bar

   - [alternativetags](https://dwm.suckless.org/patches/alternativetags/)
      - adds alternative tags which can be toggled on the fly for the sole purpose of providing visual aid

   - [alwaysfullscreen](https://dwm.suckless.org/patches/alwaysfullscreen/)
      - prevents the focus to drift from the active fullscreen client when using focusstack\(\)

   - [attachabove](https://dwm.suckless.org/patches/attachabove/)
      - new windows are placed above selected client

   - [attachaside](https://dwm.suckless.org/patches/attachaside/)
      - new windows are placed on top of the stack

   - [attachbelow](https://dwm.suckless.org/patches/attachbelow/)
      - new windows are placed below selected client

   - [attachbottom](https://dwm.suckless.org/patches/attachbottom/)
      - new windows are placed at the bottom of the stack

   - [autoresize](https://dwm.suckless.org/patches/autoresize/)
      - by default, windows that are not visible when requesting a resize/move will not get resized/moved, with this patch, however, they will

   - [autostart](https://dwm.suckless.org/patches/autostart/)
      - makes dwm run `~/.dwm/autostart_blocking.sh` and `~/.dwm/autostart.sh &` on startup

   - [awesomebar](https://dwm.suckless.org/patches/awesomebar/)
      - enhanced taskbar that allows focus / hiding / unhiding of windows by clicking on the status bar

   - [center](https://dwm.suckless.org/patches/center/)
      - adds an iscentered rule to automatically center clients on the current monitor

   - [cfacts](https://dwm.suckless.org/patches/cfacts/)
      - the cfacts patch provides the ability to assign different weights to clients in their respective stack in tiled layout

   - [combo](https://dwm.suckless.org/patches/combo/)
      - allows you to select multiple tags by pressing all the right keys as a combo, e.g. hold MOD and press and hold 1 and 3 together to view those two tags

   - [cyclelayouts](https://dwm.suckless.org/patches/cyclelayouts/)
      - lets you cycle through all your layouts

   - [emptyview](https://dwm.suckless.org/patches/emptyview/)
      - allows no tag at all to be selected
      - dwm will start with no tag selected and when a client with no tag rule is started and no tag is selected then it will be opened on the first tag

   - [ewmhtags](https://dwm.suckless.org/patches/ewmhtags/)
      - adds EWMH support for \_NET_NUMBER_OF_DESKTOPS, \_NET_CURRENT_DESKTOP, \_NET_DESKTOP_NAMES and \_NET_DESKTOP_VIEWPORT, which allows for compatibility with other bars and programs that request workspace information, e.g. polybar's xworkspaces module

   - [fancybar](https://dwm.suckless.org/patches/fancybar/)
      - shows the titles of all visible windows in the status bar

   - [floatbordercolor](https://dwm.suckless.org/patches/float_border_color/)
      - this patch allows a different border color to be chosen for floating windows

   - [focusadjacenttag](https://dwm.suckless.org/patches/focusadjacenttag/)
      - provides the ability to focus the tag on the immediate left or right of the currently focused tag
      - it also allows to send the focused window either on the left or the right tag

   - [focusonclick](https://dwm.suckless.org/patches/focusonclick/)
      - this patch makes you switch focus only by mouse click and not sloppy (focus follows mouse pointer)

   - [focusonnetactive](https://dwm.suckless.org/patches/focusonnetactive/)
      - by default, dwm responds to \_NET_ACTIVE_WINDOW client messages by setting the urgency bit on the named window
      - this patch activates the window instead

   - [focusurgent](https://dwm.suckless.org/patches/focusurgent/)
      - adds a keyboard shortcut to select the next window having the urgent flag regardless of the tag it is on

   - [fullscreen](https://dwm.suckless.org/patches/fullscreen/)
      - applies the monocle layout with the focused client on top and hides the bar
      - when pressed again it shows the bar and restores the layout that was active before going fullscreen

   - [hidevacanttags](https://dwm.suckless.org/patches/hide_vacant_tags/)
      - prevents dwm from drawing tags with no clients (i.e. vacant) on the bar

   - [holdbar](http://dwm.suckless.org/patches/holdbar/)
      - with this patch dwm's built-in status bar is only shown when HOLDKEY is pressed
      - additionally the bar will now overlay the display

   - [leftlayout](http://dwm.suckless.org/patches/leftlayout/)
      - moves the layout symbol in the status bar to the left hand side

   - [losefullscreen](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-losefullscreen-6.2.diff)
      - by default in dwm it is possible to make an application fullscreen, then use the focusstack keybindings to focus on other windows beneath the current window
      - it is also possible to spawn new windows (e.g. a terminal) that end up getting focus while the previous window remains in fullscreen
      - this patch ensures that in such scenarios the previous window loses fullscreen

   - [maximize](https://dwm.suckless.org/patches/maximize/)
      - adds helper functions for maximizing, horizontally and vertically, floating windows using keybindings

   - monitorrules
      - adds rules per monitor, e.g. have default layouts per monitor
      - the use case for this is if the second monitor is vertical (i.e. rotated) then you may want to use a different default layout for this monitor than what is used for the main monitor (for example normal vertical split for main monitor and horizontal split for the second)

   - [monoclesymbol](https://dwm.suckless.org/patches/monoclesymbol/)
      - always display the the monocle-symbol as defined in config.h if the monocle-layout is activated
      - do not display the number of open clients in the current tag

   - [movestack](https://dwm.suckless.org/patches/movestack/)
      - allows you to move clients around in the stack and swap them with the master

   - [noborder](https://dwm.suckless.org/patches/noborder/)
      - removes the border when there is only one window visible

   - [onlyquitonempty](https://dwm.suckless.org/patches/onlyquitonempty/)
      - makes it so dwm will only exit via quit() if no windows are open (in order to prevent accidental loss of work)

   - [pertag](https://dwm.suckless.org/patches/pertag/)
      - adds nmaster, mfact, layouts and more per tag rather than per monitor

   - [push](https://dwm.suckless.org/patches/push/)
      - this patch provides a way to move clients up and down inside the client list

   - [resizecorners](https://dwm.suckless.org/patches/resizecorners/)
      - by default, windows only resize from the bottom right corner
      - with this patch the mouse is warped to the nearest corner and you resize from there

   - [restartsig](https://dwm.suckless.org/patches/restartsig/)
      - adds a keyboard shortcut to restart dwm or alternatively by using kill -HUP dwmpid
      - additionally dwm can quit cleanly by using kill -TERM dwmpid

   - [rotatestack](https://dwm.suckless.org/patches/rotatestack/)
      - let's you rotate through the stack using keyboard shortcuts

   - [savefloats](https://dwm.suckless.org/patches/save_floats/)
      - saves size and position of every floating window before it is forced into tiled mode
      - if the window is made floating again then the old dimensions will be restored

   - [selfrestart](https://dwm.suckless.org/patches/selfrestart/)
      - restart dwm without the unnecessary dependency of an external script

   - [setborderpx](https://dwm.suckless.org/patches/statuspadding/)
      - this patch allows border pixels to be changed during runtime

   - [statuspadding](https://dwm.suckless.org/patches/statuspadding/)
      - adds configuration options for horizontal and vertical padding in the status bar

   - [sticky](https://dwm.suckless.org/patches/sticky/)
      - adds toggleable keyboard shortcut to make a client 'sticky', i.e. visible on all tags

   - [switchcol](https://dwm.suckless.org/patches/switchcol/)
      - allows you to switch focus between the master and stack columns using a single keybinding

   - [switchtag](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-switchtag-6.2.diff)
      - when an application opens on a specific tab this patch adds the option to also switch to that tag when the application starts

   - [systray](https://dwm.suckless.org/patches/systray/)
      - adds system tray in the status bar

   - [tagall](https://dwm.suckless.org/patches/tagall/)
      - adds keyboard shortcuts to move all (or only floating) windows from one tag to another

   - [tagallmon](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagallmon-6.2.diff)
      - move all visible windows to an adjacent monitor

   - [tagintostack](https://dwm.suckless.org/patches/tagintostack/)
      - makes new clients attach into the stack area when you toggle a new tag into view
      - this means your master area will remain unchanged when toggling views

   - [taggrid](https://dwm.suckless.org/patches/taggrid/)
      - adds an option to place tags in rows like in many other window managers

   - [tagmonfixfs](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagmonfixfs-6.2.diff)
      - allows moving a fullscreen window to another monitor while remaining in fullscreen

   - [tagswapmon](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagswapmon-6.2.diff)
      - swap all visible windows on one monitor with those of an adjacent monitor

   - [titlecolor](https://dwm.suckless.org/patches/titlecolor/)
      - adds a new color scheme used by the (selected) window title in the bar

   - [togglefullscreen](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-togglefullscreen-6.2.diff)
      - allows you to toggle fullscreen on and off using a single shortcut key

   - [unfloatvisible](https://dwm.suckless.org/patches/unfloatvisible/)
      - resets isfloating on any visible windows that have it set and optionally also applies a layout

   - [killunsel](https://dwm.suckless.org/patches/killunsel/)
      - kills all visible clients that are not selected (only the selected client will remain)

   - [urgentborder](https://dwm.suckless.org/patches/urgentborder/)
      - this patch makes "urgent" windows have different colors

   - [vanitygaps](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-vanitygaps-6.2.diff)
      - adds configurable gaps between windows differentiating between outer, inner, horizontal and vertical gaps

   - [viewontag](https://dwm.suckless.org/patches/viewontag/)
      - follow a window to the tag it is being moved to

   - [warp](https://dwm.suckless.org/patches/warp/)
      - warps the mouse cursor to the center of the currently focused window or screen when the mouse cursor is (a) on a different screen or (b) on top of a different window

   - [windowrolerule](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-windowrolerule-6.2.diff)
      - sometimes a single application opens different windows depending on the task at hand and this is often reflected in the WM_WINDOW_ROLE(STRING) x property
      - this patch adds the role field to the rule configuration so that one can differentiate between, say, Firefox "browser" vs "Preferences" vs "Manager" or Google-chrome "browser" vs "pop-up".

   - [winview](http://dwm.suckless.org/patches/winview/)
      - allows switching the view to that of a given client from the all-window view (Mod-0) using a keyboard shortcut

   - [xrdb](http://dwm.suckless.org/patches/xrdb/)
      - allows dwm to read colors from xrdb (.Xresources) during runtime

   - [zoomswap](https://dwm.suckless.org/patches/zoomswap/)
      - allows a master and a stack window to swap places rather than every window on the screen changing position

### Layouts included:

   - [bstack](https://dwm.suckless.org/patches/bottomstack/)
      - bottomstack layout

   - [bstackhoriz](https://dwm.suckless.org/patches/bottomstack/)
      - bottomstack horizontal layout

   - [centeredmaster](https://dwm.suckless.org/patches/centeredmaster/)
      - centeredmaster layout

   - [centeredfloatingmaster](https://dwm.suckless.org/patches/centeredmaster/)
      - centeredfloatingmaster layout

   - [deck](https://dwm.suckless.org/patches/deck/)
      - deck layout

   - [fibonacci](https://dwm.suckless.org/patches/fibonacci/)
      - fibonacci (dwindle and spiral) layouts

   - flextile-deluxe
      - a re-envisioned, flexible and over-the-top version of the original [flextile](https://dwm.suckless.org/patches/flextile/) patch supporting
         - multiple split layouts (horizontal, vertical, centered, floating, fixed)
         - tile arrangement on a per split basis (stack horizontally, stack vertically, grids, fibonacci)
         - pertag, cfacts, rmaster, vanitygaps compatibility
         - tile, deck, monocle, centeredmaster, bstack, bstackhoriz, gapplessgrid and more
      - this gives you a lot of versatility in terms of layout

   - [gapplessgrid](https://dwm.suckless.org/patches/gaplessgrid/)
      - gappless grid layout

   - [gridmode](https://dwm.suckless.org/patches/gridmode/)
      - gridmode (grid) layout

   - [horizgrid](https://dwm.suckless.org/patches/horizgrid/)
      - horizontal grid layout

   - [nrowgrid](https://dwm.suckless.org/patches/nrowgrid/)
      - nrowgrid layout, number of rows in grid controlled by nmaster
