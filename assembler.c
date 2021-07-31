#include <stdio.h>
#include <string.h>

#include "first_pass.h"

void assembler(char *assembly_file_path);
int check_file_ext(char *file_path, char *desired_ext);


int main(int argc, char *argv[])
{
	int i;
	if (argc < 2)
	{
		fprintf(stderr, "please insert input files as args\n");
		return 1;
	}

	for (i = 1; i < argc; i++)
		assembler(argv[i]);

	return 0;
}


void assembler(char *assembly_file_path) /*TODO tests for file errors, line lengths*/
{
	FILE *assembly_fp;

	if (!check_file_ext(assembly_file_path, ".as"))
	{
		fprintf(stderr, "%s: invalid extension\n", assembly_file_path);
		return;
	}

	assembly_fp = fopen(assembly_file_path, "r");
	if (assembly_fp == NULL)
	{
		perror(assembly_file_path);
		return;
	}

	first_pass(assembly_fp);

	fclose(assembly_fp);
}


int check_file_ext(char *file_path, char *desired_ext)
{
	char *dot = strrchr(file_path, '.');
	if (!dot)
		return 0;
	return (strcmp(dot, desired_ext) == 0);
}
	
