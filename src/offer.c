#include "offer.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>

offer* offers = NULL;

int offer_is_valid(player* p, int size, int value)
{
	int count = 0;
	for (int i=0; i < p->hand_size; ++i) {
		if (p->hand[i] == value) {
			++count;
		}
	}
	if (count >= size) 
		return 1;
	return 0;
}

void make_offer(int rank, int size, int value) 
{
	player* p = &players[rank];
	if (offer_is_valid(p, size, value)){
		printf("Thread %d making offer %d[%d]\n", rank, value, size);
		++p->offers;

		offer* new_offer = calloc(1, sizeof(offer));
		new_offer->rank = rank;
		new_offer->size = size;
		new_offer->value = value;

		new_offer->next = offers;
		offers = new_offer;
	}
}

void choose_offer(int rank)
{
	player* p = &players[rank];
	offer* prev = NULL;
	offer* cur = offers;
	while (cur != NULL) {
		/* Flip a "coin" by picking a number that is either 0 or 1.  If it is 1, select the current offer.  Otherwise, continue. */
		int num = rand() % 2;
		if (num == 1) {
			printf("Thread %d chooses offer %d[%d] from %d\n", rank, cur->value, cur->size, cur->rank);

			if (prev == NULL) {
				offers = cur->next;
			}
			else {
				prev->next = cur->next;
			}
			int my_start = rand() % (p->hand_size - cur->size + 1);
			player* partner = &players[cur->rank];
			int start = 0;
			int count = 0;
			while (count < cur->size && start < partner->hand_size) {
				if (partner->hand[start] == cur->value) {
					partner->hand[start] = p->hand[my_start+count];
					p->hand[my_start+count]=cur->value;	
					++count;
				}
				++start;
			}
			free(cur);		
			--partner->offers;
		}
		cur = cur->next;		
	}	
}
