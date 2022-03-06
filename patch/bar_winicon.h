#include <Imlib2.h>
#include <limits.h>
#include <stdint.h>

static Picture drw_picture_create_resized(Drw *drw, char *src, unsigned int src_w, unsigned int src_h, unsigned int dst_w, unsigned int dst_h);
static void drw_pic(Drw *drw, int x, int y, unsigned int w, unsigned int h, Picture pic);
static Picture geticonprop(Window w, unsigned int *icw, unsigned int *ich);
static void freeicon(Client *c);
static void updateicon(Client *c);
