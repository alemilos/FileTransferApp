#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

// #include "utils.h"

#define BUFSIZE 1024

void mkdir_r(char *path) {
  // If path is NULL or empty return (base case)
  if (path == NULL) {
    return;
  }

  char *leftpath = malloc(strlen(path) + 1);
  if (leftpath == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
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
      mkdir(leftpath, 0777);
    }
    tok = NULL;
  }
  free(leftpath);
}

void file_from_path(char *path, char **filename) {
  int start_ind = strlen(path);
  for (int i = start_ind; i > -1; i--) {
    start_ind = i;
    if (path[i] == '/') {
      break;
    }
  }

  size_t filename_len = strlen(path) - start_ind;
  *filename = malloc(filename_len);
  if (*filename == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  size_t ind = 0;
  for (size_t i = start_ind + 1; i < strlen(path); i++) {
    (*filename)[ind++] = path[i];
  }
  (*filename)[ind] = '\0';

  // Remove the filename from the path
  path[start_ind] = '\0';
}

int main() {
  char *path;
  size_t len = 0;
  char *filename = NULL;

  path = strdup("ale/milos/mariokart");

  // Duplicate the path for further usage
  char *pathcpy = strdup(path);
  if (pathcpy == NULL) {
    perror("strdup");
    free(path);
    exit(EXIT_FAILURE);
  }

  if (access(path, R_OK) == 0) {
    printf("Path already exists!\n");
    free(path);
    free(pathcpy);
    return 1;
  }

  // Remove the filename from path
  file_from_path(path, &filename);
  mkdir_r(path);

  printf("pathcpy: %s\n", pathcpy);
  int fd = open(pathcpy, O_CREAT | O_RDWR, 0777);
  printf("Result: %d (0 means ok)\n", access(path, R_OK));

  // Free allocated memory
  free(path);
  free(pathcpy);
  free(filename);
  if (fd != -1) {
    close(fd);
  }
  return 0;
}
