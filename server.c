#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils/utils.h"

int aflag, pflag, dflag;

const int MAX_QUEUE = 10;

/* Check if the filepath exists, otherwise create it
   The function return 0 on success
   1 on failure  */
int get_or_create_ft_root_directory(char *filepath, FILE **dir) { return 0; }

int main(int argc, char **argv) {

  int opt;
  int server_port;
  char *server_address = NULL;
  char *ft_root_directory = NULL;

  while ((opt = getopt(argc, argv, "a:p:dh")) != EOF) {
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
      ft_root_directory = xstrdup(optarg);
      dflag = 1;
      break;

    case 'h':
    default:
      printf("usage: %s [-a address] [-p port]\n", argv[0]);
      exit(EXIT_FAILURE);
      break;
    }
  }

  if (!aflag || !pflag) {
    printf("usage: %s [-a address] [-p port]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sd;
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  };

  struct sockaddr_in server;

  server.sin_addr.s_addr = inet_addr(server_address);
  server.sin_family = AF_INET;
  server.sin_port = htons(server_port);

  if (bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(sd, MAX_QUEUE) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on %s:%d\n", server_address, server_port);

  int socket_client;
  struct sockaddr client;
  socklen_t size_sockaddr = (socklen_t)sizeof(struct sockaddr);

  while ((socket_client = accept(sd, &client, &size_sockaddr)) >= 0) {
    printf("The socket client: %d\n", socket_client);
  }

  printf("Server closed.\n");

  /*
   ********************************************************************************
   ******************************** I want to break FREE.
   ********************************************************************************
   */
  if (server_address != NULL) {
    free(server_address);
  }
  if (ft_root_directory != NULL) {
    free(ft_root_directory);
  }
}
