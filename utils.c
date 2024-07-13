#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
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

int ls_la(char *path, char **buffer) {
  *buffer = NULL;

  DIR *dir = opendir(path);
  if (dir == NULL) {
    return -1;
  }

  struct dirent *d;
  while ((d = readdir(dir)) != NULL) {
    // Buffer for the complete line

    // Type and permissions
    char type = (d->d_type == DT_DIR) ? 'd' : '-';

    struct stat info;

    char *filepath = xmalloc(strlen(path) + strlen(d->d_name) + 2);
    sprintf(filepath, "%s/%s", path, d->d_name);

    if (stat(filepath, &info) < 0) {
      continue;
    }

    // Number of links
    unsigned long nlink = info.st_nlink;

    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);

    char uname[MAX_NAME_L];
    char gname[MAX_NAME_L];

    if (pw != NULL) {
      strncpy(uname, pw->pw_name, sizeof(uname) - 1);
    } else {
      snprintf(uname, MAX_NAME_L, "%u",
               info.st_uid); // Fallback to UID if username not found
    }

    if (gr != NULL) {
      strncpy(gname, gr->gr_name, sizeof(gname) - 1);
    } else {
      snprintf(gname, MAX_NAME_L, "%u",
               info.st_gid); // Fallback to GID if group name not found
    }

    // File size
    unsigned long size = info.st_size;

    // Last modification time
    char mtim[64];
    struct tm *timeinfo = localtime(&info.st_mtime);
    strftime(mtim, sizeof(mtim), "%b %d %H:%M", timeinfo);

    // Align the output
    // 10 are the permissions characters
    int toalloc = 1 + PERM_LEN + 1 + ULONG_SLEN + 1 + strlen(uname) + 1 +
                  strlen(gname) + 1 + ULONG_SLEN + 1 + strlen(mtim) + 1 +
                  strlen(d->d_name) + 1 + 1;

    if (!*buffer) {
      *buffer = xmalloc(toalloc);
      **buffer = 0;
    } else {
      *buffer = xrealloc(*buffer, strlen(*buffer) + toalloc);
    }

    sprintf(*buffer + strlen(*buffer),
            "%c%c%c%c%c%c%c%c%c%c %2lu %-8s %-8s %8lu %s %s\n", type,
            PERMS(info.st_mode), nlink, uname, gname, size, mtim, d->d_name);

    // Print the formatted line

    free(filepath);
  }

  if (closedir(dir) < 0) {
    perror("dir");
    return -1;
  }

  return 0;
}