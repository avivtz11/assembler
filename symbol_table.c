#include "symbol_table.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

SymbolNode *make_symbol_node(SymbolNode *next, char *symbol, int value, char *attributes);
int is_symbol_name_taken(SymbolTable *symbol_table, char *symbol);
int is_extern_symbol_taken(SymbolTable *symbol_table, char *symbol);
void add_attribute(SymbolNode *symbol_node, char *attribute);


SymbolTable *make_symbol_table()
{
	SymbolTable *temp = (SymbolTable *)malloc_with_error(sizeof(SymbolTable), "exit! no memory - couldn't allocate symbol table\n");
	
	temp -> first = NULL;
	return temp;		
}


int add_symbol(SymbolTable *symbol_table, char *symbol, int value, char *attributes)
{
	SymbolNode *temp;
	if(strcmp(attributes, "extern") == 0)
	{
		if(is_extern_symbol_taken(symbol_table, symbol))
			return 1;
	}
	else if(is_symbol_name_taken(symbol_table, symbol))
		return 1;

	temp = make_symbol_node(symbol_table->first, symbol, value, attributes);

	symbol_table->first = temp;

	return 0;
}


int is_extern_symbol_taken(SymbolTable *symbol_table, char *symbol)
{
	SymbolNode *symbol_node;

	symbol_node = symbol_table->first;
	while(symbol_node)
	{
		/*same name but also is external is ok*/
		if((strcmp(symbol_node->symbol, symbol) == 0) && !strstr(symbol_node->attributes, "external"))
			return 1;
		symbol_node = symbol_node->next;
	}
	return 0;
}


int is_symbol_name_taken(SymbolTable *symbol_table, char *symbol)
{
	SymbolNode *symbol_node;

	symbol_node = symbol_table->first;
	while(symbol_node)
	{
		if(strcmp(symbol_node->symbol, symbol) == 0)
			return 1;
		symbol_node = symbol_node->next;
	}
	return 0;
}


void increment_data_addresses(SymbolTable *symbol_table, int value_to_add)
{
	SymbolNode *symbol_node;

	symbol_node = symbol_table->first;
	while(symbol_node)
	{
		if(strstr(symbol_node->attributes, "data"))
			symbol_node->value += value_to_add;

		symbol_node = symbol_node->next;
	}
}


int mark_symbol_as_entry(SymbolTable *symbol_table, char *symbol_name)
{
	SymbolNode *symbol_node;

	symbol_node = symbol_table->first;
	while(symbol_node)
	{
		if(strcmp(symbol_node->symbol, symbol_name) == 0)
		{
			if(strstr(symbol_node->attributes, "external"))
				return 1;
			add_attribute(symbol_node, "entry");
			return 0;
		}
		symbol_node = symbol_node->next;
	}
	return 1;
}


void add_attribute(SymbolNode *symbol_node, char *attribute)
{
	char *tmp = realloc(symbol_node->attributes, strlen(symbol_node->attributes) + 2 + strlen(attribute) + 1);
	if(!tmp)
	{
		printf("couldn't allocate memory! exiting");
		exit(1);
	}
	symbol_node->attributes = tmp;

	strcat(symbol_node->attributes, ", ");
	strcat(symbol_node->attributes, attribute);
}


SymbolNode *make_symbol_node(SymbolNode *next, char *symbol, int value, char *attributes)
{
	SymbolNode *temp = (SymbolNode *)malloc_with_error(sizeof(SymbolNode), "exit! no memory - couldn't allocate symbol structure\n");
	
	temp->next = next;
	temp->value = value;

	temp->symbol = (char *)malloc_with_error(sizeof(strlen(symbol) + 1), "exit! no memory - couldn't allocate symbol string\n");
	strcpy(temp->symbol, symbol);

	temp->attributes = (char *)malloc_with_error(sizeof(strlen(attributes) + 1), "exit! no memory - couldn't allocate symbol attributes\n");
	strcpy(temp->attributes, attributes);

	return temp;
}
