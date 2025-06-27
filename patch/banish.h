#include <time.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XInput2.h>

#define INTERSECTC(X,Y,W,H,Z)   (MAX(0, MIN((X)+(W),(Z)->x+(Z)->w) - MAX((X),(Z)->x)) \
                               * MAX(0, MIN((Y)+(H),(Z)->y+(Z)->h) - MAX((Y),(Z)->y)))

static void genericevent(XEvent *e);
static void hidecursor(const Arg *arg);
static unsigned long long now(void);
static Client *recttoclient(int x, int y, int w, int h, int include_floating);
static void showcursor(const Arg *arg);
