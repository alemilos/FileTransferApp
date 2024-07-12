#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 100

void mkdir_r(char *path) {
  // If path is NULL or empty return; (basecase)
  if (path == NULL) {
    return;
  }

  // Check if dir exists, if so go to the next token.
  char *tok = path;
  tok = strtok(tok, "/");

  printf("%s\n", tok);

  // If doesn't exist, create it.
}

int main() {
  char path[BUFSIZE];
  char filename[BUFSIZE];
  printf("Please type a path: ");
  scanf("%s", path);

  if (access(path, R_OK) == 0) {
    printf("Path already exists!\n");
    return 1;
  }

  // Remove the filename from path

  mkdir_r(path);
  int fd = open(filename, O_CREAT | O_RDWR, 0777);
  printf("Result: %d (0 means ok)\n", access(path, R_OK));
}