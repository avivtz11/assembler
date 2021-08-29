#ifndef PARSERS_UTILS_H
#define PARSERS_UTILS_H

#include "symbol_table.h"
#include "externals_usage_list.h"

int get_next_param(char **params, char **result_param);
void code_register(char *register_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code);
void code_immed(char *immed_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code);
void code_label_distance(char *label_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code);
void code_register_or_label_address(char *label_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code);
void code_label_address(char *param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code);

#endif/*PARSERS_UTILS_H*/
