#include <time.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XInput2.h>

static void genericevent(XEvent *e);
static void hidecursor(const Arg *arg);
static unsigned long long now(void);
static void showcursor(const Arg *arg);
