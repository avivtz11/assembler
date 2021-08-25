#include <stdio.h>
#include <string.h>

#include "first_pass.h"
#include "second_pass.h"
#include "utils.h"

void assembler(char *assembly_file_path);
int check_file_ext(char *file_path, char *desired_ext);
char *second_pass_prep(SymbolTable *symbol_table, int ic, int dc);


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


void assembler(char *assembly_file_path)
{
	FILE *assembly_fp;
	char *ob_output_file_path;
	SymbolTable *symbol_table;
	char *data_segment;
	int pass_err_flag;
	int dc;
	int ic;

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

	symbol_table = make_symbol_table();
	pass_err_flag = first_pass(assembly_fp, symbol_table, &ic, &dc);
	fclose(assembly_fp);
	if(pass_err_flag == 1)
		return;

	data_segment = second_pass_prep(symbol_table, ic, dc);
	assembly_fp = fopen(assembly_file_path, "r");
	if (assembly_fp == NULL)
	{
		perror(assembly_file_path);
		return;
	}

	ob_output_file_path = change_path_extension(assembly_file_path, "ob");
	ob_fp = fopen(ob_output_file_path, "w");
	free(ob_output_file_path);
	if (ob_fp == NULL)
	{
		perror(assembly_file_path);
		return;
	}
	
	second_pass(assembly_fp, symbol_table, data_segment, ob_fp);

	fclose(assembly_fp);
}


char *second_pass_prep(SymbolTable *symbol_table, int ic, int dc)
{
	char *data_segment;

	increment_data_addresses(symbol_table, ic + 100);
	data_segment = (char *)malloc_with_error(dc, "failed to allocate memory for data segment");

	return data_segment;
}


int check_file_ext(char *file_path, char *desired_ext)
{
	char *dot = strrchr(file_path, '.');
	if (!dot)
		return 0;
	return (strcmp(dot, desired_ext) == 0);
}
	
