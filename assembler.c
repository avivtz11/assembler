#include <stdio.h>
#include <string.h>

void assembler(char *assembly_file_path);
int check_file_ext(char *file_path, char *desired_ext);

#define MAX_LINE 82


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
	char line[MAX_LINE];
	int line_num;

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

	line_num = 1;
	while(fgets(line, sizeof(line), assembly_fp)) /*TODO maybe error if more than MAX_LINE?*/
	{
		printf("%d : %s\n", line_num, line);
		line_num++;
	}

	fclose(assembly_fp);
}


int check_file_ext(char *file_path, char *desired_ext)
{
	char *dot = strrchr(file_path, '.');
	if (!dot)
		return 0;
	return (strcmp(dot, desired_ext) == 0);
}
	
