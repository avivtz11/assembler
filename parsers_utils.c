#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "symbol_table.h"

#define is_in_register_range(X) (((X) >= 0) && ((X) <= 31))
#define is_in_immed_range(X) (((X) >= -32768) && ((X) <= 32767))


int get_next_param(char **params, char **result_param)
{
	int result_param_length;
	char *param_first_char;
	skip_white_space(params);

	if((**params) == ',')
		return -1;

	param_first_char = *params;
	while((**params != ' ') && (**params != '\t') && (**params != '\n') && (**params != ','))
	{
		(*params)++;
	}

	result_param_length = (*params) - param_first_char;
	(*result_param) = (char *) malloc(result_param_length + 1);
	memcpy(*result_param, param_first_char, result_param_length);
	(*result_param)[result_param_length] = '\0';

	skip_white_space(params);

	if((**params) != ',' && (**params) != '\n')
		return -2;

	(*params)++;
	return 0;
}


void code_register(char *register_param, char **coded_param, SymbolTable* symbol_table, int ic, int *err_code)
{
	long int register_number;

	if(*register_param != '$')
	{
		*err_code = 1;
		return;
	}
	register_param++;
	register_number = strtol(register_param, &register_param, 10); /*ok to use because values limited(no negativity problem)*/
	if(*register_param)
	{
		*err_code = 2;
		return;
	}
	if(!is_in_register_range(register_number))
	{
		*err_code = 3;
		return;
	}

	malloc_with_error((void **)coded_param, 6, "couldn't allocate memory");/*register takes 5 + terminator*/
	num2bin(register_number, *coded_param, 6);
}


void code_immed(char *immed_param, char **coded_param, SymbolTable* symbol_table, int ic, int *err_code)
{
	long int immed_value;

	immed_value = strtol(immed_param, &immed_param, 10);
	if(*immed_param)
	{
		*err_code = 2;
		return;
	}
	if(!is_in_immed_range(immed_value))
	{
		*err_code = 3;
		return;
	}

	malloc_with_error((void **)coded_param, 17, "couldn't allocate memory");/*immed takes 17 + terminator*/
	num2bin(immed_value, *coded_param, 17);
}


void code_label_distance(char *label_param, char **coded_param, SymbolTable* symbol_table, int ic, int *err_code)
{
	int label_value;
	long int immed_value;

	label_value = get_internal_label_value(symbol_table, label_param);
	if(label_value == 1)
	{
		*err_code = 4;
		return;
	}

	immed_value = label_value - ic;
	if(!is_in_immed_range(immed_value))
	{
		*err_code = 3;
		return;
	}

	malloc_with_error((void **)coded_param, 17, "couldn't allocate memory");/*immed takes 17 + terminator*/
	num2bin(immed_value, *coded_param, 17);
}
	
