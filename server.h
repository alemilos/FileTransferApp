#include "utils.h"
#include <dirent.h>

#define MAX_QUEUE 10

extern int aflag, pflag, dflag;

/* Handle the connection with the client, retrieving informations such as OP and
 * pathnames*/
void handle_connection(int client_sd);

/* Meccanismo bloccante, non posso leggere se qualcuno sta scrivendo il file */
void handle_read(int client_sd, char *read_path, char *write_path);

/* Meccanismo bloccante, non posso scrivere se qualcuno sta già
   leggendo/scrivendo sul file */
void handle_write();

/* Meccanismo bloccante, non posso listare se qualcuno sta scrivendo sulla
 * cartella */
void handle_ls();

/* Qui gestisco i blockings mechanisms per la lettura scrittura su un
 * file/cartella */
void handle_blocks();

/* Se c'è un errore a seguito di un OPERATION del client, egli deve essere
   notificato I possibili errori:
   - READ su file non esistente
   - WRITE con Disk Space non sufficiente
   - CONNECTION terminated (crash o altro)
*/
void notify_client(int client_sd, char *message, int status);

/* Check if the path exists, otherwise create it.
   Returns:
   - 0 on success
   - 1 on failure
*/
int get_or_create_ft_root_directory(char *path, DIR **dir);