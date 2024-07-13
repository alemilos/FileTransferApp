#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

/* Operations */
#define READ 'r'
#define WRITE 'w'
#define LIST 'l'

#define DIRACC 0770
#define FILEACC 0660

/* Statuses */
#define OK 200
#define CREATED 201
#define BADREQ 400
#define NOTFOUND 404
#define SERVERERROR 500

#define LOCKERR 501
#define STATERR 502

/* Constants types */
#define ULONG_SLEN 19
#define PERM_LEN 9

/* Bufsizes */
#define BUFSIZE 1024
#define MAX_NAME_L 256

/* Macro Permissions  */
// 4 r
// 2 w
// 1 x

/*
Example of PERMS MACRO
b = 765
b = 7 6 5 (root user other)
b = 111 110 101

PERM((b)>>6) --> 111
PERM((b)>>3) --> 111 110
PERM((b)) --> 111 110 101

111 110 101 & 000 000 100 -> 100 -> r
111 110 101 & 000 000 010 -> 000 -> -
111 110 101 & 000 000 001 -> 001 -> x
*/
#define PERM(b)                                                                \
  (((b) & 4) ? 'r' : '-'), (((b) & 2) ? 'w' : '-'), (((b) & 1) ? 'x' : '-')

#define PERMS(b) PERM((b) >> 6), PERM((b) >> 3), PERM(b)

void *memcheck(const char *name, void *mem);

void *xmalloc(size_t size);

void *xrealloc(void *old, size_t size);

/* Wrapper of strdup -> Allocate and return string */
void *xstrdup(const char *s);

/* Simulate the mkdir -p command to create nested directories from the path
   Return 0 on success,
   -1 on failure. */
int mkdir_p(char *path);

/* Simulate the ls -la command and fill a buffer with the output .
   Returns 0 on success.
   1 on failure.
*/
int ls_la(char *path, char **buffer);