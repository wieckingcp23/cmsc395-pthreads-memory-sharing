#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "player.h"
#include "offer.h"

int num_threads = 0;
extern offer* offers;

int game_over()
{
	for (int i=0; i < num_threads; ++i) {
		if (player_wins(i))
			return 1;
	}
	return 0;
}

void* play_lair(void *arg)
{
	long rank = (long)arg;
	unsigned int seed = rank;
	player* p = &players[rank];
	printf("Thread %d starting\n", rank);
	while (!game_over()) {
		if (p->offers < 1) {
			int size = (rand_r(&seed) % (p->hand_size-1)) + 1;
			int pos = rand_r(&seed) % (p->hand_size - size + 1);
			make_offer(rank, size, p->hand[pos]);
		}
		//Sleep a random amount of time from 1 to 5 seconds.
		int rand = rand_r(&seed) % 5 + 1;
		sleep(rand);
		choose_offer(rank);
	}
	return NULL;
}

void start_threads()
{
	for (long i=0; i < num_threads; ++i) {
		players[i].rank = i;
		pthread_create(&players[i].thread, NULL, play_lair, (void *)i);
	}	
}

void print_hands()
{
	for (int i=0; i < num_threads; ++i) {
		print_hand(i);
	}
	printf("\n");
}

void join_threads()
{
	for (int i=0; i < num_threads; ++i) {
		pthread_join(players[i].thread, NULL);
	}
}

int main(int argc, char* argv[])
{
	srand(4);
	if (argc < 2) {
		printf("Syntax: ./Lair <number of players>\n");
		return 1;
	}
	num_threads = atoi(argv[1]);
	
	deal_cards();
	start_threads();
	while (!game_over())
	{
		print_hands();
		sleep(1);
	}
	printf("Game over!\n");
	join_threads();
	print_hands();
	clean_players();
}
