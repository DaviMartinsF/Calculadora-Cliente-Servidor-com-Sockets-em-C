#Davi Martins Figueiredo - 10374878
CC = gcc
CFLAGS = -Wall -g

all: server client  # alvo 'all' depende de server e client

server: server.c
	$(CC) $(CFLAGS) server.c -o server  # TAB antes do comando

client: client.c
	$(CC) $(CFLAGS) client.c -o client  # TAB antes do comando

clean:
	rm -f server client
