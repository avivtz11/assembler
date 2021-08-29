#ifndef COMMAND_PARSERS_H
#define COMMAND_PARSERS_H

void(*get_command_parsing_function(char *command))(char **, char *, char **, SymbolTable *, int, int *);

#endif/*COMMAND_PARSERS_H*/
