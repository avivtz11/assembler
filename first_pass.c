#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 82

char *get_next_word(char **line)


void first_pass(FILE *assembly_fp)
{
	char line[MAX_LINE];
	int line_num;
	char *current_word;
	char *line_ptr;
	int does_line_define_symbol;
	int ic = 100;
	int dc = 0;

	line_num = 1;
	while(fgets(line, sizeof(line), assembly_fp)) /*TODO maybe error if more than MAX_LINE?*/
	{
		line_ptr = line;
		does_line_define_symbol = 0;
		current_word = get_next_word(&line_ptr);
		line_ptr += strlen(current_word);
		while(line + strlen(line) >= line_ptr)
		{
			if((*current_word != '\n') && (*current_word != ';')) /*not comment or empty*/
			{
				if(*(current_word + strlen(current_word) - 1) == ':') /*has symbol*/
				{
					does_line_define_symbol = 1;
					current_word = get_next_word(line_ptr);
					line_ptr += strlen(current_word);
				}

				if((strcmp(current_word, ".dh") == 0) || (strcmp(current_word, ".dw") == 0) || (strcmp(current_word, ".db") == 0) || (strcmp(current_word, ".asciz") == 0)) /*is data storage line*/
				{
					/*TODO insert to symbol table, and put data in data section 7-8*/
				}

			
			}
			free(current_word); /*TODO maybe change this to array*/
		}
	}
}


char *get_next_word(char **current_char)
{
	char *result_word;
	int result_word_length;
	char *first_word_char;

	while((**current_char == ' ') || (**current_char == '\t')) /*skipping prefix whitespace*/
	{
		(*current_char)++;
	}
	first_word_char = *current_char;

	while((**current_char != ' ') && (**current_char != '\t') && (**current_char != '\n')) /*TODO check this*/
	{
		(*current_char)++;
	}

	result_word_length = (*current_char) - first_word_char;
	result_word = (char *) malloc(result_word_length + 1);
	memcpy(result_word, first_word_char, result_word_length);
	result_word[result_word_length] = '\0';
	return result_word;
}
