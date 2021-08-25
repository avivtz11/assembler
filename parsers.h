int count_data_length(char *data_command, char **params);
void code_data_to_dc(char *data_command, char **params, char *data_segment, int *dc);
char *code_command(char *current_word, char **line_ptr, SymbolTable* symbol_table);
