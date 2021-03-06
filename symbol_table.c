#include "symbol_table.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void make_symbol_node(SymbolNode **result, SymbolNode *next, char *symbol, int value, char *attributes);
int is_symbol_name_taken(SymbolTable *symbol_table, char *symbol);
int is_extern_symbol_taken(SymbolTable *symbol_table, char *symbol);
void add_attribute(SymbolNode *symbol_node, char *attribute);


void make_symbol_table(SymbolTable **result)
/*this function creates a symbol table*/
{
	malloc_with_error((void **)result, sizeof(SymbolTable));
	
	(*result) -> first = NULL;
}


int add_symbol(SymbolTable *symbol_table, char *symbol, int value, char *attributes)
/*this function adds a symbol to symbol table - returns error flag*/
{
	SymbolNode *temp;
	if(strcmp(attributes, "extern") == 0)
	{
		if(is_extern_symbol_taken(symbol_table, symbol))
			return 1;
	}
	else if(is_symbol_name_taken(symbol_table, symbol))
		return 1;

	make_symbol_node(&temp, symbol_table->first, symbol, value, attributes);

	symbol_table->first = temp;

	return 0;
}


int is_extern_symbol_taken(SymbolTable *symbol_table, char *symbol)
/*this function checks if a symbol name is taken by a non external label*/
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
/*this function adds a offset to all addresses in symbol table*/
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


int get_internal_label_value(SymbolTable *symbol_table, char *label_param)
/*this funtion gets a non external label - if non exists, return error flag*/
{
	SymbolNode *symbol_node;
	symbol_node = symbol_table->first;
	while(symbol_node)
	{
		if(strcmp(symbol_node->symbol, label_param) == 0)
		{
			if(strstr(symbol_node->attributes, "external"))
				return 1;
			return symbol_node->value;
		}
		symbol_node = symbol_node->next;
	}
	return 1;
}


int get_label_value(SymbolTable *symbol_table, char *label_param)
/*if external - zero*/
{
	SymbolNode *symbol_node;
	symbol_node = symbol_table->first;
	while(symbol_node)
	{
		if(strcmp(symbol_node->symbol, label_param) == 0)
		{
			return symbol_node->value;
		}
		symbol_node = symbol_node->next;
	}
	return 1;
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


int output_symbols_of_attribute(FILE *fp, SymbolTable *symbol_table, char *attribute)
/*this function outputs names and values of symbols with the given attribute*/
{
	int outputs;
	SymbolNode *node;
	outputs = 0;
	node = symbol_table->first;
	while(node)
	{
		if(strstr(node->attributes, attribute))
		{
			fprintf(fp, "%s ", node->symbol);
			fprintf(fp, "%04d\n", node->value);
			outputs = 1;
		}
		node = node->next;
	}
	return outputs;
}


void make_symbol_node(SymbolNode **result, SymbolNode *next, char *symbol, int value, char *attributes)
{
	malloc_with_error((void **)result, sizeof(SymbolNode));
	
	(*result)->next = next;
	(*result)->value = value;

	malloc_with_error((void **)&((*result)->symbol), sizeof(strlen(symbol) + 1));
	strcpy((*result)->symbol, symbol);

	malloc_with_error((void **)&((*result)->attributes), sizeof(strlen(attributes) + 1));
	strcpy((*result)->attributes, attributes);
}


void free_symbol_table(SymbolTable *symbol_table)
/*this function frees a symbol table completely*/
{
	SymbolNode *tmp;
	while(symbol_table->first)
	{
		tmp = symbol_table->first;
		symbol_table->first = symbol_table->first->next;
		free(tmp->symbol);
		free(tmp->attributes);
		free(tmp);
	}
}
