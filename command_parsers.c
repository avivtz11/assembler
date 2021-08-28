#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "parsers_utils.h"
#include "symbol_table.h"

void get_command_funct(char **result, char *command);
void code_opcode0_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code);
void code_opcode1_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code);
void code_param(char **result, char **line_ptr, int *err_code, int line_bin_offset, int bin_size);


void(*get_command_parsing_function(char *command))(char **, char *, char **, SymbolTable*, int *)
{
	if((strcmp(command, "add") == 0) || (strcmp(command, "sub") == 0) || (strcmp(command, "and") == 0) || (strcmp(command, "or") == 0) ||
(strcmp(command, "nor") == 0))
		return code_opcode0_to_binary;

	if((strcmp(command, "move") == 0) || (strcmp(command, "mvhi") == 0) || (strcmp(command, "mvlo") == 0))
		return code_opcode1_to_binary;

	return NULL;
}


void code_opcode1_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code)
{
	char *command_funct;

	malloc_with_error((void **)result, 4*8 + 1, "couldn't allocate memory");

	memcpy(*result, "000001", 7); /*opcode*/

	code_param(result, line_ptr, err_code, 16, 6);/*rd*/

	memcpy((*result) + 11, "00000", 6);/*rt*/

	code_param(result, line_ptr, err_code, 6, 6);/*rs*/

	if((**line_ptr != '\n') && (**line_ptr != '\0'))
	{
		*err_code = 4;
		return;
	}

	get_command_funct(&command_funct, command);
	memcpy((*result) + 21, command_funct, 6); /*funct*/
	free(command_funct);

	memcpy((*result) + 26, "000000", 7); /*unused*/
	(*result)[32] = '\0';
}


void code_param(char **result, char **line_ptr, int *err_code, int line_bin_offset, int bin_size)
{
	char *current_param;
	char *coded_param;

	*err_code = get_next_param(line_ptr, &current_param);
	if(*err_code)
		return;
	code_register(current_param, &coded_param, err_code);
	free(current_param);
	if(*err_code)
		return;
	memcpy((*result) + line_bin_offset, coded_param, bin_size);
	free(coded_param);
}


void code_opcode0_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code)
{
	char *command_funct;

	malloc_with_error((void **)result, 4*8 + 1, "couldn't allocate memory");

	memcpy(*result, "000000", 7); /*opcode*/

	code_param(result, line_ptr, err_code, 6, 6);/*rs*/
	code_param(result, line_ptr, err_code, 11, 6);/*rt*/
	code_param(result, line_ptr, err_code, 16, 6);/*rd*/

	if((**line_ptr != '\n') && (**line_ptr != '\0'))
	{
		*err_code = 4;
		return;
	}

	get_command_funct(&command_funct, command);
	memcpy((*result) + 21, command_funct, 6); /*funct*/
	free(command_funct);

	memcpy((*result) + 26, "000000", 7); /*unused*/
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
