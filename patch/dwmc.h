typedef struct {
	const char * sig;
	void (*func)(const Arg *);
} Signal;

static void setlayoutex(const Arg *arg);
static void viewex(const Arg *arg);
static void viewallex(const Arg *arg);
static void toggleviewex(const Arg *arg);
static void tagex(const Arg *arg);
static void toggletagex(const Arg *arg);
static void tagallex(const Arg *arg);
static int fake_signal(void);

