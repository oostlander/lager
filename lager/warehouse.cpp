#include "stdafx.h"
#include "avl_baum.h"
#include "warehouse.h"
#include <math.h>

void warehouse_allocate(struct warehouse *whouse)
{
	int i, rows, cols;
	cols = whouse->num_comp;
	rows = whouse->num_shelfs;
	struct Listnode ***mat;
	mat = malloc(cols*sizeof(struct Listnode**));
	for (i = 0; i < cols; i++)
	{
		mat[i] = malloc(rows*sizeof(struct Listnode*));
	}
	whouse->Warehouse = mat;
	avl_create(&(whouse->avl_tree));
	//mat[1][0] = NULL;
	//mat[0][0] = NULL;
	//llist_insert(&(mat[0][0]), bulid_dataset(1337, "test", 1, 1.4, 1.6));
	//llist_insert(&(mat[0][0]), bulid_dataset(1337, "test", 2, 1.4, 1.6));
}

void warehouse_initialize(struct warehouse *whouse)
{
	int i, j;
	float max_width = 120;// move this to main
	float max_weight = 50;
	for (i = 0; i < whouse->num_shelfs; i++)
	{
		for (j = 0; j < whouse->num_comp; j++)
		{
			whouse->Warehouse[i][j] = NULL;
		}
	}
}

struct Datensatz *warehouse_build_dataset(long Teilenummer, char *bez, int Elementnummer, float Breite, float Gewicht)
{
	struct Datensatz *temp_data = (struct Datensatz*)malloc(sizeof(struct Datensatz));
	temp_data->Teilenummer = Teilenummer;
	strncpy_s(temp_data->Bezeichner, 29, bez, 29);
	temp_data->Elementnummer = Elementnummer;
	temp_data->Breite = Breite;
	temp_data->Gewicht = Gewicht;
	temp_data->schluessel.Bezeichner = temp_data->Bezeichner;
	temp_data->schluessel.Elementnummer = &temp_data->Elementnummer;
	temp_data->schluessel.Teilenummer = &temp_data->Teilenummer;
	return temp_data;
}

bool warehouse_add_object(struct warehouse *whouse, struct Datensatz *element)
{
	bool placed = false;
	bool result = false;
	int current_shelf = 0;
	int current_comp = 0;
	float free_width = 0;
	float free_weight = 0;
	float current_rating = 0;
	float best_rating = 0;
	int best_shelf = 0;
	int best_comp = 0;
	while (!result && (current_shelf < whouse->num_shelfs)) // while threshold not met
	{
		while (!result && (current_comp < whouse->num_comp))
		{
			if (whouse->Warehouse[current_shelf][current_comp] != NULL) //is there something in the compartment
			{
				llist_get_freespace(whouse,whouse->Warehouse[current_shelf][current_comp], &free_width, &free_weight);
				if ((free_width>=element->Breite) && (free_weight>=element->Gewicht))//passt ins Fach
				{
					// get current rating
					current_rating = fabsf(llist_get_rating(whouse, &(whouse->Warehouse[current_shelf][current_comp]))
						- llist_get_rating_add(whouse, whouse->Warehouse[current_shelf][current_comp], element));
					if (current_rating>=whouse->threshold)//rating better than threshold
					{
						avl_einfuegen(element, &(whouse->avl_tree));// add to compartment
						llist_insert(&(whouse->Warehouse[current_shelf][current_comp]), element);
						result = true;// object hast been placed
					}
					else
					{
						//save rating and pointer if better than best so far
						if (best_rating<current_rating)
						{
							best_rating = current_rating;
							best_shelf = current_shelf;
							best_comp = current_comp;
						}
						result = false;
					}
				}//if it does not fit in the compartment move on
			}
			else
			{
				avl_einfuegen(element, &(whouse->avl_tree));//place the object as first element in the compartment
				llist_insert(&(whouse->Warehouse[current_shelf][current_comp]), element);
				result = true;
			}
			current_comp++;
		}
		current_shelf++;
	}
	//TODO
	if (false) // if not been placed
	{
		if (false) //best position has been found
		{
			//place at best position found
			result = true;
		}
		else
		{
			result = false;
		}
	}
	return true;
}