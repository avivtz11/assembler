assembler: assembler.c first_pass.c symbol_table.c utils.c parsers.c
	gcc -Wall -ansi -pedantic -o assembler assembler.c first_pass.c symbol_table.c utils.c parsers.c
