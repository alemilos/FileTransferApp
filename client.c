#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils/utils.h"

int aflag, pflag, dflag;

int readflag, writeflag, listflag;

const int MAX_QUEUE = 10;

void write_to_server(char *localpath, char *remotepath) {
  // If localpath == NULL or does not exist in FileSystem,
  // perror();

  // If remotepath == NULL, use the localpath

  // Write the localpath file to the server.
}

void read_from_server(char *localpath, char *remotepath) {
  // If remotepath == NULL, perror()

  // If localpath does not exist in FileSystem, create it.

  // If localpath == NULL, use the remotepath

  // Read the remotepath file from the server and write it on localpath.
}

void ls_from_server(char *remotepath) {
  // If remotepath == NULL, perror()

  // Read the remotepath ls from the server()
}

int main(int argc, char **argv) {

  int opt;
  int server_port;
  char *server_address = NULL;

  while ((opt = getopt(argc, argv, "a:p:drwlhf:o:")) != EOF) {
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

    case 'r':
      readflag = 1;
      break;

    case 'w':
      writeflag = 1;
      break;

    case 'l':
      listflag = 1;
      break;

    case 'f':
      break;

    case 'o':
      break;

    case 'h':
    default:
      printf("usage: %s [-w | -r | -l] [-a address] [-p port] [-f path] [-o "
             "path]\n\nWhen performing a -w operation\n  -f specifies the "
             "local_path/filename_local (to be read)\n  -o specifies the "
             "remote_path/filename_remote (to write in)\n  NOTE: -o can be "
             "omitted, and -f "
             "will be "
             "used as the remote filepath.\n\nWhen performing a -r operation\n "
             " -f specifies the "
             "remote_path/filename_remote (to be read)\n  -o specifies the "
             "local_path/filename_local (to write in)\n  NOTE: -o can be "
             "omitted, and -f "
             "will be "
             "used as the local filepath.\n\nWhen performing a -l operation\n  "
             "-f specifies the "
             "remote_path (to be listed)\n",

             argv[0]);
      exit(EXIT_SUCCESS);
      break;
    }
  }

  // The operations are exclusive, only one can be performed at a time.
  int flag_op_count = writeflag + readflag + listflag;

  if (!aflag || !pflag || flag_op_count != 1) {
    printf("usage: %s [-w | -r | -l] [-a address] [-p port]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_addr.s_addr = inet_addr(server_address);
  server.sin_family = AF_INET;
  server.sin_port = htons(server_port);

  int sd;
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  };

  if (connect(sd, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  printf("Client  connected\n");

  /*
  ********************************************************************************
  ******************************** I want to break FREE.
  ********************************************************************************
  */
  if (server_address != NULL) {
    free(server_address);
  }
}
