assembler: assembler.c first_pass.c symbol_table.c utils.c
	gcc -Wall -ansi -pedantic -o assembler assembler.c first_pass.c symbol_table.c utils.c
