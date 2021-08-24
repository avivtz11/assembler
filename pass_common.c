#include <stdlib.h>
#include <string.h>

#include "pass_common.h"
#include "utils.h"


char *get_next_word(char **current_char)
/*returns empty string if reached end of line - current_char skips the result word as well*/
{
	char *result_word;
	int result_word_length;
	char *first_word_char;

	skip_white_space(current_char); /*skips tabs and spaces*/
	first_word_char = *current_char;

	while((**current_char != ' ') && (**current_char != '\t') && (**current_char != '\n') && (**current_char != '\0'))
	{
		(*current_char)++;
	}

	result_word_length = (*current_char) - first_word_char;
	result_word = (char *) malloc(result_word_length + 1);
	memcpy(result_word, first_word_char, result_word_length);
	result_word[result_word_length] = '\0';
	return result_word;
}
