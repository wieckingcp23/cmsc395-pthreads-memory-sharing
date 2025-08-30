#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <errno.h>

#include <pthread.h>

#include "ezsocket.h"

struct node* head = NULL;

int server(int port) {
	/* Set up a network socket to listen for incoming connections */
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0) {
		perror("socket");
		exit(1);
	}

	/* Trick from https://www.unix.com/programming/29475-how-solve-error-bind-address-already-use.html */
	int on = 1;
	int err = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	if (err < 0) {
		perror("setsockopt");
		exit(1);
	}


	/* Bind the socket to the "Wildcard" 0.0.0.0 address on the provided port number */
	struct sockaddr_in saddr;
	socklen_t saddr_len = sizeof(saddr);

	bzero(&saddr, saddr_len);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	err = inet_pton(AF_INET, "0.0.0.0", &saddr.sin_addr);
	if (err < 0) {
		perror("inet_pton");
		exit(1);
	}

	err = bind(listen_sock, (struct sockaddr*)&saddr, saddr_len);
	if (err < 0) {
		perror("bind");
		exit(1);
	}

	err = listen(listen_sock, 5);
	if (err < 0) {
		perror("listen");
		exit(1);
	}
	return listen_sock;
}	

struct node* client(int serv)
{
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	bzero(&addr, addr_len);
	node* incoming = (node *)malloc(sizeof(node));
	incoming->next = head;
	head = incoming;

	incoming->socket = accept(serv, (struct sockaddr*)&addr, &addr_len);	
	if (incoming->socket < 0) {
		perror("accept");
		head = head->next;
		free(incoming);
		return NULL;
	}
	return incoming;
}

void cleanup(int serv)
{
	node* cur = head;
	while (cur != NULL) {
		cur = cur->next;
		free(head);
		head = cur;
	}

	int err = close(serv);
	if (err < 0) {
		perror("close(server)");
		exit(1);
	}

}
