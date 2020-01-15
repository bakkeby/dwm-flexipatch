/* Key binding functions */
static void defaultgaps(const Arg *arg);
static void incrgaps(const Arg *arg);
static void incrigaps(const Arg *arg);
static void incrogaps(const Arg *arg);
static void incrohgaps(const Arg *arg);
static void incrovgaps(const Arg *arg);
static void incrihgaps(const Arg *arg);
static void incrivgaps(const Arg *arg);
static void togglegaps(const Arg *arg);

/* Internals */
#if CENTEREDFLOATINGMASTER_LAYOUT || COLUMNS_LAYOUT || DECK_LAYOUT || FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT || GAPPLESSGRID_LAYOUT || HORIZGRID_LAYOUT || BSTACK_LAYOUT || BSTACKHORIZ_LAYOUT || GRIDMODE_LAYOUT || FLEXTILE_DELUXE_LAYOUT || TILE_LAYOUT
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
#endif // CENTEREDFLOATINGMASTER_LAYOUT || DECK_LAYOUT || FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT || GAPPLESSGRID_LAYOUT || HORIZGRID_LAYOUT || BSTACK_LAYOUT || BSTACKHORIZ_LAYOUT || GRIDMODE_LAYOUT || FLEXTILE_DELUXE_LAYOUT || TILE_LAYOUT
static void setgaps(int oh, int ov, int ih, int iv);