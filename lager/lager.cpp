#include "stdafx.h"
#include "avl_baum.h"
#include "llist.h"
#include"warehouse.h"
#include <stdbool.h>

const int COMMAND_NUM = 4;
const int COMMAND_NONE = -1;
const int COMMAND_MULTIPLE = -2;
struct Listnode* liste; // a pointer to the first Element of the List
struct Datensatz *bulid_dataset(long Teilenummer, char *bez, int Elementnummer, float Breite, float Gewicht);

// allocate "size" bytes of Memory and exits the program if allocation was not possible.
void *allocateMemory(size_t size)
{
	void *p;
	if ((p = malloc(size)) == NULL)
	{
		printf("\n\nSpeicherprobleme!\n");
		/* exit program */
		exit(1);
	}
	return p;
}


//prints the command defined by "command"
void print_command(int command, bool *terminate, char var[20])
{
	switch (command)
	{
	case -2:
		printf("Please enter only one commmand at a time!\n");
		break;
	case 0:
		printf("This Program builds a linked list from numbers the user types in.\n");
		printf("Non numerical input will be disregarded.\n");
		printf("available commands:\n    help   displays this message\n    list   prints the current list\n    exit   exit the programm\n");
		break;
	case 1:
		//print list
		break;
	case 2:
		*terminate = true;
		break;
	case 3:
		printf("I should remove an element...\n");
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
	char commands[4][10] = { { "help" }, { "list" }, { "exit" }, { "-r" } };// eigentlich COMMAND_NUM
	int command = -1;
	struct warehouse my_warehouse;
	my_warehouse.num_shelfs = 2;
	my_warehouse.num_comp = 5;
	my_warehouse.threshold = 0.1;
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

	// attempt to read the next line and store 
	// the value in the "number" variable 
	while (fscanf(in_file, "%d %s %f %f", &t_Teilenummer, &t_Bezeichner, &t_Breite, &t_Gewicht) == 4)
	{
		if (!(warehouse_add_object(&my_warehouse, warehouse_build_dataset(t_Teilenummer, t_Bezeichner, 1, t_Breite, t_Gewicht))))
		{
			printf("We could not read %d\n", t_Teilenummer);
		}
		else
			//avl_einfuegen(warehouse_build_dataset(t_Teilenummer, t_Bezeichner, 1, t_Breite, t_Gewicht), &(my_warehouse.avl_tree));
		{
			printf("We just read %d\n", t_Teilenummer);
		}
	}
	fclose(in_file);
	///////////////////// reading in done //////////////////////
	avl_write(my_warehouse.avl_tree, 1);

	printf("Listbuilder v1.1\n(c) Klaas P. Oostlander\nThis Program builds a linked list of numbers from user-input\nPlease enter a number:\n");
	char var[20];    // This is the variable to store input.
	int i = 0;
	int varisnum = 0;    // Is var all numbers?  1 for yes, 0 for no.
	bool terminate = false;
	while (!terminate)
	{
		fgets(var, sizeof(var), stdin);
		i = 0; //reset i to 0(we are in a loop her ;-)
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
			print_command(command, &terminate, var);
		}
		else
		{
			llist_print_list(liste);
			printf("please enter a number:\n");
		}
	}

	return 0;
}