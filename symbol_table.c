#include "symbol_table.h"


SymbolTable *make_symbol_table(){
	SymbolTable *temp;
	if (!(temp = (Symbol_Table *)malloc(sizeof(SymbolTable)))){
		fprintf(stderr, "%s", "exit! no memory - couldn't allocate symbol table\n");
		exit(1);
	}
	temp -> first = NULL;
	return temp;		
}

