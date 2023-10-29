/* See LICENSE file for copyright and license details. */

#ifndef MAX
#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#endif
#ifndef MIN
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#endif
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

#ifdef _DEBUG
#define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG(...)
#endif

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);

