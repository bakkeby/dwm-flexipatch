# tiagoprn's custom dwm-flexipatch build

## PRE-REQUISITES

- I customize the dwm status bar using [my fork of dwmblocks](https://github.com/tiagoprn/dwmblocks), which is in turn a fork of lukesmith's fork of it. The scripts that dwmblocks triggers are at `$HOME/apps/scripts/bin/statusbar`.

## INSTRUCTIONS
- Edit **only** the files with the `*.def.h` extension. The `.h` only files hold a copy of the previous configuration, and are not versioned.
- After finished editing, run `build.sh`. **This will override the `.h` files** with the new (versioned) content.


## ABOUT THE CURRENT ACTIVE PATCHES

- To get a list, run: `list-active-patches.sh`.

- Here is an updated list of all active patches, and their respective configuration on `config.def.h`:

---

### CODES:
[#] Are not referenced on config.def.h
[x] Are referenced on config.def.h, but have NO configuration on config.def.h
[C] Have customization options/bindings on config.def.h
[E] Excluir um dos 2 patches - configuração conflita com outro patch

---

### ACTIVE PATCHES:
[#] AUTORESIZE_PATCH
[#] BAR_HIDEVACANTTAGS_PATCH
[#] BAR_IGNORE_XFT_ERRORS_WHEN_DRAWING_TEXT_PATCH
[x] BAR_LTSYMBOL_PATCH
[x] BAR_PADDING_PATCH
[#] BAR_STATUSCOLORS_PATCH
[x] BAR_STATUSPADDING_PATCH
[#] BAR_STATUS_PATCH
[#] BAR_SYSTRAY_PATCH
[#] BAR_TAGS_PATCH
[#] BAR_TITLE_LEFT_PAD_PATCH
[x] BAR_WINTITLE_PATCH
[x] BSTACK_LAYOUT
[x] CENTEREDMASTER_LAYOUT
[#] CENTER_TRANSIENT_WINDOWS_BY_PARENT_PATCH
[x] COMBO_PATCH
[C] CYCLELAYOUTS_PATCH
[x] DECK_LAYOUT
[C] DECORATION_HINTS_PATCH
[x] FIBONACCI_SPIRAL_LAYOUT
[#] FOCUSONNETACTIVE_PATCH
[C] FULLSCREEN_PATCH
[C] MAXIMIZE_PATCH
[x] MONOCLE_LAYOUT
[C] MOVERESIZE_PATCH
[C] MOVESTACK_PATCH
[#] NET_CLIENT_LIST_STACKING_PATCH
[C] ONLYQUITONEMPTY_PATCH
[C] PUSH_PATCH
[#] SAVEFLOATS_PATCH
[C] SCRATCHPADS_PATCH
[C] SETBORDERPX_PATCH
[C] SHIFTVIEW_PATCH
[C] STACKER_PATCH
[#] STEAM_PATCH
[C] STICKY_PATCH
[C] SWALLOW_PATCH
[#] TAGINTOSTACK_ALLMASTER_PATCH
[#] TILE_LAYOUT
[C] VANITYGAPS_PATCH
[#] VIEWONTAG_PATCH
[#] WINDOWROLERULE_PATCH
[C] XRDB_PATCH (`~/.Xresources`)
[#] ZOOMSWAP_PATCH

---
