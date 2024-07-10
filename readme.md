Strategia per sviluppare il software

CLIENT (cosa può fare)

- Si connette al server

1. Può scrivere un file nel server in 2 modi (-w operation)

- specificando localpath/filename e remotepath/filename
- specifianco solo localpath/filename (il remotepath sarà uguale al localpath)

2. Può leggere un file dal server (-r read operation) e copiare in locale

- specificando sourcepath/filename e remotepath/filename
- specificando solo remotepath/filename (il localpath sarà uguale al remotepath)

3. Listare i file che si trovano in remotepath (-l operation)

SERVER (cosa può fare)

1. Deve gestire richieste concorrenti

- richieste di scrittura concorrenti sullo stesso file o path

2. Gestire errori

- spazio su disco esaurito
- client connection interruption
- richiesta accesso a file non esistenti


## Edge Cases

### ERRORS
- Client vuole leggere da un file remoto che non esiste.
- Client vuole scrivere un file locale che non esiste.

## SOLVABLE 
- Client vuole leggere un file ma vuole scriverlo su un path non esistente.
- -> Soluzione: creo il path locale e ci scrivo dentro ciò che viene letto in remote.

- Client vuole scrivere un file in remote su un path non esistente.
- -> Soluzione: creo il path remote e ci scrivo dentro ciò che viene letto in locale.

- Client vuole listare un path non esistente 
- -> Printo l'output di stderr di ls.