#ifndef PLAYER_H
#define PLAYER_H

#include <pthread.h>

extern int num_threads;

typedef struct player {
	pthread_t thread;
	int rank;
	int *hand;
	int hand_size;
	int offers;
} player;

extern player* players;

void deal_cards();
void print_hand(int rank);
int player_wins(int rank);

void clean_players();

#endif //PLAYER_H
