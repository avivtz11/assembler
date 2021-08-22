#include <stdlib.h>

#include "utils.h"

#define is_byte_size(X) ((-128 <= (X)) && ((X) <= 127))
#define is_half_word_size(X) ((-32768 <= (X)) && ((X) <= 32767))
#define is_word_size(X) ((-2147483648 <= (X)) && ((X) <= 2147483647))

int get_next_param(char **params, char **result_param);


int count_data_length(char *data_command, char **params)
{
	int param_err_code;
	char *current_param;
	char *current_param_iterator;
	long int current_param_value;
	int params_counter;

	params_counter = 0;
	while((*params) != '\n')
	{
		param_err_code = get_next_param(params, &current_param);
		if(param_err_code != 0)
			return param_err_code;

		current_param_iterator = current_param;
		current_param_value = strtol(current_param, &current_param_iterator, 10);
		if(!*current_param_iterator)
			return -3;

		if(data_command == ".db" && !is_byte_size(current_param_value))
			return -4;
		if(data_command == ".dh" && !is_half_word_size(current_param_value))
			return -4;
		if(data_command == ".dw" && !is_word_size(current_param_value))
			return -4;

		params_counter++;
	}
	return params_counter;
}

int get_next_param(char **params, char **result_param)
{
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

	if((**params) != ',')
		return -2;

	(*params)++;
	return 0;
}
