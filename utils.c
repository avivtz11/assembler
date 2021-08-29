#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


void malloc_with_error(void **destination, int allocation_size, char *error_message)
{
	if (!(*destination = malloc(allocation_size)))
	{
		fprintf(stderr, "%s", error_message);
		exit(1); /*TODO maybe not exit straight away for sake of cleanup*/
	}
}


void skip_white_space(char **str)
{
	while((**str == ' ') || (**str == '\t'))
	{
		(*str)++;
	}
}


void change_path_extension(char **result, char *file_path, char *new_extension)
{
	char *path_pointer;
	char *extension_start;

	extension_start = file_path;
	path_pointer = file_path;
	while(*path_pointer)
	{
		if((*path_pointer) == '.')
			extension_start = path_pointer;
		path_pointer++;
	}

	malloc_with_error((void **)result, extension_start - file_path + strlen(new_extension) + 1, "couldn't allocate memory");
	(*result)[extension_start - file_path + strlen(new_extension)] = '\0';
	memcpy(*result, file_path, extension_start - file_path);
	memcpy((*result) + (extension_start - file_path), new_extension, strlen(new_extension));
}


void num2bin(long int num, char *result_buffer, int result_buffer_size)/*TODO make sure 2's complement*/
{
	int i;
	char *temp;

	*(result_buffer + result_buffer_size - 1) = '\0';
	temp = result_buffer + (result_buffer_size - 2);

	for(i = result_buffer_size - 2; i >= 0; i--)
	{
		*temp = (num & 1) + '0';
		temp--;
		num >>= 1;
	}
}


void binary_32_to_bytes(char **result, char *as_binary, int is_little_endian)
{	
	char *temp;
	char *binary_byte;
	int offset;
	int diff;
	int i;

	malloc_with_error((void **)result, 4, "couldn't allocate memory");

	malloc_with_error((void **)&binary_byte, 9, "couldn't allocate memory");
	*(binary_byte + 8) = '\0';

	offset = 0;
	diff = 1;
	if(is_little_endian)
	{
		offset = 3;
		diff = -1;
	}
	for(i = 0; i < 8*4; i+=8)
	{
		memcpy(binary_byte, as_binary + i, 8);
		*((*result) + offset) = strtol(binary_byte, &temp, 2);
		offset += diff;
	}
}
