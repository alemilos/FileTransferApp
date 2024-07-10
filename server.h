/* Operations */
#define READ
#define WRITE
#define LIST

/* Meccanismo bloccante, non posso leggere se qualcuno sta scrivendo il file */
void handle_read();

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
void notify_client();