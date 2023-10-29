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
#if DRAGMFACT_PATCH || CENTEREDMASTER_LAYOUT || CENTEREDFLOATINGMASTER_LAYOUT || COLUMNS_LAYOUT || DECK_LAYOUT || FIBONACCI_DWINDLE_LAYOUT || FIBONACCI_SPIRAL_LAYOUT || GAPPLESSGRID_LAYOUT || NROWGRID_LAYOUT || HORIZGRID_LAYOUT || BSTACK_LAYOUT || BSTACKHORIZ_LAYOUT || GRIDMODE_LAYOUT || FLEXTILE_DELUXE_LAYOUT || TILE_LAYOUT || (VANITYGAPS_MONOCLE_PATCH && MONOCLE_LAYOUT)
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
#endif
static void setgaps(int oh, int ov, int ih, int iv);
#if IPC_PATCH || DWMC_PATCH
static void setgapsex(const Arg *arg);
#endif // IPC_PATCH | DWMC_PATCH

