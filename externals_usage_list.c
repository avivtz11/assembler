#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "externals_usage_list.h"
#include "utils.h"

void make_external_usage_node(ExternalsUsageNode **result, ExternalsUsageNode *next, int ic, char *symbol);


void make_externals_usage_list(ExternalsUsageList **result)
{
	malloc_with_error((void **)result, sizeof(ExternalsUsageList));
	
	(*result) -> first = NULL;
}


void free_externals_usage_list(ExternalsUsageList *external_usage_list)
{
	ExternalsUsageNode *tmp;
	while(external_usage_list->first)
	{
		tmp = external_usage_list->first;
		external_usage_list->first = external_usage_list->first->next;
		free(tmp->symbol);
		free(tmp);
	}
}


int output_externals_usage_list(FILE *fp, ExternalsUsageList *externals_usage_list)
{
	int outputs;
	ExternalsUsageNode *node;
	outputs = 0;
	node = externals_usage_list->first;
	while(node)
	{
		fprintf(fp, "%s ", node->symbol);
		fprintf(fp, "%04d\n", node->usage_address);
		outputs = 1;
		node = node->next;
	}
	return outputs;
}


void add_to_externals_usage_list(ExternalsUsageList *externals_usage_list, int ic, char *symbol)
{
	ExternalsUsageNode *temp;

	make_external_usage_node(&temp, externals_usage_list->first, ic, symbol);

	externals_usage_list->first = temp;
}


void make_external_usage_node(ExternalsUsageNode **result, ExternalsUsageNode *next, int ic, char *symbol)
{
	malloc_with_error((void **)result, sizeof(ExternalsUsageNode));
	
	(*result)->next = next;
	(*result)->usage_address = ic;

	malloc_with_error((void **)&((*result)->symbol), sizeof(strlen(symbol) + 1));
	strcpy((*result)->symbol, symbol);
}
