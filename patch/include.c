#if ALPHA_PATCH
#include "alpha.c"
#endif

#if ATTACHABOVE_PATCH || ATTACHASIDE_PATCH || ATTACHBELOW_PATCH || ATTACHBOTTOM_PATCH
#include "attachx.c"
#endif

#if AUTOSTART_PATCH
#include "autostart.c"
#endif

#if CYCLELAYOUTS_PATCH
#include "cyclelayouts.c"
#endif // CYCLELAYOUTS_PATCH

#if PERTAG_PATCH
#include "pertag.c"
#endif

#if ROTATESTACK_PATCH
#include "rotatestack.c"
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

#if ZOOMSWAP_PATCH
#include "zoomswap.c"
#endif
