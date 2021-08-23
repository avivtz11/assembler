#include <stdlib.h>
#include <stdio.h>


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
