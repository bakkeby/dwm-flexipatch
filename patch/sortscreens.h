#define RIGHTOF(a,b) (a.y_org > b.y_org) || ((a.y_org == b.y_org) && (a.x_org > b.x_org))

static void sortscreens(XineramaScreenInfo *screens, int n);

