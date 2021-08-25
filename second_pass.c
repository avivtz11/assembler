#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "pass_common.h"
#include "parsers.h"

void handle_second_pass_input_line(char *line, SymbolTable *symbol_table, char *data_segment, int *dc, int line_num, int *err_flag);


void second_pass(FILE *assembly_fp, SymbolTable *symbol_table, char *data_segment, FILE *ob_fp)
{
	char line[MAX_LINE];
	int line_num;
	int err_flag;
	int dc;

	dc = 0;
	line_num = 1;
	err_flag = 0;
	while(fgets(line, sizeof(line), assembly_fp)) /*TODO maybe error if more than MAX_LINE?*/
	{
		if(!is_comment_or_empty(line)) /*not comment or empty*/
		{
			handle_second_pass_input_line(line, symbol_table, data_segment, &dc, line_num, &err_flag);
		}

		line_num++;
	}
}


void handle_second_pass_input_line(char *line, SymbolTable *symbol_table, char *data_segment, int *dc, int line_num, int *err_flag)
{
	char *line_ptr;
	char *current_word;
	int entry_err_code;

	line_ptr = line;
	current_word = get_next_word(&line_ptr);

	if(is_symbol_def(current_word)) /*has symbol - skipping it*/
		current_word = get_next_word(&line_ptr);

	if(is_data_storage_line(current_word)) /*is data storage line*/
		code_data_to_dc(current_word, &line_ptr, data_segment, dc); /* assuming free of errors because of first pass */

	else if(is_entry_def(current_word)) /*is entry line*/
	{
		current_word = get_next_word(&line_ptr);
		entry_err_code = mark_symbol_as_entry(symbol_table, current_word);
		if(entry_err_code)
		{
			*err_flag = 1;
			printf("line %d: entry symbol name must match existing non external symbol", line_num);
		}

		if(((*line_ptr) != '\n') && ((*line_ptr) != '\0'))
		{
			*err_flag = 1;
			printf("line %d: extern can only have one param", line_num);
		}
	}

	else if(!is_extern_def(current_word)) /*extern was taken care of in first pass*/
		code_command_to_file();

	free(current_word);
}
