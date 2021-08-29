#include <stdio.h>

#include "symbol_table.h"
#include "externals_usage_list.h"

void format_output_bytes(char *current_output_bytes, int bytes_count, FILE *fp);


void print_counters(FILE *fp, int ic, int dc)
{
	fprintf(fp, "%4c %02d %02d\n", ' ', ic-100, dc);
}


void print_ob_output_line(FILE *fp, int *start_byte_index, char *current_output_bytes)
{
	fprintf(fp,"%04d ", *start_byte_index);
	*start_byte_index += 4;
	format_output_bytes(current_output_bytes, 4, fp);
	fputc('\n', fp);
}


void format_output_bytes(char *current_output_bytes, int bytes_count, FILE *fp)
{
	int i;
	for(i = 0; i < bytes_count; i++)
	{
		if(i > 0)
			fputc(' ', fp);
		fprintf(fp, "%02X", (unsigned char)(current_output_bytes[i]));
	}
}


void output_data_segment(FILE *fp, char *data_segment, int ic, int dc)
{
	int i;
	int *ic_ref;

	ic_ref = &ic;
	for(i = 0; (i/4) < (dc/4); i+=4)
	{
		print_ob_output_line(fp, ic_ref, data_segment + i);
	}
	fprintf(fp,"%04d ", *ic_ref);
	format_output_bytes(data_segment + i, dc % 4, fp);	
}


void output_symbols(char *output_file_path, SymbolTable *symbol_table, char *attribute)
{
	FILE *fp;
	int outputs;

	fp = fopen(output_file_path, "w");
	if (fp == NULL)
	{
		perror(output_file_path);
		return;
	}

	outputs = output_symbols_of_attribute(fp, symbol_table, attribute);

	fclose(fp);
	if(!outputs)
		remove(output_file_path);
}


void output_externals_usage(char *output_file_path, ExternalsUsageList *externals_usage_list)
{
	FILE *fp;
	int outputs;

	fp = fopen(output_file_path, "w");
	if (fp == NULL)
	{
		perror(output_file_path);
		return;
	}

	outputs = output_externals_usage_list(fp, externals_usage_list);

	fclose(fp);
	if(!outputs)
		remove(output_file_path);
}
