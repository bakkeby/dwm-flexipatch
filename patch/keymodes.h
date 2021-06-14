#define COMMANDMODE             1
#define INSERTMODE              2

typedef struct {
	unsigned int mod[4];
	KeySym keysym[4];
	void (*func)(const Arg *);
	const Arg arg;
} Command;

static void clearcmd(const Arg *arg);
static void grabkeys(void);
static int isprotodel(Client *c);
static void keypress(XEvent *e);
static void onlyclient(const Arg *arg);
static void setkeymode(const Arg *arg);

/* variables */
static unsigned int cmdmod[4];
static unsigned int keymode = INSERTMODE;
static KeySym cmdkeysym[4];

