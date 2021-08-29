#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "symbol_table.h"
#include "externals_usage_list.h"

int second_pass(FILE *assembly_fp, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, char *data_segment, FILE *ob_fp);

#endif/*SECOND_PASS_H*/
