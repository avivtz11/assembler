#ifndef OUTPUT_UTILS_H
#define OUTPUT_UTILS_H

#include <stdio.h>

#include "symbol_table.h"
#include "externals_usage_list.h"

void print_counters(FILE *fp, int ic, int dc);
void print_ob_output_line(FILE *fp, int *start_byte_index, char *current_output_bytes);
void output_symbols(char *output_file_path, SymbolTable *symbol_table, char *attribute);
void output_data_segment(FILE *fp, char *data_segment, int ic, int dc);
void output_externals_usage(char *output_file_path, ExternalsUsageList *externals_usage_list);

#endif/*OUTPUT_UTILS_H*/
