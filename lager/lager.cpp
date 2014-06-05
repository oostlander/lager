// lager.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

struct Listnode{
	int zahl;
	struct Listnode* next;
};

const int COMMAND_NUM = 4;
const int COMMAND_NONE = -1;
const int COMMAND_MULTIPLE = -2;
struct Listnode* liste; // a pointer to the first Element of the List

void print_list(struct Listnode* list)
{
	if (list != NULL)
	{
		struct Listnode* temp_list;
		temp_list = list;
		printf("%d\n", list->zahl);
		while (temp_list->next != NULL)
		{
			temp_list = temp_list->next;
			printf("%d\n", temp_list->zahl);
		}
	}
}

//delets every element in "start"
void del_list(struct Listnode **start)
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

//inserts the "number" in the first place in the "list" given; please note that the pointer to the pointer of the list ist needed!
void insert(Listnode **list, int number)
{
	struct Listnode *element_new = (Listnode *)malloc(sizeof(Listnode));
	element_new->zahl = number;
	element_new->next = *list;
	*list = element_new;
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

//removes "element" from "list". Returns true if sucessfull, false if element could not be removed (e.g. not existent)
bool element_remove(struct Listnode **list, int element)
{
	return true;
}

//places a new element"number" at the right place in an already sorted "list"(ascending order). 
void insert_sort(Listnode **list, int number)
{
	struct Listnode *element_new = (Listnode *)malloc(sizeof(Listnode));
	struct Listnode *current_element = NULL;
	element_new->zahl = number;
	current_element = *list;
	if (current_element != NULL)
	{
		while ((current_element->next != NULL) && (current_element->next->zahl < number))
		{
			current_element = current_element->next;
		}
		if ((current_element == *list) && (current_element->zahl > number))
		{
			element_new->next = *list;
			*list = element_new;
		}
		else
		{
			element_new->next = current_element->next;
			current_element->next = element_new;
		}
	}
	else
	{
		element_new->next = *list;
		*list = element_new;
	}

}

//prints the command defined by "command"
void print_command(int command, bool *terminate, char var[20])
{
	switch (command)
	{
	case COMMAND_MULTIPLE:
		printf("Please enter only one commmand at a time!\n");
		break;
	case 0:
		printf("This Program builds a linked list from numbers the user types in.\n");
		printf("Non numerical input will be disregarded.\n");
		printf("available commands:\n    help   displays this message\n    list   prints the current list\n    exit   exit the programm\n");
		break;
	case 1:
		printf("list entries:(%d)\n", list_length(liste));
		print_list(liste);
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
	char commands[COMMAND_NUM][10] = { { "help" }, { "list" }, { "exit" }, { "-r" } };
	int command = -1;

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
			insert_sort(&liste, (int)strtol(var, NULL, 10));
			printf("list entries:(%d)\n", list_length(liste));
			print_list(liste);
			printf("please enter a number:\n");
		}
	}
	del_list(&liste);
	return 0;
}

