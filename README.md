This side project has a different take on dwm patching. It uses preprocessor directives to decide whether or not to include a patch during build time; essentially this means that this build, for better or worse, contains both the patched _and_ the original code. The aim being that you can select which patches to include and the build will contain that code and nothing more.

For example to include the alpha patch then you would only need to flip this setting from 0 to 1 in [patches.h](https://github.com/bakkeby/dwm-flexipatch/blob/master/patches.h):
```c
#define ALPHA_PATCH 1
```

Refer to [https://dwm.suckless.org/](https://dwm.suckless.org/) for details on the dwm window manager, how to install it and how it works.

---

### Changelog:

2019-09-05 - Alpha, systray and pertag patches added

### Patches included:

   - [alpha](https://dwm.suckless.org/patches/alpha/)
      - adds transparency for the status bar

   - [pertag](https://dwm.suckless.org/patches/pertag/)
      - adds nmaster, mfact, layouts and more per tag rather than per monitor

   - [systray](https://dwm.suckless.org/patches/systray/)
      - adds system tray in the status bar
