static void flextile(Monitor *m);
static void getfactsforrange(Monitor *m, int an, int ai, int size, int *rest, float *fact);
static void mirrorlayout(const Arg *arg);
static void rotatelayoutaxis(const Arg *arg);
#if IPC_PATCH || DWMC_PATCH
static void setlayoutaxisex(const Arg *arg);
#endif // IPC_PATCH | DWMC_PATCH
static void incnstack(const Arg *arg);

/* Symbol handlers */
static void setflexsymbols(Monitor *m, unsigned int n);
static void monoclesymbols(Monitor *m, unsigned int n);
static void decksymbols(Monitor *m, unsigned int n);

/* Layout split */
static void layout_no_split(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical_dual_stack(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal_dual_stack(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_vertical(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_horizontal(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_floating_master(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_vertical_dual_stack_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_horizontal_dual_stack_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_vertical_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_split_centered_horizontal_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);
static void layout_floating_master_fixed(Monitor *m, int x, int y, int h, int w, int ih, int iv, int n);

/* Layout tile arrangements */
static void arrange_left_to_right(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_top_to_bottom(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_monocle(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_gapplessgrid(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_gapplessgrid_alt1(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_gapplessgrid_alt2(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_gridmode(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_horizgrid(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_dwindle(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_spiral(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);
static void arrange_tatami(Monitor *m, int ax, int ay, int ah, int aw, int ih, int iv, int n, int an, int ai);

/* Named flextile constants */
enum {
	LAYOUT,       // controls overall layout arrangement / split
	MASTER,       // indicates the tile arrangement for the master area
	STACK,        // indicates the tile arrangement for the stack area
	STACK2,       // indicates the tile arrangement for the secondary stack area
	LTAXIS_LAST,
};

/* Layout arrangements */
enum {
	NO_SPLIT,
	SPLIT_VERTICAL,                    // master stack vertical split
	SPLIT_HORIZONTAL,                  // master stack horizontal split
	SPLIT_CENTERED_VERTICAL,           // centered master vertical split
	SPLIT_CENTERED_HORIZONTAL,         // centered master horizontal split
	SPLIT_VERTICAL_DUAL_STACK,         // master stack vertical split with dual stack
	SPLIT_HORIZONTAL_DUAL_STACK,       // master stack vertical split with dual stack
	FLOATING_MASTER,                   // (fake) floating master
	SPLIT_VERTICAL_FIXED,              // master stack vertical fixed split
	SPLIT_HORIZONTAL_FIXED,            // master stack horizontal fixed split
	SPLIT_CENTERED_VERTICAL_FIXED,     // centered master vertical fixed split
	SPLIT_CENTERED_HORIZONTAL_FIXED,   // centered master horizontal fixed split
	SPLIT_VERTICAL_DUAL_STACK_FIXED,   // master stack vertical split with fixed dual stack
	SPLIT_HORIZONTAL_DUAL_STACK_FIXED, // master stack vertical split with fixed dual stack
	FLOATING_MASTER_FIXED,             // (fake) fixed floating master
	LAYOUT_LAST,
};

static char layoutsymb[] = {
	32,  // " ",
	124, // "|",
	61,  // "=",
	94,  // "^",
	126, // "~",
	58,  // ":",
	59,  // ";",
	43,  // "+",
	124, // "￤",
	61,  // "=",
	94,  // "^",
	126, // "~",
	58,  // ":",
	59,  // ";",
	43,  // "+",
};

/* Tile arrangements */
enum {
	TOP_TO_BOTTOM,     // clients are arranged vertically
	LEFT_TO_RIGHT,     // clients are arranged horizontally
	MONOCLE,           // clients are arranged in deck / monocle mode
	GAPPLESSGRID,      // clients are arranged in a gappless grid (original formula)
	GAPPLESSGRID_ALT1, // clients are arranged in a gappless grid (alt. 1, fills rows first)
	GAPPLESSGRID_ALT2, // clients are arranged in a gappless grid (alt. 2, fills columns first)
	GRIDMODE,          // clients are arranged in a grid
	HORIZGRID,         // clients are arranged in a horizontal grid
	DWINDLE,           // clients are arranged in fibonacci dwindle mode
	SPIRAL,            // clients are arranged in fibonacci spiral mode
	TATAMI,            // clients are arranged as tatami mats
	AXIS_LAST,
};

static char tilesymb[] = {
	61,  // "=",
	124, // "|",
	68,  // "D",
	71,  // "G",
	49,  // "1",
	50,  // "2"
	35,  // "#",
	126, // "~",
	92,  // "\\",
	64,  // "@",
	84,  // "T",
};

