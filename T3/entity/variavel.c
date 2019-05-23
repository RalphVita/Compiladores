
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variavel.h"


// Symbols Table
// ----------------------------------------------------------------------------

//#define SYMBOL_MAX_SIZE 128
//#define SYMBOL_TABLE_MAX_SIZE 100



Variavel* create_variavel(char *name, int line, int escopo, int tamanho) {
    Variavel *v = malloc(sizeof(Variavel));
    strcpy(v->name,name);
    v->line = line;
    v->escopo = escopo;
    v->tamanho = tamanho;
    return v;
}

void print_variavel(Variavel *v){
	printf("Entry %d -- name: %s, line: %d\n", v->escopo, v->name, v->line/*, get_line(st, i)*/);
}



