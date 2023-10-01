#include <ctype.h> /* for making tab label lowercase, very tiny standard library */

static int width_taglabels(Bar *bar, BarArg *a);
static int draw_taglabels(Bar *bar, BarArg *a);
static int click_taglabels(Bar *bar, Arg *arg, BarArg *a);
static int hover_taglabels(Bar *bar, BarArg *a, XMotionEvent *ev);
