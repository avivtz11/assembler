#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "parsers_utils.h"
#include "symbol_table.h"

void get_command_funct(char **result, char *command);
void code_opcode0_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code);


void(*get_command_parsing_function(char *command))(char **, char *, char **, SymbolTable*, int *)
{
	if((strcmp(command, "add") == 0) || (strcmp(command, "sub") == 0) || (strcmp(command, "and") == 0) || (strcmp(command, "or") == 0) ||
(strcmp(command, "nor") == 0))
		return code_opcode0_to_binary;
	return NULL;
}


void code_opcode0_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code)
{
	char *current_param;
	char *coded_param;
	int offset;
	int i;
	char *command_funct;

	malloc_with_error((void **)result, 4*8 + 1, "couldn't allocate memory");

	memcpy(*result, "000000", 7); /*opcode*/
	offset = 6;

	for(i = 0; i < 3; i++)
	{
		*err_code = get_next_param(line_ptr, &current_param);
		if(*err_code)
			return;
		code_register(current_param, &coded_param, err_code);
		free(current_param);
		if(*err_code)
			return;
		memcpy((*result) + offset, coded_param, 6);
		free(coded_param);
		offset += 5;
	}
	if((**line_ptr != '\n') && (**line_ptr != '\0'))
	{
		*err_code = 4;
		return;
	}

	get_command_funct(&command_funct, command);
	memcpy((*result) + offset, command_funct, 6); /*funct*/
	free(command_funct);
	offset += 5;
	memcpy((*result) + offset, "000000", 7); /*unused*/
	(*result)[32] = '\0';
}


void get_command_funct(char **result, char *command)
{
	malloc_with_error((void **)result, 6, "couldn't allocate memory");
	if((strcmp(command, "add") == 0) || (strcmp(command, "move") == 0))
		strcpy(*result, "00001");
	else if((strcmp(command, "sub") == 0) || (strcmp(command, "mvhi") == 0))
		strcpy(*result, "00010");
	else if((strcmp(command, "and") == 0) || (strcmp(command, "mvlo") == 0))
		strcpy(*result, "00011");
	else if(strcmp(command, "or") == 0)
		strcpy(*result, "00100");
	else if(strcmp(command, "nor") == 0)
		strcpy(*result, "00101");
	/*at this point it can't be anything else (get_command_parsing_function validated it)*/
}
