#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "utils.h"

/* Flags */
int aflag, pflag, dflag;
int readflag, writeflag, listflag;
int fflag, oflag;

int main(int argc, char **argv) {

  int opt;
  int server_port;
  char *server_address = NULL;

  char *f_arg_path = NULL;
  char *o_arg_path = NULL;

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
      fflag = 1;
      f_arg_path = xstrdup(optarg);
      break;

    case 'o':
      oflag = 1;
      o_arg_path = xstrdup(optarg);
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
  if (writeflag && !fflag) {
    printf("[-f local_path] must be specified when performing a "
           "-w operation\n");
    exit(EXIT_FAILURE);
  }
  if (readflag && !fflag) {
    printf("[-f remote_path] must be specified when performing a "
           "-r operation\n");
    exit(EXIT_FAILURE);
  }
  if (listflag && !fflag) {
    printf("[-f remote_path] must be specified when performing a "
           "-l operation\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_addr.s_addr = inet_addr(server_address);
  server.sin_family = AF_INET;
  server.sin_port = htons(server_port);

  int client_sd;
  if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  };

  if (connect(client_sd, (struct sockaddr *)&server,
              (socklen_t)sizeof(server)) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  printf("Client  connected\n");
  /* Formatting propertly the request */

  if (writeflag) {
    write_to_server(client_sd, f_arg_path, o_arg_path);
  }

  if (readflag) {
    read_from_server(client_sd, f_arg_path, o_arg_path);
  }

  if (listflag) {
    ls_from_server(client_sd, f_arg_path);
  }

  /*
  ********************************************************************************
  ******************************** I want to break FREE.
  ********************************************************************************
  */
  if (server_address != NULL) {
    free(server_address);
  }
  if (f_arg_path != NULL) {
    free(f_arg_path);
  }
  if (o_arg_path != NULL) {
    free(o_arg_path);
  }
}

void write_to_server(int client_sd, char *f_arg_path, char *o_arg_path) {
  char buff[BUFSIZE];
  bzero(buff, BUFSIZE);

  if (oflag) {
    sprintf(buff, "%c %s %s", WRITE, f_arg_path, o_arg_path);
  } else {
    // If -o remote_path was not specified, the f_arg_path will be the
    // remote_path.
    sprintf(buff, "%c %s %s", WRITE, f_arg_path, f_arg_path);
  }

  write(client_sd, buff, BUFSIZE);
  recv(client_sd, buff, BUFSIZE, 0);
  // If localpath == NULL or does not exist in FileSystem,
  // perror();

  // If remotepath == NULL, use the localpath

  // Write the localpath file to the server.
}

void read_from_server(int client_sd, char *f_arg_path, char *o_arg_path) {
  if (f_arg_path == NULL) {
    fprintf(stderr, "Missing File\n");
    exit(EXIT_FAILURE);
  }

  char op = READ;
  write(client_sd, &op, sizeof(char));

  receive_status(client_sd, "op");

  if (!oflag) {
    o_arg_path = xstrdup(f_arg_path);
  }

  char *o_arg_path_cpy = xstrdup(o_arg_path);

  // Send filepath/filename to server
  write(client_sd, f_arg_path, BUFSIZE);

  // If the -o filepath/filename doesn't exist, create
  printf("Checking Or creating %s\n", o_arg_path);

  if (mkdir_r(o_arg_path)) {
    perror("creating file");
    exit(EXIT_FAILURE);
  }

  printf("Opening %s\n", o_arg_path_cpy);

  int fd;
  if ((fd = open(o_arg_path_cpy, O_RDWR, FULLACCESS)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // Check file exists on server
  receive_status(client_sd, "sent file");

  // Check File Size
  int f_size;
  recv(client_sd, &f_size, sizeof(int), 0);

  // Get File data
  int bytes_read;
  char *data;

  FILE *fp = fopen(o_arg_path_cpy, "w");

  data = xmalloc(f_size + 1);

  int t = recv(client_sd, data, f_size, 0);
  data[t] = '\0';

  fputs(data, fp);
  if (fp != NULL) {
    fclose(fp);
  }

  if (o_arg_path_cpy != NULL) {
    free(o_arg_path_cpy);
  }

  if (data != NULL) {
    free(data);
  }
}

void ls_from_server(int client_sd, char *f_arg_path) {
  char buff[BUFSIZE];
  bzero(buff, BUFSIZE);

  sprintf(buff, "%c %s", LIST, f_arg_path);
  write(client_sd, buff, BUFSIZE);

  // If remotepath == NULL, perror()

  // Read the remotepath ls from the server()
}

void receive_status(int client_sd, char *message) {
  int status;
  recv(client_sd, &status, sizeof(int), 0);

  if (status == OK) {
    fprintf(stdout, "%d: %s\n", OK, message);
    return;
  }
  if (status == BADREQ) {
    fprintf(stderr, "%d: %s\n", BADREQ, message);
    exit(EXIT_FAILURE);
  }
  if (status == SERVERERROR) {
    fprintf(stderr, "%d: %s\n", SERVERERROR, message);
    exit(EXIT_FAILURE);
  }
  if (status == NOTFOUND) {
    fprintf(stderr, "%d: %s\n", NOTFOUND, message);
    exit(EXIT_FAILURE);
  }
}