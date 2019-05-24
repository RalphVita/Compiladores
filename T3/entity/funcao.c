#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcoes.h"


Funcao* create_funcao(char *name, int line, int aridade) {
    Funcao *f = malloc(sizeof(Funcao));
    strcpy(f->name,name);
    f->line = line;
    f->aridade = aridade;
    return f;
}

void print_funcao(Funcao *f){
	printf("Entry %d -- name: %s, line: %d\n", f->aridade, f->name, f->line);
}
