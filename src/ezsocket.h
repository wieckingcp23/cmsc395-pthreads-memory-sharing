#ifndef __EZSOCKET_H
#define __EZSOCKET_H

/* Linked List of sockets and threads */
typedef struct node {
	int socket;
	pthread_t thread;
	struct node* next;
} node;

extern struct node* head;

/* Create a server on port "port" that will listen for incoming connections.  Return a socket fd. */
int server(int port);

/* Accept an incoming connection from server socket "serv".  Returns a node containing the client socket. */
struct node* client(int serv);

/* Free socket list and close server "serv" */
void cleanup(int serv);

#endif //__EZSOCKET_H
