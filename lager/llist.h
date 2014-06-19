#ifndef L_LIST_H
#define L_LIST_H
#include "avl_baum.h"
struct Listnode{
	Datensatz *dataset;
	struct Listnode *next;
};
void llist_print_list(struct Listnode* list);
void llist_del_list(struct Listnode **start);
bool llist_insert(Listnode **list, Datensatz *dataset);
bool llist_element_remove(struct Listnode **list, Datensatz *element);
#endif