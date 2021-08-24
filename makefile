assembler: assembler.c first_pass.c symbol_table.c utils.c parsers.c second_pass.c pass_common.c
	gcc -Wall -ansi -pedantic -o assembler assembler.c first_pass.c symbol_table.c utils.c parsers.c second_pass.c pass_common.c
