#include <stdio.h>
#include <stdlib.h>

#include "player.h"

player* players;

void deal_cards()
{
	players = calloc(num_threads, sizeof(player));
	for (long i=0; i < num_threads; ++i) {
		players[i].hand = calloc(num_threads, sizeof(int));
		players[i].hand_size = 0;
		players[i].offers = 0;
	}
	//For each type of card
	for (long i=0; i < num_threads; ++i) {
		//Deal "num_threads" cards of that type
		for (long j=0; j < num_threads; ++j) {
			/* Find a player with at least one empty spot in their hand. */
			int p = random() % num_threads;
			while (players[p].hand_size >= num_threads) {
				p = random() % num_threads;
			}
			player* player = &players[p];
			//Give them this card.	
			player->hand[player->hand_size] = i;
			++player->hand_size;
		}	
	}
}

void print_hand(int rank)
{
	printf("Hand[%d]: ", rank);
	for (int i=0; i < players[rank].hand_size; ++i) {
		printf("%d ", players[rank].hand[i]);
	}
	printf("\n");
}

int player_wins(int rank)
{
	player* player = &players[rank];
	int val = player->hand[0];
	for (int i=1; i < num_threads; ++i) {
		if (player->hand[i] != val)
			return 0;
	}
	return 1;
}

void clean_players()
{
	for (int i=0; i < num_threads; ++i) {
		free(players[i].hand);
	}
	free(players);
}
