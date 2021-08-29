#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "symbol_table.h"
#include "externals_usage_list.h"

#define is_in_register_range(X) (((X) >= 0) && ((X) <= 31))
#define is_in_immed_range(X) (((X) >= -32768) && ((X) <= 32767))
#define is_in_address_range(X) (((X) >= 0) && ((X) <= 33554431))

void read_register_value(long int *register_number, char *register_param, int *err_code);


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
	malloc_with_error((void **)result_param, result_param_length + 1);
	memcpy(*result_param, param_first_char, result_param_length);
	(*result_param)[result_param_length] = '\0';

	skip_white_space(params);

	if((**params) == '\0')
		return 0;

	if((**params) != ',' && (**params) != '\n')
		return -2;

	(*params)++;
	return 0;
}


void read_register_value(long int *register_number, char *register_param, int *err_code)
{
	if(*register_param != '$')
	{
		*err_code = 1;
		return;
	}
	register_param++;
	*register_number = strtol(register_param, &register_param, 10); /*ok to use because values limited(no negativity problem)*/
	if(*register_param)
	{
		*err_code = 2;
		return;
	}
	if(!is_in_register_range(*register_number))
	{
		*err_code = 3;
		return;
	}
}


void code_register(char *register_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code)
{
	long int register_number;

	read_register_value(&register_number, register_param, err_code);
	if(*err_code)
		return;

	malloc_with_error((void **)coded_param, 6);/*register takes 5 + terminator*/
	num2bin(register_number, *coded_param, 6);
}


void code_immed(char *immed_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code)
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

	malloc_with_error((void **)coded_param, 17);/*immed takes 16 + terminator*/
	num2bin(immed_value, *coded_param, 17);
}


void code_label_distance(char *label_param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code)
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

	malloc_with_error((void **)coded_param, 17);/*immed takes 16 + terminator*/
	num2bin(immed_value, *coded_param, 17);
}


void code_register_or_label_address(char *param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code)
{
	long int param_value;
	malloc_with_error((void **)coded_param, 27);/*takes 26 + terminator*/
	if((*param) == '$')
	{
		**coded_param = '1';/*reg*/
		read_register_value(&param_value, param, err_code);		
		if(*err_code)
		{
			free(*coded_param);
			return;
		}
	}
	else
	{
		**coded_param = '0';/*reg*/
		param_value = get_label_value(symbol_table, param);/*if external - zero*/
		if(param_value == 1)
		{
			*err_code = 5;
			return;
		}
		if(!is_in_address_range(param_value))
		{
			*err_code = 3;
			return;
		}
		if(param_value == 0)
			add_to_externals_usage_list(externals_usage_list, ic, param);
	}

	num2bin(param_value, (*coded_param) + 1, 26);
}


void code_label_address(char *param, char **coded_param, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code)
{
	long int param_value;
	malloc_with_error((void **)coded_param, 27);/*takes 26 + terminator*/
	**coded_param = '0';/*reg*/

	param_value = get_label_value(symbol_table, param);/*if external - zero*/
	if(param_value == 1)
	{
		*err_code = 5;
		return;
	}
	if(!is_in_address_range(param_value))
	{
		*err_code = 3;
		return;
	}

	if(param_value == 0)
		add_to_externals_usage_list(externals_usage_list, ic, param);

	num2bin(param_value, (*coded_param) + 1, 26);
}
	
