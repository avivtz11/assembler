#include <stdio.h>

#include "symbol_table.h"

#define MAX_LINE 82


void second_pass(FILE *assembly_fp, SymbolTable *symbol_table, char *data_segment)
{
	char line[MAX_LINE];
	int line_num;
	/*char *current_word;
	char *line_ptr;
	int err_flag;*/

	line_num = 1;
	while(fgets(line, sizeof(line), assembly_fp)) /*TODO maybe error if more than MAX_LINE?*/
	{
		/*line_ptr = line;
		current_word = get_next_word(&line_ptr);
		line_ptr += strlen(current_word);
		while(line + strlen(line) >= line_ptr)
		{
			
		}
		
		free(current_word);*/
	}
	line_num++;
}
