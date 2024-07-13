#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

//////////////////////////////////////////////////////////////////////////////////////////
// MEM UTILS
//////////////////////////////////////////////////////////////////////////////////////////
void *memcheck(const char *name, void *mem) {
  if (!mem) {
    perror(name);
    exit(EXIT_FAILURE);
  }
  return mem;
}

void *xmalloc(size_t size) { return memcheck("malloc", malloc(size)); }

void *xrealloc(void *old, size_t size) {
  return memcheck("realloc", realloc(old, size));
}

void *xstrdup(const char *s) { return memcheck("strdup", strdup(s)); }

//////////////////////////////////////////////////////////////////////////////////////////
// FS Utils
//////////////////////////////////////////////////////////////////////////////////////////

int mkdir_p(char *path) {
  if (path == NULL) {
    return 0;
  }
  printf("PATH: %s\n", path);
  if (access(path, R_OK) != 0) {
    // Path do not exist, create it.
    char *pathcpy = xstrdup(path);
    if (pathcpy == NULL) {
      return -1;
    }

    char *leftpath = xmalloc(strlen(path) + 1);
    leftpath[0] = '\0';

    char *tok = path;
    while ((tok = strtok(tok, "/")) != NULL) {
      if (strlen(leftpath) > 0) {
        sprintf(leftpath + strlen(leftpath), "/%s", tok);
      } else {
        sprintf(leftpath, "%s", tok);
      }
      if (access(leftpath, R_OK) != 0) {
        if (mkdir(leftpath, DIRACC) < 0) {
          return -1;
        }
      }
      tok = NULL;
    }

    // Cleanup
    free(pathcpy);
    free(leftpath);
  }

  return 0;
}

void list_files(char *path) {}