#include <sys/socket.h>

/* Flags */
extern int aflag, pflag, dflag;
extern int readflag, writeflag, listflag;
extern int fflag, oflag;

/* Communication with the server */

void write_to_server(int client_sd, char *localpath, char *remotepath);
void read_from_server(int client_sd, char *localpath, char *remotepath);
void ls_from_server(int client_sd, char *remotepath);

void receive_status(int client_sd, char *message);