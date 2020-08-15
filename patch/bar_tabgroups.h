/* Bartabgroups properties, you can override these in your config.h if you want. */
#ifndef BARTAB_BORDERS
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#endif
#ifndef BARTAB_TAGSINDICATOR
#define BARTAB_TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#endif
#ifndef BARTAB_TAGSPX
#define BARTAB_TAGSPX 5        // # pixels for tag grid boxes
#endif
#ifndef BARTAB_TAGSROWS
#define BARTAB_TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
#endif
#ifndef BARTAB_SHOWFLOATING
#define BARTAB_SHOWFLOATING 0  // whether to show titles for floating windows, hidden clients are always shown
#endif

static int width_bartabgroups(Bar *bar, BarWidthArg *a);
static int draw_bartabgroups(Bar *bar, BarDrawArg *a);
static int click_bartabgroups(Bar *bar, Arg *arg, BarClickArg *a);

static void bartabdraw(Monitor *m, Client *c, int unused, int x, int w, int groupactive, Arg *arg);
static void bartabclick(Monitor *m, Client *c, int passx, int x, int w, int unused, Arg *arg);
static void bartabcalculate(Monitor *m, int offx, int w, int passx, void(*tabfn)(Monitor *, Client *, int, int, int, int, Arg *arg), Arg *arg);