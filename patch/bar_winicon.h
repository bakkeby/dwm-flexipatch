#include <Imlib2.h>
#include <limits.h>
#include <stdint.h>

void drw_img(Drw *drw, int x, int y, XImage *img, uint32_t *tmp);
static XImage *geticonprop(Window win);
static void freeicon(Client *c);
static void updateicon(Client *c);