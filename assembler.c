/*Aviv Tzitayat 315111492 - maman 14*/

/* This is a program that assembles an assembly code in the studied language 
input - assembly files in a correct format
output - .ob file containing the code and data segmments
	 .ext file containing the locations and names of used external labels
	 .ent file containing the names and location of declaration of entries
	all names are as the input file.
	if no ext or ent are present, the files are not created*/

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
void free_and_close_all(char *data_segment, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, FILE *ob_fp, FILE *assembly_fp);
int handle_first_pass(char *assembly_file_path, SymbolTable **symbol_table, int *ic, int *dc);
int handle_second_pass_prep(FILE **assembly_fp, char *assembly_file_path, SymbolTable **symbol_table, ExternalsUsageList **externals_usage_list, char **data_segment, int ic, int dc);
int handle_second_pass(char *assembly_file_path, FILE *assembly_fp, FILE **ob_fp, char **data_segment, SymbolTable **symbol_table, ExternalsUsageList **externals_usage_list, int ic, int dc);


int main(int argc, char *argv[])
{
	int i;
	if (argc < 2)
	{
		fprintf(stderr, "please insert input files as args\n");
		return 1;
	}

	for (i = 1; i < argc; i++)
	{
		if (!check_file_ext(argv[i], ".as"))
			fprintf(stderr, "%s: invalid extension\n", argv[i]);
		else
			assembler(argv[i]);
	}

	return 0;
}


void assembler(char *assembly_file_path)
/* this function contains the main flow of the program-
we divide the process to 2 passes on the input:

first pass - validating and counting data definitions, building symbol table
second pass - populating the data segment, validates and outputs the commands using the symbol table when needed*/
{
	FILE *assembly_fp;
	FILE *ob_fp;
	char *ext_output_file_path;
	char *ent_output_file_path;
	SymbolTable *symbol_table;
	ExternalsUsageList *externals_usage_list;
	char *data_segment;
	int dc;
	int ic;

	if(handle_first_pass(assembly_file_path, &symbol_table, &ic, &dc))
		return;/*error*/

	if(handle_second_pass_prep(&assembly_fp, assembly_file_path, &symbol_table, &externals_usage_list, &data_segment, ic, dc))
		return;/*error*/

	if(handle_second_pass(assembly_file_path, assembly_fp, &ob_fp, &data_segment, &symbol_table, &externals_usage_list, ic, dc))
		return;/*error*/

	output_data_segment(ob_fp, data_segment, ic, dc);

	change_path_extension(&ext_output_file_path, assembly_file_path, ".ext");
	output_externals_usage(ext_output_file_path, externals_usage_list);
	free(ext_output_file_path);

	change_path_extension(&ent_output_file_path, assembly_file_path, ".ent");
	output_symbols(ent_output_file_path, symbol_table, "entry");
	free(ent_output_file_path);

	free_and_close_all(data_segment, symbol_table, externals_usage_list, ob_fp, assembly_fp);
}


int handle_second_pass(char *assembly_file_path, FILE *assembly_fp, FILE **ob_fp, char **data_segment, SymbolTable **symbol_table, ExternalsUsageList **externals_usage_list, int ic, int dc)
/* this function operates the second pass - returns error flag*/
{
	int pass_err_flag;
	char *ob_output_file_path;

	change_path_extension(&ob_output_file_path, assembly_file_path, ".ob");
	*ob_fp = fopen(ob_output_file_path, "w");
	if (*ob_fp == NULL)
	{
		free(ob_output_file_path);
		free(*data_segment);
		free_symbol_table(*symbol_table);
		perror(ob_output_file_path);
		return 1;
	}

	print_counters(*ob_fp, ic, dc);
	pass_err_flag = second_pass(assembly_fp, *symbol_table, *externals_usage_list, *data_segment, *ob_fp);
	if(pass_err_flag)
	{
		remove(ob_output_file_path);
		free_and_close_all(*data_segment, *symbol_table, *externals_usage_list, *ob_fp, assembly_fp);
		return 1;
	}
	return 0;
}


int handle_second_pass_prep(FILE **assembly_fp, char *assembly_file_path, SymbolTable **symbol_table, ExternalsUsageList **externals_usage_list, char **data_segment, int ic, int dc)
/*this function operates the prep for second pass - returns error flag*/
{
	second_pass_prep(*symbol_table, externals_usage_list, data_segment, ic, dc);
	*assembly_fp = fopen(assembly_file_path, "r");
	if (*assembly_fp == NULL)
	{
		free(*data_segment);
		free_symbol_table(*symbol_table);
		free_externals_usage_list(*externals_usage_list);
		perror(assembly_file_path);
		return 1;
	}
	return 0;
}


int handle_first_pass(char *assembly_file_path, SymbolTable **symbol_table, int *ic, int *dc)
/*this function operates the first pass - returns error flag*/
{
	int pass_err_flag;
	FILE *assembly_fp;

	assembly_fp = fopen(assembly_file_path, "r");
	if (assembly_fp == NULL)
	{
		perror(assembly_file_path);
		return 1;
	}

	make_symbol_table(symbol_table);
	pass_err_flag = first_pass(assembly_fp, *symbol_table, ic, dc);
	fclose(assembly_fp);
	if(pass_err_flag)
	{
		free_symbol_table(*symbol_table);
		return 1; /*error printed in first pass*/
	}
	return 0;
}


void free_and_close_all(char *data_segment, SymbolTable *symbol_table, ExternalsUsageList *externals_usage_list, FILE *ob_fp, FILE *assembly_fp)
{	
	free(data_segment);
	free_externals_usage_list(externals_usage_list);
	free_symbol_table(symbol_table);
	fclose(ob_fp);
	fclose(assembly_fp);
}


void second_pass_prep(SymbolTable *symbol_table, ExternalsUsageList **externals_usage_list, char **data_segment, int ic, int dc)
/*this function creates the externals_usage_list, and the data_segment array. also fixes the symbol table addresses accordingly*/
{
	make_externals_usage_list(externals_usage_list);
	increment_data_addresses(symbol_table, ic);
	malloc_with_error((void **)data_segment, dc);
}


int check_file_ext(char *file_path, char *desired_ext)
{
	char *dot = strrchr(file_path, '.');
	if (!dot)
		return 0;
	return (strcmp(dot, desired_ext) == 0);
}
	
