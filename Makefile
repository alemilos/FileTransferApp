all: run

run: server 
	 ./server

main: server.o 
	$(CC) $(LDFLAGS) -o $@ $<

main.o: server.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f server server.o

.PHONY: all run clean