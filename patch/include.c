/* Patches */
#if ALPHA_PATCH
#include "alpha.c"
#endif
#if ALTERNATIVE_TAGS_PATCH
#include "alternativetags.c"
#endif
#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH
#include "attachx.c"
#endif
#if AUTOSTART_PATCH
#include "autostart.c"
#endif
#if AWESOMEBAR_PATCH
#include "awesomebar.c"
#endif
#if CFACTS_PATCH
#include "cfacts.c"
#endif
#if CMDCUSTOMIZE_PATCH
#include "cmdcustomize.c"
#endif
#if COMBO_PATCH
#include "combo.c"
#endif
#if CYCLELAYOUTS_PATCH
#include "cyclelayouts.c"
#endif
#if DWMC_PATCH
#include "dwmc.c"
#endif
#if EWMHTAGS_PATCH
#include "ewmhtags.c"
#endif
#if FOCUSADJACENTTAG_PATCH
#include "focusadjacenttag.c"
#endif
#if FOCUSURGENT_PATCH
#include "focusurgent.c"
#endif
#if FULLSCREEN_PATCH
#include "fullscreen.c"
#endif
#if HOLDBAR_PATCH
#include "holdbar.c"
#endif
#if EXRESIZE_PATCH
#include "exresize.c"
#endif
#if KILLUNSEL_PATCH
#include "killunsel.c"
#endif
#if MAXIMIZE_PATCH
#include "maximize.c"
#endif
#if MDPCONTROL_PATCH
#include "mdpcontrol.c"
#endif
#if MOVESTACK_PATCH
#include "movestack.c"
#endif
#if PERTAG_PATCH
#include "pertag.c"
#endif
#if PUSH_NO_MASTER_PATCH
#include "push_no_master.c"
#elif PUSH_PATCH
#include "push.c"
#endif
#if RESTARTSIG_PATCH
#include "restartsig.c"
#endif
#if ROTATESTACK_PATCH
#include "rotatestack.c"
#endif
#if SCRATCHPAD_PATCH
#include "scratchpad.c"
#endif
#if SELFRESTART_PATCH
#include "selfrestart.c"
#endif
#if SETBORDERPX_PATCH
#include "setborderpx.c"
#endif
#if SORTSCREENS_PATCH
#ifdef XINERAMA
#include "sortscreens.c"
#endif // XINERAMA
#endif
#if STICKY_PATCH
#include "sticky.c"
#endif
#if SYSTRAY_PATCH
#include "systray.c"
#endif
#if SWALLOW_PATCH
#include "swallow.c"
#endif // SWALLOW_PATCH
#if SWITCHCOL_PATCH
#include "switchcol.c"
#endif
#if TAGALL_PATCH
#include "tagall.c"
#endif
#if TAGALLMON_PATCH
#include "tagallmon.c"
#endif
#if TAGGRID_PATCH
#include "taggrid.c"
#endif
#if TAGSWAPMON_PATCH
#include "tagswapmon.c"
#endif
#if TOGGLEFULLSCREEN_PATCH
#include "togglefullscreen.c"
#endif
#if UNFLOATVISIBLE_PATCH
#include "unfloatvisible.c"
#endif
#if VANITYGAPS_PATCH
#include "vanitygaps.c"
#endif
#if WARP_PATCH
#include "warp.c"
#endif
#if WINVIEW_PATCH
#include "winview.c"
#endif
#if ZOOMSWAP_PATCH
#include "zoomswap.c"
#endif
#if XRDB_PATCH
#include "xrdb.c"
#endif
/* Layouts */
#if BSTACK_LAYOUT
#include "bstack.c"
#endif
#if BSTACKHORIZ_LAYOUT
#include "bstackhoriz.c"
#endif
#if CENTEREDMASTER_LAYOUT
#include "centeredmaster.c"
#endif
#if CENTEREDFLOATINGMASTER_LAYOUT
#include "centeredfloatingmaster.c"
#endif
#if COLUMNS_LAYOUT
#include "columns.c"
#endif
#if DECK_LAYOUT
#include "deck.c"
#endif
#if FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT
#include "fibonacci.c"
#endif
#if FLEXTILE_DELUXE_LAYOUT
#include "flextile-deluxe.c"
#endif
#if GAPPLESSGRID_LAYOUT
#include "gapplessgrid.c"
#endif
#if GRIDMODE_LAYOUT
#include "grid.c"
#endif
#if HORIZGRID_LAYOUT
#include "horizgrid.c"
#endif
#if MONOCLE_LAYOUT
#include "monocle.c"
#endif
#if NROWGRID_LAYOUT
#include "nrowgrid.c"
#endif
#if TILE_LAYOUT
#include "tile.c"
#endif
