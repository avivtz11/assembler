#include "symbol_table.h"
#include "utils.h"
#include <string.h>

SymbolNode *make_symbol_node(SymbolNode *next, char *symbol, int value, char *attributes);

SymbolTable *make_symbol_table()
{
	SymbolTable *temp = (SymbolTable *)malloc_with_error(sizeof(SymbolTable), "exit! no memory - couldn't allocate symbol table\n");
	
	temp -> first = NULL;
	return temp;		
}


void add_symbol(SymbolTable *symbol_table, char *symbol, int value, char *attributes)
{
	SymbolNode *temp = make_symbol_node(symbol_table->first, symbol, value, attributes);

	symbol_table->first = temp;
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
