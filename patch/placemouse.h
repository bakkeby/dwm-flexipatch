#define INTERSECTC(x,y,w,h,z)   (MAX(0, MIN((x)+(w),(z)->x+(z)->w) - MAX((x),(z)->x)) \
                               * MAX(0, MIN((y)+(h),(z)->y+(z)->h) - MAX((y),(z)->y)))

static void moveorplace(const Arg *arg);
static void placemouse(const Arg *arg);
static Client *recttoclient(int x, int y, int w, int h);

