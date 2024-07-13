// #include <errno.h>
// #include <libgen.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/stat.h>

// int mkdir_p(char *path) {
//   if (path == NULL || strcmp(path, ".") == 0) {
//     printf("End of Path: %s\n", path);
//     return 0;
//   }

//   if (mkdir(path, 0775) < 0) {
//     if (errno == EEXIST) {
//       return 0; // Directory already exists
//     }
//     char *path_cpy = strdup(path);
//     char *prefixpath = dirname(path_cpy);
//     printf("pathcpy: %s\n", path_cpy);
//     printf("prefixpath: %s\n", prefixpath);

//     if (mkdir_p(prefixpath) == 0) {

//       printf("Path is:%s\n", path);
//       if (mkdir(path, 0775) == 0) {
//         free(path_cpy);
//         return 0;
//       }
//     }

//     free(path_cpy);
//     return -1;
//   }

//   return 0;
// }

// int main() {
//   char buffer[1024] = "a/b/c/d/e";

//   char *buf_cpy = strdup(buffer);
//   char *dirpath = dirname(buf_cpy);

//   if (mkdir_p(dirpath)) {
//     printf("Creating file: %s\n", buffer);

//     return 0;
//   } else {
//     perror("");
//     return 1;
//   }
// }
