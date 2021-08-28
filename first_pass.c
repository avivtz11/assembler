#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbol_table.h"
#include "parsers.h"
#include "utils.h"
#include "pass_common.h"

int is_symbol_name_valid(char *symbol_name);
int is_reserved_word(char *word);
void data_line_error(int err_code, int line_num);
void check_symbol_and_add(SymbolTable *symbol_table, char *symbol_name, int value, char *attributes, int *err_flag, int line_num);
void handle_first_pass_input_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, int line_num, int *err_flag);


int first_pass(FILE *assembly_fp, SymbolTable *symbol_table, int *ic_ref, int *dc_ref)
{
	char line[MAX_LINE];
	int line_num;
	int err_flag;
	int ic = 100;
	int dc = 0;

	line_num = 1;
	err_flag = 0;
	while(fgets(line, sizeof(line), assembly_fp)) /*TODO maybe error if more than MAX_LINE?*/
	{
		if(!is_comment_or_empty(line)) /*not comment or empty*/
		{
			handle_first_pass_input_line(line, symbol_table, &ic, &dc, line_num, &err_flag);
		}

		line_num++;
	}

	*ic_ref = ic;
	*dc_ref = dc;
	return err_flag;
}


void handle_first_pass_input_line(char *line, SymbolTable *symbol_table, int *ic, int *dc, int line_num, int *err_flag)
{
	char *line_ptr;
	int does_line_define_symbol;
	char *current_word;
	char *symbol_name;
	int data_line_length;

	line_ptr = line;
	does_line_define_symbol = 0;
	get_next_word(&current_word, &line_ptr);
	if(is_symbol_def(current_word)) /*has symbol*/
	{
		does_line_define_symbol = 1;
		symbol_name = current_word;
		symbol_name[strlen(symbol_name) - 1] = '\0'; /*removing the : */
		get_next_word(&current_word, &line_ptr);
	}

	if(is_data_storage_line(current_word)) /*is data storage line*/
	{
		if(does_line_define_symbol)
		{
			check_symbol_and_add(symbol_table, symbol_name, *dc, "data", err_flag, line_num);
		}
		data_line_length = count_data_length(current_word, &line_ptr);
		if(data_line_length < 0) /*error flag from count_data_length*/
		{
			*err_flag = 1;
			data_line_error(data_line_length, line_num);
			data_line_length = 0;  /*won't go to second pass so 0 is okay*/
		}
		*dc += data_line_length;
	}

	else if(is_extern_def(current_word))
	{
		get_next_word(&current_word, &line_ptr);
		check_symbol_and_add(symbol_table, current_word, 0, "external", err_flag, line_num);
		if(((*line_ptr) != '\n') && ((*line_ptr) != '\0'))
		{
			*err_flag = 1;
			printf("line %d: extern can only have one param", line_num);
		}
	}

	else if(!is_entry_def(current_word)) /*entry saved for second pass*/
	{
		/*regular command*/
		if(does_line_define_symbol)
		{
			check_symbol_and_add(symbol_table, symbol_name, *ic, "code", err_flag, line_num);
		}
		*ic += 4;
	}

	
	if(does_line_define_symbol)
		free(symbol_name);
	free(current_word);
}


void data_line_error(int err_code, int line_num)
{
	char *err_message;

	switch(err_code)
	{
		case -1:
			err_message = "line %d: redundant comma between params";
			break;
		case -2:
			err_message = "line %d: missing comma between params";
			break;
		case -3:
			err_message = "line %d: data param is not a valid number";
			break;
		case -4:
			err_message = "line %d: data param is not of valid size";
			break;
		case -5:
			err_message = "line %d: asciz only gets one param";
			break;
		case -6:
			err_message = "line %d: string param in asciz must have \"";
			break;
		default:
			err_message = "line %d: unknown error";
	}
	printf(err_message, line_num);
}


void check_symbol_and_add(SymbolTable *symbol_table, char *symbol_name, int value, char *attributes, int *err_flag, int line_num)
{
	if(!is_symbol_name_valid(symbol_name))
	{
		*err_flag = 1;
		printf("line %d: symbol name is invalid, not adding it", line_num);
	}
	/*in case of extern we have no problem with symbol already being in symbol table as external*/
	else if(add_symbol(symbol_table, symbol_name, value, attributes) == 1)
	{
		*err_flag = 1;
		printf("line %d: symbol name already exists, not adding again", line_num);
	}
}


int is_symbol_name_valid(char *symbol_name)
{
	char *current_char = symbol_name;

	if(isalpha(*current_char) == 0)/*first symbol char must be alphabetic*/
		return 0;

	while(*(current_char++) != '\0')/*all chars must be alphanumeric*/
	{
		if(isalnum(*current_char) == 0)
			return 0;
	}

	if(strlen(symbol_name) > 31)
		return 0;

	if(is_reserved_word(symbol_name))
		return 0;

	return 1;
}


int is_reserved_word(char *word)
{
	if(!strcmp(word, "add") == 0)
		return 1;
	else if(!strcmp(word, "sub") == 0)
		return 1;
	else if(!strcmp(word, "and") == 0)
		return 1;
	else if(!strcmp(word, "or") == 0)
		return 1;
	else if(!strcmp(word, "nor") == 0)
		return 1;
	else if(!strcmp(word, "move") == 0)
		return 1;
	else if(!strcmp(word, "mvhi") == 0)
		return 1;
	else if(!strcmp(word, "mvlo") == 0)
		return 1;
	else if(!strcmp(word, "addi") == 0)
		return 1;
	else if(!strcmp(word, "subi") == 0)
		return 1;
	else if(!strcmp(word, "andi") == 0)
		return 1;
	else if(!strcmp(word, "ori") == 0)
		return 1;
	else if(!strcmp(word, "nori") == 0)
		return 1;
	else if(!strcmp(word, "bne") == 0)
		return 1;
	else if(!strcmp(word, "beq") == 0)
		return 1;
	else if(!strcmp(word, "blt") == 0)
		return 1;
	else if(!strcmp(word, "bgt") == 0)
		return 1;
	else if(!strcmp(word, "lb") == 0)
		return 1;
	else if(!strcmp(word, "sb") == 0)
		return 1;
	else if(!strcmp(word, "lw") == 0)
		return 1;
	else if(!strcmp(word, "sw") == 0)
		return 1;
	else if(!strcmp(word, "lh") == 0)
		return 1;
	else if(!strcmp(word, "sh") == 0)
		return 1;
	else if(!strcmp(word, "jmp") == 0)
		return 1;
	else if(!strcmp(word, "la") == 0)
		return 1;
	else if(!strcmp(word, "call") == 0)
		return 1;
	else if(!strcmp(word, "stop") == 0)
		return 1;

	else if(!strcmp(word, "db") == 0)
		return 1;
	else if(!strcmp(word, "dw") == 0)
		return 1;
	else if(!strcmp(word, "dh") == 0)
		return 1;
	else if(!strcmp(word, "asciz") == 0)
		return 1;
	else if(!strcmp(word, "entry") == 0)
		return 1;
	else if(!strcmp(word, "extern") == 0)
		return 1;

	return 0;
}
