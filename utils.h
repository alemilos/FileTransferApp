#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

/* Operations */
#define READ 'r'
#define WRITE 'w'
#define LIST 'l'

#define FULLACCESS 0777

/* Statuses */
#define OK 200
#define CREATED 201
#define SERVERERROR 500
#define NOTFOUND 404
#define BADREQ 400

#define BUFSIZE 1024

void *memcheck(const char *name, void *mem);

void *xmalloc(size_t size);

void *xrealloc(void *old, size_t size);

/* Wrapper of strdup -> Allocate and return string */
void *xstrdup(const char *s);

/* Simulate the mkdir -p command to create nested directories from the path
   Return 0 on success,
   1 on failure. */
int mkdir_r(char *path);

/* Extract the filename from the path
   e.g: d1/d2/.../dn/file
   will extract the filename from the path
   and assign it to 'file'
   */
void file_from_path(char *path, char **filename);

void list_files(char *path);