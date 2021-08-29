#ifndef PARSERS_H
#define PARSERS_H

int count_data_length(char *data_command, char **params);
void code_data_to_dc(char *data_command, char **params, char *data_segment, int *dc);
void code_command(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int ic, int *err_code);

#endif/*PARSERS_H*/
