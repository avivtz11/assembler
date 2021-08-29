#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "symbol_table.h"


int first_pass(FILE *assembly_fp, SymbolTable *symbol_table, int *ic_ref, int *dc_ref);

#endif/*FIRST_PASS_H*/
