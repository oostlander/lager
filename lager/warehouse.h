#ifndef WHOUSE_H
#define WHOUSE_H
#include "avl_baum.h"
#include "llist.h"
struct warehouse
{
	int num_shelfs;
	int num_comp;
	float max_weight;
	float max_width;
	float threshold;
	struct Listnode*** Warehouse;
	AVL_baum avl_tree;
};
void warehouse_allocate(struct warehouse *whouse);
void warehouse_initialize(struct warehouse *whouse);
struct Datensatz *warehouse_build_dataset(long Teilenummer, char *bez, int Elementnummer, float Breite, float Gewicht);
bool warehouse_add_object(struct warehouse *whouse, struct Datensatz *element);
#endif