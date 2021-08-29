#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "externals_usage_list.h"
#include "pass_common.h"
#include "parsers.h"
#include "utils.h"
#include "output_utils.h"

char *handle_second_pass_input_line(char *line, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, char *data_segment, int *dc, int *ic, int line_num, int *err_flag);


int second_pass(FILE *assembly_fp, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, char *data_segment, FILE *ob_fp)
{
	char line[MAX_LINE];
	int line_num;
	char *current_output_bytes;
	int err_flag;
	int dc;
	int ic;
	int byte_location;

	dc = 0;
	ic = 100;
	line_num = 1;
	err_flag = 0;
	byte_location = 100;
	while(fgets(line, sizeof(line), assembly_fp)) /*won't be longer than MAX_LINE - checked in first_pass*/
	{
		if(!is_comment_or_empty(line)) /*not comment or empty*/
		{
			current_output_bytes = handle_second_pass_input_line(line, symbol_table, externals_usage_list, data_segment, &dc, &ic, line_num, &err_flag);
			if(current_output_bytes)/*if error - skips in print (output file deleted anyway)*/
			{
				print_ob_output_line(ob_fp, &byte_location, current_output_bytes);
				free(current_output_bytes);
			}
		}

		line_num++;
	}
	return err_flag;
}


char *handle_second_pass_input_line(char *line, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, char *data_segment, int *dc, int *ic, int line_num, int *err_flag)
{
	char *line_ptr;
	char *current_word;
	int err_code;
	char *result;

	result = NULL;
	line_ptr = line;
	get_next_word(&current_word, &line_ptr);

	if(is_symbol_def(current_word)) /*has symbol - skipping it*/
		get_next_word(&current_word, &line_ptr);

	if(is_data_storage_line(current_word)) /*is data storage line*/
		code_data_to_dc(current_word, &line_ptr, data_segment, dc); /* assuming free of errors because of first pass */

	else if(is_entry_def(current_word)) /*is entry line*/
	{
		get_next_word(&current_word, &line_ptr);
		err_code = mark_symbol_as_entry(symbol_table, current_word);
		if(err_code)
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
	{
		code_command(&result, current_word, &line_ptr, symbol_table, externals_usage_list, *ic, &err_code);
		if(!result)
		{
			*err_flag = 1;
			/*command_input_line_error(err_code, line_num);*/ /*TODO handle error code*/
		}
		(*ic) += 4;
	}

	free(current_word);
	return result;
}
