#include <stdint.h>

static void persistmonitorstate(Monitor *m);
static int restoremonitorstate(Monitor *m);
static void persistclientstate(Client *c);
static int restoreclientstate(Client *c);
static void setmonitorfields(Monitor *m);
static int getmonitorfields(Monitor *m);
static void setmonitortags(Monitor *m);
static int getmonitortags(Monitor *m);
static void setclientfields(Client *c);
static int getclientfields(Client *c);
static void setclienttags(Client *c);
static int getclienttags(Client *c);
static int getlayoutindex(const Layout *layout);
#if SAVEFLOATS_PATCH
static void savewindowfloatposition(Client *c, Monitor *m);
static int restorewindowfloatposition(Client *c, Monitor *m);
#endif // SAVEFLOATS_PATCH
