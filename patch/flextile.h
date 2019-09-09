static void flextile(Monitor *m);
static void mirrorlayout(const Arg *arg);
static void rotatelayoutaxis(const Arg *arg);
static void setflexlayout(const Arg *arg);
static void setflexsymbols(Monitor *m, unsigned int n);

/* Named flextile constants */
#define LAYOUT 0
#define MASTER 1
#define STACK 2
#define SPLIT_VERTICAL 1   // master stack vertical split
#define SPLIT_HORIZONTAL 2 // master stack horizontal split
#define SPLIT_CENTERED_V 3 // centered master vertical split
#define SPLIT_CENTERED_H 4 // centered master horizontal split
#define LEFT_TO_RIGHT 1    // clients are stacked horizontally
#define TOP_TO_BOTTOM 2    // clients are stacked vertically
#define MONOCLE 3          // clients are stacked in deck / monocle mode
#define GRID 4             // clients are stacked in grid mode