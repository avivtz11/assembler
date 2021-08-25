#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


void *malloc_with_error(int allocation_size, char *error_message)
{
	void *temp;
	if (!(temp = malloc(allocation_size)))
	{
		fprintf(stderr, "%s", error_message);
		exit(1); /*TODO maybe not exit straight away for sake of cleanup*/
	}
	return temp;
}


void skip_white_space(char **str)
{
	while((**str == ' ') || (**str == '\t'))
	{
		(*str)++;
	}
}


void copy_char_array(char *src, char *dest, int *dest_position)
{
	while((*src) != '\0')
	{
		*(dest + *dest_position) = *src;
		src++;
		(*dest_position)++;
	}
}


char *change_path_extension(char *file_path, char *new_extension)
{
	char *path_pointer;
	char *extension_start;
	char *result;

	extension_start = file_path;
	path_pointer = file_path;
	while(*path_pointer)
	{
		if((*path_pointer) == '.')
			extension_start = path_pointer + 1;
		path_pointer++;
	}

	result = (char *)malloc_with_error(extension_start - path_pointer + strlen(new_extension), "couldn't allocate memory");
	memcpy(result, file_path, extension_start - path_pointer);
	memcpy(result + (extension_start - path_pointer), new_extension, strlen(new_extension));
	return result;
}
