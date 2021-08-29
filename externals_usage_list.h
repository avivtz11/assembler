#ifndef EXTERNALS_USAGE_LIST_H
#define EXTERNALS_USAGE_LIST_H

struct ExternalsUsageNode{
	char *symbol;
	int usage_address;
	struct ExternalsUsageNode *next;
};

typedef struct ExternalsUsageNode ExternalsUsageNode;

typedef struct ExternalsUsageList{
	ExternalsUsageNode *first;
} ExternalsUsageList;

void make_externals_usage_list(ExternalsUsageList **result);
void free_externals_usage_list(ExternalsUsageList *external_usage_list);
int output_externals_usage_list(FILE *fp, ExternalsUsageList *externals_usage_list);
void add_to_externals_usage_list(ExternalsUsageList *externals_usage_list, int ic, char *symbol);

#endif/*EXTERNALS_USAGE_LIST_H*/
