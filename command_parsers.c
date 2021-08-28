#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "parsers_utils.h"
#include "symbol_table.h"

void code_funct(char **result, char *command);
void code_opcode(char **result, char *command);
void code_R_arithmetic_logic_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code);
void code_R_copying_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code);
void code_param(char **result, char **line_ptr, int *err_code, int line_bin_offset, int bin_size);


void(*get_command_parsing_function(char *command))(char **, char *, char **, SymbolTable*, int *)
{
	if((strcmp(command, "add") == 0) || (strcmp(command, "sub") == 0) || (strcmp(command, "and") == 0) || (strcmp(command, "or") == 0) ||
(strcmp(command, "nor") == 0))
		return code_R_arithmetic_logic_to_binary;

	if((strcmp(command, "move") == 0) || (strcmp(command, "mvhi") == 0) || (strcmp(command, "mvlo") == 0))
		return code_R_copying_to_binary;

	return NULL;
}


void code_R_copying_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code)
{
	malloc_with_error((void **)result, 4*8 + 1, "couldn't allocate memory");

	code_opcode(result, command);/*opcode*/
	code_param(result, line_ptr, err_code, 16, 6);/*rd*/
	if(*err_code)
		return;
	memcpy((*result) + 11, "00000", 6);/*rt*/
	code_param(result, line_ptr, err_code, 6, 6);/*rs*/
	if(*err_code)
		return;

	if((**line_ptr != '\n') && (**line_ptr != '\0'))
	{
		*err_code = 4;
		return;
	}

	code_funct(result, command);/*funct*/

	memcpy((*result) + 26, "000000", 7); /*unused*/
	(*result)[32] = '\0';
}


void code_R_arithmetic_logic_to_binary(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, int *err_code)
{
	malloc_with_error((void **)result, 4*8 + 1, "couldn't allocate memory");

	code_opcode(result, command);/*opcode*/
	code_param(result, line_ptr, err_code, 6, 6);/*rs*/
	if(*err_code)
		return;
	code_param(result, line_ptr, err_code, 11, 6);/*rt*/
	if(*err_code)
		return;
	code_param(result, line_ptr, err_code, 16, 6);/*rd*/
	if(*err_code)
		return;

	if((**line_ptr != '\n') && (**line_ptr != '\0'))
	{
		*err_code = 4;
		return;
	}

	code_funct(result, command);/*funct*/

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


void code_funct(char **result, char *command)
{
	if((strcmp(command, "add") == 0) || (strcmp(command, "move") == 0))
		memcpy((*result) + 21, "00001", 6);
	else if((strcmp(command, "sub") == 0) || (strcmp(command, "mvhi") == 0))
		memcpy((*result) + 21, "00010", 6);
	else if((strcmp(command, "and") == 0) || (strcmp(command, "mvlo") == 0))
		memcpy((*result) + 21, "00011", 6);
	else if(strcmp(command, "or") == 0)
		memcpy((*result) + 21, "00100", 6);
	else if(strcmp(command, "nor") == 0)
		memcpy((*result) + 21, "00101", 6);
	/*at this point it can't be anything else (get_command_parsing_function validated it)*/
}


void code_opcode(char **result, char *command)
{
	if((strcmp(command, "add") == 0) || (strcmp(command, "sub") == 0) || (strcmp(command, "and") == 0) || (strcmp(command, "or") == 0) ||
(strcmp(command, "nor") == 0))
		memcpy(*result, "000000", 7);/*0*/
	else if(strcmp(command, "addi") == 0)
		memcpy(*result, "001010", 7);/*10*/
	else if(strcmp(command, "subi") == 0)
		memcpy(*result, "001011", 7);/*11*/
	else if(strcmp(command, "andi") == 0)
		memcpy(*result, "001100", 7);/*12*/
	else if(strcmp(command, "ori") == 0)
		memcpy(*result, "001101", 7);/*13*/
	else if(strcmp(command, "nori") == 0)
		memcpy(*result, "001110", 7);/*14*/
	else if(strcmp(command, "bne") == 0)
		memcpy(*result, "001111", 7);/*15*/
	else if(strcmp(command, "beq") == 0)
		memcpy(*result, "010000", 7);/*16*/
	else if(strcmp(command, "blt") == 0)
		memcpy(*result, "010001", 7);/*17*/
	else if(strcmp(command, "bgt") == 0)
		memcpy(*result, "010010", 7);/*18*/
	else if(strcmp(command, "lb") == 0)
		memcpy(*result, "010011", 7);/*19*/
	else if(strcmp(command, "sb") == 0)
		memcpy(*result, "010100", 7);/*20*/
	else if(strcmp(command, "lw") == 0)
		memcpy(*result, "010101", 7);/*21*/
	else if(strcmp(command, "sw") == 0)
		memcpy(*result, "010110", 7);/*22*/
	else if(strcmp(command, "lh") == 0)
		memcpy(*result, "010111", 7);/*23*/
	else if(strcmp(command, "sh") == 0)
		memcpy(*result, "011000", 7);/*24*/
	else if(strcmp(command, "jmp") == 0)
		memcpy(*result, "011110", 7);/*30*/
	else if(strcmp(command, "la") == 0)
		memcpy(*result, "011111", 7);/*31*/
	else if(strcmp(command, "call") == 0)
		memcpy(*result, "100000", 7);/*32*/
	else if(strcmp(command, "stop") == 0)
		memcpy(*result, "110011", 7);/*63*/
	/*at this point it can't be anything else (get_command_parsing_function validated it)*/
}
