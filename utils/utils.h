#include <stdlib.h>

void *memcheck(const char *name, void *mem);

void *xmalloc(size_t size);

void *xrealloc(void *old, size_t size);

/* Wrapper of strdup -> Allocate and return string */
void *xstrdup(const char *s);