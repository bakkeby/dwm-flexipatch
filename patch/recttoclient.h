#define INTERSECTC(X,Y,W,H,Z)   (MAX(0, MIN((X)+(W),(Z)->x+(Z)->w) - MAX((X),(Z)->x)) \
                               * MAX(0, MIN((Y)+(H),(Z)->y+(Z)->h) - MAX((Y),(Z)->y)))

static Client *recttoclient(int x, int y, int w, int h, int include_floating);
