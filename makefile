assembler: assembler.c first_pass.c
	gcc -Wall -ansi -pedantic -o assembler assembler.c first_pass.c
