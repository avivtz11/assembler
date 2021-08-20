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


SymbolTable *make_symbol_table();
int add_symbol(SymbolTable *symbol_table, char *symbol, int value, char *attributes);
