This side project has a different take on dwm patching. It uses preprocessor directives to decide whether or not to include a patch during build time. Essentially this means that this build, for better or worse, contains both the patched _and_ the original code. The aim being that you can select which patches to include and the build will contain that code and nothing more.

For example to include the alpha patch then you would only need to flip this setting from 0 to 1 in [patches.h](https://github.com/bakkeby/dwm-flexipatch/blob/master/patches.h):
```c
#define ALPHA_PATCH 1
```

Refer to [https://dwm.suckless.org/](https://dwm.suckless.org/) for details on the dwm window manager, how to install it and how it works.

---

### Changelog:

2019-09-06 - Added attachabove, attachaside, attachbelow and attachbottom patches

2019-09-05 - Alpha, systray, togglefullscreen, tagallmon, tagmonfixfs, tagswapmon, pertag and zoomswap patches added

### Patches included:

   - [alpha](https://dwm.suckless.org/patches/alpha/)
      - adds transparency for the status bar

   - [attachabove](https://dwm.suckless.org/patches/attachabove/)
      - new windows are placed above selected client

   - [attachaside](https://dwm.suckless.org/patches/attachaside/)
      - new windows are placed on top of the stack

   - [attachbelow](https://dwm.suckless.org/patches/attachbelow/)
      - new windows are placed below selected client

   - [attachbottom](https://dwm.suckless.org/patches/attachbottom/)
      - new windows are placed at the bottom of the stack

   - [pertag](https://dwm.suckless.org/patches/pertag/)
      - adds nmaster, mfact, layouts and more per tag rather than per monitor

   - [systray](https://dwm.suckless.org/patches/systray/)
      - adds system tray in the status bar

   - [tagallmon](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagallmon-6.2.diff)
      - move all visible windows to an adjacent monitor

   - [tagmonfixfs](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagmonfixfs-6.2.diff)
      - allows moving a fullscreen window to another monitor while remaining in fullscreen

   - [tagswapmon](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-tagswapmon-6.2.diff)
      - swap all visible windows on one monitor with those of an adjacent monitor

   - [togglefullscreen](https://github.com/bakkeby/dwm-vanitygaps/blob/master/patches/dwm-togglefullscreen-6.2.diff)
      - allows you to toggle fullscreen on and off using a single shortcut key

   - [zoomswap](https://dwm.suckless.org/patches/zoomswap/)
      - allows a master and a stack window to swap places rather than every window on the screen changing position
