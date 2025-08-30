#ifndef OFFER_H
#define OFFER_H
#include "player.h"

typedef struct offer {
	int rank;
	int size;
	int value;
	struct offer* next;
} offer;

extern offer* offers;

int offer_is_valid(player* p, int size, int value);
void make_offer(int rank, int size, int value);
void choose_offer(int rank);

#endif //OFFER_H
