#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include "server.h"
#include "utils.h"

int aflag, pflag, dflag;
char *ft_root_dir_pathname = NULL;

int main(int argc, char **argv) {

  int opt;
  int server_port;
  DIR *ft_root_dir = NULL;
  char *server_address = NULL;

  while ((opt = getopt(argc, argv, "a:p:d:h")) != EOF) {
    char *eptr;
    switch (opt) {
    case 'a':
      server_address = xstrdup(optarg);
      aflag = 1;
      break;

    case 'p':
      // Check positivity and make sure sscanf doesn't fail
      if (isdigit(optarg[0]) != 0 && sscanf(optarg, "%u", &server_port) != 0) {
        pflag = 1;
      }
      break;

    case 'd':
      ft_root_dir_pathname = xstrdup(optarg);
      dflag = 1;
      break;

    case 'h':
    default:
      printf("usage: %s [-a address] [-p port]\n", argv[0]);
      exit(EXIT_FAILURE);
      break;
    }
  }

  if (!aflag || !pflag || !dflag) {
    printf("usage: %s [-a address] [-p port] [-d ft_root_dir]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (get_or_create_ft_root_directory(ft_root_dir_pathname, &ft_root_dir)) {
    perror("cannot set ft_root_dir");
    exit(EXIT_FAILURE);
  } else {
    printf("%s set as Root Directory\n", ft_root_dir_pathname);
  }

  int server_sd;
  if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  };

  struct sockaddr_in server;

  server.sin_addr.s_addr = inet_addr(server_address);
  server.sin_family = AF_INET;
  server.sin_port = htons(server_port);

  if (bind(server_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(server_sd, MAX_QUEUE) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on %s:%d\n", server_address, server_port);

  int client_sd;
  struct sockaddr client;
  socklen_t size_sockaddr = (socklen_t)sizeof(struct sockaddr);

  while ((client_sd = accept(server_sd, &client, &size_sockaddr)) >= 0) {
    printf("Socket %d connected\n", client_sd);
    handle_connection(client_sd);
  }

  printf("Server closed.\n");
  close(server_sd);

  /*
   ********************************************************************************
   ******************************** I want to break FREE.
   ********************************************************************************
   */
  if (server_address != NULL) {
    free(server_address);
  }
  if (ft_root_dir_pathname != NULL) {
    free(ft_root_dir_pathname);
  }
  if (ft_root_dir != NULL) {
    free(ft_root_dir);
  }
}

int get_or_create_ft_root_directory(char *path, DIR **dir) {
  // If path == NULL || "" -> Invalid path, return 1
  if (path == NULL || dir == NULL) {
    return 1;
  }

  // Try to open directory
  *dir = opendir(path);

  if (ENOENT == errno) {
    // Try to create it
    if (mkdir(path, FULLACCESS) == 0) {
      *dir = opendir(path);
      return 0;
    } else {
      return 1;
    }
  }

  return 0;
}

void handle_connection(int client_sd) {
  char op;
  recv(client_sd, &op, sizeof(char), 0);

  // 1. Find the OP that client wants to perform.
  // char op = request[0];
  if (op != READ && op != WRITE && op != LIST) {
    notify_status(client_sd, BADREQ);
    return;
  } else {
    notify_status(client_sd, OK);
  }

  char filepath[BUFSIZE];
  recv(client_sd, filepath, BUFSIZE, 0);

  // 3. Call the handler for that OP.
  if (op == READ) {
    handle_read(client_sd, filepath);
  }
  if (op == WRITE) {
    // handle_write(client_sd, );
  }
  if (op == LIST) {
  }
}

void handle_read(int client_sd, char *read_path) {
  char fullpath[BUFSIZE];
  sprintf(fullpath, "%s/%s", ft_root_dir_pathname, read_path);

  printf("The full path %s\n", fullpath);

  if (access(fullpath, R_OK) == 0) {
    notify_status(client_sd, OK);

    struct stat obj;
    stat(fullpath, &obj);

    int fd = open(fullpath, O_RDONLY);
    int f_size = obj.st_size;

    // Tell the client the file size
    write(client_sd, &f_size, sizeof(int));

    int bytes_sent;
    while ((bytes_sent = sendfile(client_sd, fd, NULL, BUFSIZE)) > 0) {
      printf("Sent %d bytes\n", bytes_sent);
    }

  } else {
    // the file doesn't exist or cannot be accessed
    notify_status(client_sd, NOTFOUND);
  }
}

void notify_status(int client_sd, int status) {
  write(client_sd, &status, sizeof(int));
}
