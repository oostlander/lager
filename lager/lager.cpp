#include "stdafx.h"
#include "avl_baum.h"
#include "llist.h"
#include "warehouse.h"
#include <stdbool.h>

const int COMMAND_NUM = 4;
const int COMMAND_NONE = -1;
const int COMMAND_MULTIPLE = -2;
struct Listnode* liste; // a pointer to the first Element of the List
struct Datensatz *bulid_dataset(long Teilenummer, char *bez, int Elementnummer, float Breite, float Gewicht);

write_list(struct warehouse *whouse)
{
	FILE *in_file = fopen("Lagerliste.txt", "w"); // write 

	if (!in_file) // equivalent to saying if ( in_file == NULL ) 
	{
		printf("oops, file can't be read\n");
	}
	else
	{
		//print the formated list to file
		avl_write_file(whouse->avl_tree, in_file);
		fclose(in_file);
	}
}

//prints the command defined by "command"
void print_command(int command, bool *terminate, char var[],struct warehouse *whouse)
{
	switch (command)
	{
	case -2:
		printf("Please enter only one commmand at a time!\n");
		break;
	case 0:
		printf("This Program bulds and maintains a virtual warehouse of objects\n");
		printf("They can either be read in through a file or entered manually.\n");
		printf("available commands:\n    help   displays this message\n    list   displays an ordered list of all objects stored\n");
		printf("    print  prints a list of all objects currently stored to a file\n    exit   exit the programm\n");
		break;
	case 1:
		write_list(whouse);
		printf("A list of all items currently stored has been printed to: Lagerliste.txt\n");
		break;
	case 2:
		*terminate = true;
		break;
	case 3:
		avl_write_list(whouse->avl_tree);
		//char **test;
		//element_remove(&liste, strtol(strstr(var, "-r") + (2 * sizeof(char)), test, 10));
		break;
	default:
		printf("Input was not all Numbers!\n");
		break;
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	liste = NULL;
	char commands[4][10] = { { "help" }, { "print" }, { "exit" }, { "list" } };// eigentlich COMMAND_NUM
	int command = -1;
	struct warehouse my_warehouse;
	my_warehouse.num_shelfs = 2;
	my_warehouse.num_comp = 5;
	my_warehouse.threshold = 0.09;
	warehouse_allocate(&my_warehouse);
	warehouse_initialize(&my_warehouse);
	warehouse_add_object(&my_warehouse, warehouse_build_dataset(3456, "Belag", 1, 30, 2));

	////////////////// read in the Data from File ///////////////
	long t_Teilenummer;
	char t_Bezeichner[30];
	float t_Breite, t_Gewicht;

	FILE* in_file = fopen("test.data", "r"); // read only  

	if (!in_file) // equivalent to saying if ( in_file == NULL ) 
	{
		printf("oops, file can't be read\n");
	}
	else
	{
		// attempt to read the next line and store 
		// the value in the "number" variable 
		while (fscanf(in_file, "%d %s %f %f", &t_Teilenummer, t_Bezeichner, &t_Breite, &t_Gewicht) == 4)
		{
			if (!(warehouse_add_object(&my_warehouse, warehouse_build_dataset(t_Teilenummer, t_Bezeichner, 1, t_Breite, t_Gewicht))))
			{
				printf("We could not read %d\n", t_Teilenummer);
			}
			else
			{
				printf("We just read %d\n", t_Teilenummer);
			}
		}
		fclose(in_file);
	}
	///////////////////// reading in done //////////////////////
	///uncomment the following to see the read in avl tree and the final list
	//avl_write(my_warehouse.avl_tree, 1);
	//printf("testing List:\n");
	//avl_write_list(my_warehouse.avl_tree);

	printf("Lagerverwalter v1.1\n(c) Klaas P. Oostlander, Eduard Boitschenko\nThis Program bulds and maintains a virtual warehouse of objects");
	printf("\nPlease enter an Object to add like this:\n\"Teilenummer\"(int) \"Bezeichner\"(char[30]) \"Breite\"(float) \"Gewicht(float)\"\n");
	char var[80];    // This is the variable to store input.
	int i = 0;
	int varisnum = 0;    // Is var all numbers?  1 for yes, 0 for no.
	bool terminate = false;
	//int numarg = 0;
	//char test[30];//string we use to test what we have read.
	while (!terminate)
	{
		//numarg = 0;
		i = 0; //reset i to 0(we are in a loop her ;-)
		fgets(var, sizeof(var), stdin);
		if (sscanf(var, "%d %s %f %f", &t_Teilenummer, t_Bezeichner, &t_Breite, &t_Gewicht) == 4)
		{
			warehouse_add_object(&my_warehouse, warehouse_build_dataset(t_Teilenummer, t_Bezeichner, 1, t_Breite, t_Gewicht));
		}
		else
		{
			command = -1; //no command has been chosen jet
			while (isalnum(var[i]) != 0)
			{    // Loop until it a character is not alpha-numeric.
				if (isdigit(var[i]) != 0)
				{    // Is var[i] a numeric digit?
					varisnum = 1;
				}
				else
				{
					varisnum = 0;
					break;    // If we encounter a non-numeric character, there is no need to keep looping, so just break out.	
				}
				i++;    // Move counter to the next element.
			}
			if (varisnum == 0)
			{
				for (int j = 0; j < COMMAND_NUM; j++)
				{
					if (strstr(var, commands[j]) != NULL)
					{
						if (command != COMMAND_NONE)
						{
							command = COMMAND_MULTIPLE;//two commands (and possibly more) were entered
							break;
						}
						command = j;
					}
				}
				print_command(command, &terminate, var, &my_warehouse);
			}
			else
			{
				//llist_print_list(liste);
				printf("Input not recognized! Please try again.\n");
			}
		}
	}

	return 0;
}