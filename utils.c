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
void file_from_path(char *path, char **filename) {
  int start_ind = strlen(path);
  for (int i = start_ind; i > -1; i--) {
    start_ind = i;
    if (path[i] == '/') {
      break;
    }
  }

  size_t filename_len = strlen(path) - start_ind;
  *filename = xmalloc(filename_len);

  size_t ind = 0;
  for (size_t i = start_ind + 1; i < strlen(path); i++) {
    (*filename)[ind++] = path[i];
  }
  (*filename)[ind] = '\0';

  // Remove the filename from the path
  path[start_ind] = '\0';
}

int mkdir_r(char *path) {
  if (path == NULL) {
    return 1;
  }

  if (access(path, R_OK) != 0) {
    // Path Must be created

    char *pathcpy = xstrdup(path);
    char *filename = NULL;

    // Remove the filename from path
    file_from_path(path, &filename);
    printf("path: %s\nfilename: %s\n", path, filename);

    // If path is NULL or empty return (base case)

    char *leftpath = xmalloc(strlen(path) + 1);
    leftpath[0] = '\0';

    char *tok = path;
    while ((tok = strtok(tok, "/")) != NULL) {
      if (strlen(leftpath) > 0) {
        sprintf(leftpath + strlen(leftpath), "/%s", tok);
      } else {
        sprintf(leftpath, "%s", tok);
      }
      printf("Checking %s\n", leftpath);
      if (access(leftpath, R_OK) != 0) {
        mkdir(leftpath, FULLACCESS);
      }
      tok = NULL;
    }
    free(leftpath);

    int fd = open(pathcpy, O_CREAT | O_RDWR, FULLACCESS);

    path = strdup(pathcpy); // reset path to original
    free(pathcpy);
    free(filename);

    if (fd != -1) {
      close(fd);
    }

    return 0;
  };

  return 0;
}