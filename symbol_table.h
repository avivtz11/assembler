#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct SymbolNode{
	char *symbol;
	int value;
	char *attributes;
	struct SymbolNode *next;
};

typedef struct SymbolNode SymbolNode;

typedef struct SymbolTable{
	SymbolNode *first;
} SymbolTable;


void make_symbol_table(SymbolTable **result);
int add_symbol(SymbolTable *symbol_table, char *symbol, int value, char *attributes);
void increment_data_addresses(SymbolTable *symbol_table, int value_to_add);
int mark_symbol_as_entry(SymbolTable *symbol_table, char *symbol_name);
int get_internal_label_value(SymbolTable *symbol_table, char *label_param);
int get_label_value(SymbolTable *symbol_table, char *label_param);
void free_symbol_table(SymbolTable *symbol_table);

#endif /*SYMBOL_TABLE_H*/
