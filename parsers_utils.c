#include <stdlib.h>
#include <string.h>

#include "utils.h"


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


void code_register(char *register_param, char **coded_param, int *err_code)
{
	int register_number;

	if(*register_param != '$')
	{
		*err_code = 1;
		return;
	}
	register_param++;
	register_number = strtol(register_param, &register_param, 10);
	if(*register_param)
	{
		*err_code = 2;
		return;
	}
	if((register_number < 0) || (register_number > 31))
	{
		*err_code = 3;
		return;
	}

	malloc_with_error((void **)coded_param, 6, "couldn't allocate memory");/*register takes 5 + terminator*/
	byte2bin((char)(register_number), *coded_param, 6);
}
