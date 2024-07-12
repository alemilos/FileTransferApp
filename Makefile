ARGS = -a 0.0.0.0 -p 1024 -d root

all: run

run: server 
	 ./server ${ARGS}

server: server.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^

client: client.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^

server.o: server.c
	$(CC) $(CFLAGS) -c -o $@ $<

client.o: client.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o server client

.PHONY: all run clean