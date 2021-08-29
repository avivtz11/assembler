#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "parsers_utils.h"
#include "command_parsers.h"

#define is_byte_size(X) ((-128 <= (X)) && ((X) <= 127))
#define is_half_word_size(X) ((-32768 <= (X)) && ((X) <= 32767))
#define is_word_size(X) (((-2147483647L - 1) <= (X)) && ((X) <= 2147483647L))

int get_next_param(char **params, char **result_param);
int size_of_single_number(char *data_command);
int count_asciz_data_length(char **params);
int split_number_to_bytes(char **result, long int value, int is_little_endian, char *data_command);


void code_command(char **result, char *command, char **line_ptr, SymbolTable* symbol_table, ExternalsUsageList *externals_usage_list, int ic, int *err_code)
{
	char *as_binary;
	void(*code_command_to_binary)(char **, char *, char **, SymbolTable *, ExternalsUsageList *, int, int *);

	code_command_to_binary = get_command_parsing_function(command);
	code_command_to_binary(&as_binary, command, line_ptr, symbol_table, externals_usage_list, ic, err_code);
	if(!as_binary)
		return;

	binary_32_to_bytes(result, as_binary, 1);

	free(as_binary);
}


void code_data_to_dc(char *data_command, char **params, char *data_segment, int *dc)
/* assuming free of errors */
{
	char *current_param;
	char *current_param_iterator;
	long int current_param_value;
	char *data_bytes_temp;
	int counter;
	int current_number_size;

	if(strcmp(data_command, ".asciz") == 0)
	{
		get_next_param(params, &current_param);
		current_param_iterator = current_param;
		current_param_iterator++; /*skipping first " */

		malloc_with_error((void **)&data_bytes_temp, strlen(current_param) - 2 + 1); /*removing 2 for the 2 ", adding 1 for terminator */
		counter = 0;
		while((*current_param_iterator) != '"')
		{
			*(data_bytes_temp + counter) = *current_param_iterator;
			counter++;
			current_param_iterator++;
		}
		*(data_bytes_temp + counter) = '\0'; /*marking end of data*/
		memcpy(data_segment + *dc, data_bytes_temp, counter + 1);
		(*dc) += counter+1;

		free(data_bytes_temp);
		free(current_param);
		return;
	}

	while((**params) != '\0')
	{
		get_next_param(params, &current_param);

		current_param_iterator = current_param;
		current_param_value = atoi(current_param);

		current_number_size = split_number_to_bytes(&data_bytes_temp, current_param_value, 1, data_command);
		memcpy(data_segment + *dc, data_bytes_temp, current_number_size);
		(*dc) += current_number_size;

		free(data_bytes_temp);
		free(current_param);
	}
}


int split_number_to_bytes(char **result, long int value, int is_little_endian, char *data_command)
{
	int size;
	int diff;
	int counter;
	int position;
	int mask = 255; /* 00 00 00 FF */

	diff = 1;
	counter = 0;
	position = 0;

	if(strcmp(data_command, ".db") == 0)
		size = 1;
	else if(strcmp(data_command, ".dh") == 0)
		size = 2;
	else if(strcmp(data_command, ".dw") == 0)
		size = 4;

	if(!is_little_endian) /*inserting bytes in reverse order*/
	{
		position = size - 1;
		diff = -1;
	}

	malloc_with_error((void **)result, size);

	while(counter < size)
	{
		*(*result + position) = (char)((value & mask) >> (8 * counter)); /*leaves only bits that are on in mask as well*/

		mask <<= 8;
		position += diff;
		counter++;
	}
	return size;
}


int count_data_length(char *data_command, char **params)
{
	int param_err_code;
	char *current_param;
	char *current_param_iterator;
	long int current_param_value;
	int params_counter;

	if(strcmp(data_command, ".asciz") == 0)
		return count_asciz_data_length(params);

	params_counter = 0;
	while((**params) != '\0')
	{
		param_err_code = get_next_param(params, &current_param);
		if(param_err_code != 0)
		{
			free(current_param);
			return param_err_code;
		}

		current_param_iterator = current_param;
		current_param_value = strtol(current_param, &current_param_iterator, 10); /*long int so no sign problem*/
		if(*current_param_iterator)
		{
			free(current_param);
			return -3;
		}

		if(((strcmp(data_command, ".db") == 0) && !is_byte_size(current_param_value)) || 
			((strcmp(data_command, ".dh") == 0) && !is_half_word_size(current_param_value)) ||
			((strcmp(data_command, ".dw") == 0) && !is_word_size(current_param_value)))
		{
			free(current_param);
			return -4;
		}

		params_counter++;
		free(current_param);
	}
	return params_counter * size_of_single_number(data_command);
}


int count_asciz_data_length(char **params)
{
	int param_err_code;
	char *current_param;
	int result;

	param_err_code = get_next_param(params, &current_param);
	if(param_err_code != 0)
		result = param_err_code;

	else if((**params) != '\0')
		result = -5;

	else if(((*current_param) != '"') || ((*(current_param + strlen(current_param) - 1)) != '"'))
		result = -6;

	else
		result = strlen(current_param) - 2 + 1;/*removing 2 for the " */

	free(current_param);
	return result;
}


int size_of_single_number(char *data_command)
{
	if(strcmp(data_command, ".db") == 0)
		return 1;
	else if(strcmp(data_command, ".dh") == 0)
		return 2;
	else /*can't be anything else at this point*/
		return 4;
}
