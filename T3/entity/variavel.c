#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variavel.h"


Variavel* create_variavel(char *name, int line, int escopo, int tamanho) {
    Variavel *v = malloc(sizeof(Variavel));
    strcpy(v->name,name);
    v->line = line;
    v->escopo = escopo;
    v->tamanho = tamanho;
    return v;
}

void print_variavel(Variavel *v){
	printf("Entry %d -- name: %s, line: %d\n", v->escopo, v->name, v->line);
}



