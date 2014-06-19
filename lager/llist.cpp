#include "stdafx.h"
#include "llist.h"
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
bool llist_insert(Listnode **list, Datensatz *dataset)
{
	bool result = false;
	Listnode *element = *list;
	Listnode *previous = NULL;
	struct Listnode *element_new = (Listnode *)malloc(sizeof(Listnode));
	if (element_new != NULL)
	{
		result = true;
	}
	element_new->dataset = dataset;
	while (element != NULL)
	{
		previous = element;
		element = element->next;
	}
	if (previous = NULL)
	{
		element_new->next = *list;
		*list = element_new;
	}
	else
	{
		element_new->next = element;
		previous->next = element_new;
	}
	return result;
}

//counts the length "list"
int list_length(Listnode *list)
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
bool llist_element_remove(struct Listnode **list, Datensatz *element)
{
	bool result = false;
	Listnode *current_element = *list;
	Listnode *previous_element = NULL;
	if (*list != NULL)
	{
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
	}
	else
	{
		result = false;
	}
	return result;
}

//returns the pointer to the first element in the list that is equal to "element"
//returns a NULL-Pointer if the element was not found
Datensatz *llist_element_find(Listnode **list, Datensatz element)
{
	Datensatz *result = NULL;
	return result;
}
