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

#if COMBO_PATCH
#include "combo.c"
#endif

#if CYCLELAYOUTS_PATCH
#include "cyclelayouts.c"
#endif

#if EWMHTAGS_PATCH
#include "ewmhtags.c"
#endif

#if PERTAG_PATCH
#include "pertag.c"
#endif

#if PUSH_PATCH
#include "push.c"
#endif

#if ROTATESTACK_PATCH
#include "rotatestack.c"
#endif

#if SELFRESTART_PATCH
#include "selfrestart.c"
#endif

#if SETBORDERPX_PATCH
#include "setborderpx.c"
#endif

#if SYSTRAY_PATCH
#include "systray.c"
#endif

#if TAGALLMON_PATCH
#include "tagallmon.c"
#endif

#if TAGSWAPMON_PATCH
#include "tagswapmon.c"
#endif

#if TOGGLEFULLSCREEN_PATCH
#include "togglefullscreen.c"
#endif

#if VANITYGAPS_PATCH
#include "vanitygaps.c"
#endif

#if ZOOMSWAP_PATCH
#include "zoomswap.c"
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

#if DECK_LAYOUT
#include "deck.c"
#endif

#if FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT
#include "fibonacci.c"
#endif

#if FLEXTILE_LAYOUT
#include "flextile.c"
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