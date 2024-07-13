#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

void my_write() {}

void my_read(int socket) {}

void handle_conn(void *sd) {
  int socket = (int)((unsigned int)((size_t)sd));

  printf("%d\n", socket);

  my_read(socket);
}

int main() {
  for (size_t i = 0; i < 10; i++) {
    pthread_t tid;

    int socket = i;
    pthread_create(&tid, NULL, (void *)handle_conn,
                   (void *)((size_t)((unsigned int)socket)));
  }
  sleep(1);
}
