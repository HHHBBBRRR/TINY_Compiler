#ifndef SYMTAB_H
#define SYMTAB_H

#include "globals.h"

void st_insert ( char * name, int lineno, int loc );
int st_lookup ( char * name );
void printSymTab(FILE * listing);

#endif