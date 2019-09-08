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
#if CFACTS_PATCH
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc, float *mf, float *sf);
#else
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
#endif // CFACTS_PATCH
static void setgaps(int oh, int ov, int ih, int iv);