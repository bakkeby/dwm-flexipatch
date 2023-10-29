#define GETINC(X)               ((X) - 2000)
#define INC(X)                  ((X) + 2000)
#define ISINC(X)                ((X) > 1000 && (X) < 3000)
#define PREVSEL                 3000
#define MOD(N,M)                ((N)%(M) < 0 ? (N)%(M) + (M) : (N)%(M))
#define TRUNC(X,A,B)            (MAX((A), MIN((X), (B))))

static void focusstack(const Arg *arg);
static void pushstack(const Arg *arg);
static int stackpos(const Arg *arg);

