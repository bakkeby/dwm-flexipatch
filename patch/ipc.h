#include <sys/epoll.h>

static int handlexevent(struct epoll_event *ev);
static void setlayoutsafe(const Arg *arg);
static void setupepoll(void);
static void setstatus(const Arg *arg);

