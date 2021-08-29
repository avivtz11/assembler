#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "first_pass.h"
#include "second_pass.h"
#include "utils.h"
#include "output_utils.h"
#include "externals_usage_list.h"
#include "symbol_table.h"
#include "externals_usage_list.h"

void assembler(char *assembly_file_path);
int check_file_ext(char *file_path, char *desired_ext);
void second_pass_prep(SymbolTable *symbol_table, ExternalsUsageList **externals_usage_list, char **data_segment, int ic, int dc);
void free_and_close_all(char *data_segment, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, FILE *ob_fp, FILE *assembly_fp, char *ob_output_file_path);


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
	FILE *ob_fp;
	char *ob_output_file_path;
	char *ext_output_file_path;
	char *ent_output_file_path;
	SymbolTable *symbol_table;
	ExternalsUsageList *externals_usage_list;
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

	make_symbol_table(&symbol_table);
	pass_err_flag = first_pass(assembly_fp, symbol_table, &ic, &dc);
	fclose(assembly_fp);
	if(pass_err_flag)
	{
		free_symbol_table(symbol_table);
		return;
	}

	second_pass_prep(symbol_table, &externals_usage_list, &data_segment, ic, dc);
	assembly_fp = fopen(assembly_file_path, "r");
	if (assembly_fp == NULL)
	{
		free(data_segment);
		free_symbol_table(symbol_table);
		perror(assembly_file_path);
		return;
	}

	change_path_extension(&ob_output_file_path, assembly_file_path, ".ob");
	ob_fp = fopen(ob_output_file_path, "w");
	if (ob_fp == NULL)
	{
		free(ob_output_file_path);
		free(data_segment);
		free_symbol_table(symbol_table);
		perror(ob_output_file_path);
		return;
	}

	print_counters(ob_fp, ic, dc);
	pass_err_flag = second_pass(assembly_fp, symbol_table, externals_usage_list, data_segment, ob_fp);
	if(pass_err_flag)
	{
		remove(ob_output_file_path);
		free_and_close_all(data_segment, symbol_table, externals_usage_list, ob_fp, assembly_fp, ob_output_file_path);
		return;
	}

	output_data_segment(ob_fp, data_segment, ic, dc);

	change_path_extension(&ext_output_file_path, assembly_file_path, ".ext");
	output_externals_usage(ext_output_file_path, externals_usage_list);
	free(ext_output_file_path);
	change_path_extension(&ent_output_file_path, assembly_file_path, ".ent");
	output_symbols(ent_output_file_path, symbol_table, "entry");
	free(ent_output_file_path);

	free_and_close_all(data_segment, symbol_table, externals_usage_list, ob_fp, assembly_fp, ob_output_file_path);
}


void free_and_close_all(char *data_segment, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, FILE *ob_fp, FILE *assembly_fp, char *ob_output_file_path)
{	
	free(data_segment);
	free_externals_usage_list(externals_usage_list);
	free_symbol_table(symbol_table);
	fclose(ob_fp);
	fclose(assembly_fp);
	free(ob_output_file_path);
}


void second_pass_prep(SymbolTable *symbol_table, ExternalsUsageList **externals_usage_list, char **data_segment, int ic, int dc)
{
	make_externals_usage_list(externals_usage_list);
	increment_data_addresses(symbol_table, ic);
	malloc_with_error((void **)data_segment, dc, "failed to allocate memory for data segment");
}


int check_file_ext(char *file_path, char *desired_ext)
{
	char *dot = strrchr(file_path, '.');
	if (!dot)
		return 0;
	return (strcmp(dot, desired_ext) == 0);
}
	
