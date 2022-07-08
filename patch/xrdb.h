#include <X11/Xresource.h>

#define XRDB_LOAD_COLOR(R,V)    if (XrmGetResource(xrdb, R, NULL, &type, &value) == True) { \
                                  if (value.addr != NULL && strnlen(value.addr, 8) == 7 && value.addr[0] == '#') { \
                                    int i = 1; \
                                    for (; i <= 6; i++) { \
                                      if (value.addr[i] < '0') break; \
                                      if (value.addr[i] > '9' && value.addr[i] < 'A') break; \
                                      if (value.addr[i] > 'F' && value.addr[i] < 'a') break; \
                                      if (value.addr[i] > 'f') break; \
                                    } \
                                    if (i == 7) { \
                                      strncpy(V, value.addr, 7); \
                                      V[7] = '\0'; \
                                    } \
                                  } \
                                }

#if BAR_ALPHA_PATCH
#define XRDB_LOAD_FLOAT(R,V)    if (XrmGetResource(xrdb, R, NULL, &type, &value) == True) { \
                                  if (value.addr) \
                                    V = strtof(value.addr, NULL) * OPAQUE; \
                                }
#endif // BAR_ALPHA_PATCH

static void loadxrdb(void);
static void xrdb(const Arg *arg);


