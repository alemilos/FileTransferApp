#include <stdlib.h>

/* Operations */
#define READ 'r'
#define WRITE 'w'
#define LIST 'l'

#define FULLACCESS 0777

/* Statuses */
#define OK 200
#define SERVERERROR 500
#define NOTFOUND 404
#define BADREQ 400

#define BUFSIZE 1024

void *memcheck(const char *name, void *mem);

void *xmalloc(size_t size);

void *xrealloc(void *old, size_t size);

/* Wrapper of strdup -> Allocate and return string */
void *xstrdup(const char *s);

/* Simulate the mkdir -p command to create nested directories from the path */
void mkdir_r(char *path);