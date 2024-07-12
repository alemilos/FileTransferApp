// #include <dirent.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <time.h>
// #include <unistd.h>

// void print_permissions(mode_t mode) {
//   printf("%c", S_ISDIR(mode) ? 'd' : '-');
//   printf("%c", (mode & S_IRUSR) ? 'r' : '-');
//   printf("%c", (mode & S_IWUSR) ? 'w' : '-');
//   printf("%c", (mode & S_IXUSR) ? 'x' : '-');
//   printf("%c", (mode & S_IRGRP) ? 'r' : '-');
//   printf("%c", (mode & S_IWGRP) ? 'w' : '-');
//   printf("%c", (mode & S_IXGRP) ? 'x' : '-');
//   printf("%c", (mode & S_IROTH) ? 'r' : '-');
//   printf("%c", (mode & S_IWOTH) ? 'w' : '-');
//   printf("%c", (mode & S_IXOTH) ? 'x' : '-');
// }

// void print_file_info(const char *name, const struct stat *sb) {
//   print_permissions(sb->st_mode);
//   printf(" %lu", sb->st_nlink);

//   printf(" %5ld", (long)sb->st_size);

//   char timebuf[80];
//   struct tm *tm_info = localtime(&sb->st_mtime);
//   strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
//   printf(" %s", timebuf);

//   printf(" %s\n", name);
// }

// void list_directory(const char *dirpath) {
//   DIR *dir = opendir(dirpath);
//   if (!dir) {
//     perror("opendir");
//     exit(EXIT_FAILURE);
//   }

//   struct dirent *entry;
//   while ((entry = readdir(dir)) != NULL) {
//     struct stat sb;
//     if (stat(entry->d_name, &sb) == -1) {
//       perror("stat");
//       continue;
//     }
//     print_file_info(entry->d_name, &sb);
//   }

//   closedir(dir);
// }

// int main(int argc, char *argv[]) {
//   const char *dirpath = ".";
//   if (argc > 1) {
//     dirpath = argv[1];
//   }

//   list_directory(dirpath);

//   return 0;
// }
