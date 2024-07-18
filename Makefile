SERVER_ARGS= -a 0.0.0.0 -p 1024 -d root
# CFLAGS = -std=c99 -ggdb -Wall -Wextra -pedantic -pedantic-errors

CFLAGS += -ggdb

all: run

run: server 
	 ./server ${SERVER_ARGS}

server: server.o utils.o
	$(CC) -pthread $(LDFLAGS) -o $@ $^

client: client.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^

server.o: server.c
	$(CC) $(CFLAGS) -c -o $@ $<

client.o: client.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o client server 

.PHONY: all run clean
