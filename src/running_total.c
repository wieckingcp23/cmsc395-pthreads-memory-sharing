#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <pthread.h>
#include "ezsocket.h"

int total = 0;

void * sum( void * arg ) 
{
	node* client = (node *)arg;

	char buffer[4096];
	int pos = 0;
	int err = read(client->socket, &buffer[pos], 1);
	while (!isspace(buffer[pos])) {
		++pos;
		err = read(client->socket, &buffer[pos], 1);
		if (pos >= 4095) break;
	}
	buffer[++pos] = '\0';
	int num = atoi(buffer);
	total += num;
	printf(" + %d =\n %d", num, total);
	fflush(stdout);
	err = close(client->socket);
	if (err < 0) {
		perror("close(client)");
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Syntax: ./Server <port>\n");
		exit(1);
	}

	int port = atoi(argv[1]);

	int serv = server(port);

	int count = 0;
	printf("Total: %d", total);
	while (count < 50) {
		node* incoming = client(serv);
		if (incoming != NULL) {
			pthread_create(&incoming->thread, NULL, sum, (void *)incoming);	
			++count;
		}
	}

	
	node* cur = head;
	while (cur != NULL) {
		pthread_join(cur->thread, NULL);
		cur = cur->next;
	}

	printf("\n");
	cleanup(serv);
	

	printf("The total is: %ld\n", total);

	return 0;
}
