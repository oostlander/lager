#include "stdafx.h"
#include "llist.h"
#include "warehouse.h"

void llist_print_list(struct Listnode* list)
{
	if (list != NULL)
	{
		struct Listnode* temp_list;
		temp_list = list;
		printf("%d\n", list->dataset);
		while (temp_list->next != NULL)
		{
			temp_list = temp_list->next;
			printf("%d\n", temp_list->dataset);
		}
	}
}
//delets every element in "start"
void llist_del_list(struct Listnode **start)
{
	struct Listnode *pointer1, *pointer2;
	while (*start != NULL)
	{
		pointer1 = *start;
		pointer2 = pointer1->next;
		*start = pointer2;
		free(pointer1);
	}
}

//inserts the "Dataset" in the last place in the "list" given;
//returns true on sucess. Returns false if memory allocation was not possible
//please note that the pointer to the pointer of the list ist needed!
bool llist_insert(struct Listnode **list, struct Datensatz *dataset)
{
	bool result = false;
	struct Listnode *element = *list;
	struct Listnode *previous = NULL;
	struct Listnode *element_new = (struct Listnode *)malloc(sizeof(struct Listnode));
	if (element_new != NULL)
	{
		result = true;
	}
	element_new->dataset = dataset;
	if (element != NULL)
	{
		while (element != NULL)
		{
			previous = element;
			element = element->next;
		}
		element_new->next = NULL;
		previous->next = element_new;
	}
	else
	{
		*list = element_new;
		element_new->next = NULL;
	}
	return result;
}

//counts the length "list"
int list_length(struct Listnode *list)
{
	struct Listnode *element; //the current element we are looking at while counting
	int i = 0;
	element = list;
	if (element != NULL)
	{
		while (element != NULL)
		{
			i++;
			element = element->next;
		}
	}
	else
	{
		return 0;
	}
	return i;
}

//removes "element" from "list". Returns true if sucessfull, false if element could not be removed (e.g. element was not found)
//"element" needs to be the pointer to the element to be removed (get it with llist_element_find())
bool llist_element_remove(struct Listnode **list, struct Datensatz *element)
{
	bool result = false;
	struct Listnode *current_element = *list;
	struct Listnode *previous_element = NULL;
	while (current_element != NULL)
	{
		if (current_element->dataset == element) //we found the element we are searching for
		{
			if (previous_element != NULL) //we are deleting the element in the list
			{
				previous_element->next = current_element->next;
				free(current_element);
			}
			else //we are deleting the last element in the list containing 1 element
			{
				free(current_element);
				*list = NULL;
			}
			result = true; // element obviously was found
			break; // quit searching
		}
		else
		{ // moving through the list
			previous_element = current_element;
			current_element = current_element->next;
		}
	}
	return result;
}

//returns the pointer to the first element in the list that is equal to "element"
//returns a NULL-Pointer if the element was not found
struct Datensatz *llist_element_find(struct Listnode **list, struct Datensatz element)
{
	struct Listnode *current_element = *list;
	struct Datensatz *result = NULL;
	while (current_element != NULL)
	{
		if (avl_gleich(current_element->dataset->schluessel, element.schluessel))
		{
			result = current_element->dataset;
			break;
		}
	}
	return result;
}

//returns the balancing rating of the "list"
//returns -1 if division by 0 was attempted
//might be doing this in parallel
float llist_get_rating(struct warehouse *whouse, struct Listnode **list)
{
	float a, b, result;
	struct Listnode *l = *list;
	a = 0.0;
	b = 0.0;
	if (l != NULL)
	{
		while (l->next != NULL)
		{
			a = a + (l->dataset->Breite);
			b = b + (l->dataset->Gewicht);
		}
		if (b != 0)
		{
			
			result = ((a / b) / (whouse->max_width / whouse->max_weight));
		}
		else
		{
			result = -1;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

//returns the balancing rating of the "list" assuming "element" was already added to the list
//might be doing this in parallel
float llist_get_rating_add(struct warehouse *whouse, struct Listnode **list, struct Datensatz *element)
{
	float a, b, result;
	struct Listnode *l = *list;
	a = element->Breite;
	b = element->Gewicht;
	if (l != NULL)
	{
		while (l->next != NULL)
		{
			a = a + (l->dataset->Breite);
			b = b + (l->dataset->Gewicht);
		}
		result = ((a / b) / (whouse->max_width / whouse->max_weight));
	}
	else
	{
		result = 0;
	}
	return result;
}

// puts the current total weight and width of the load on the compartment "list" in "width" and "weight"
void llist_get_freespace(struct warehouse *whouse, struct Listnode **list, float *width, float *weight)
{
	*width = 0;
	*weight = 0;
	struct Listnode *l = *list;
	while (l != NULL)
	{
		*width = *width + (l->dataset->Breite);
		*weight = *weight + (l->dataset->Gewicht);
		l = l->next;
	}
	*width = whouse->max_width - *width;
	*weight = whouse->max_weight - *weight;
}
