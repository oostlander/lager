#ifndef L_LIST_H
#define L_LIST_H
#include "avl_baum.h"
#include <stdbool.h>
struct Listnode{
	struct Datensatz *dataset;
	struct Listnode *next;
};
//void llist_print_list(struct Listnode* list);
void llist_del_list(struct Listnode **start);
bool llist_insert(struct Listnode **list, struct Datensatz *dataset);
bool llist_element_remove(struct Listnode **list, struct Datensatz *element);
float llist_get_rating(struct warehouse *whouse, struct Listnode **list);
float llist_get_rating_add(struct warehouse *whouse, struct Listnode **list, struct Datensatz *element);
void llist_get_freespace(struct warehouse *whouse, struct Listnode **list, float *width, float *weight);
#endif