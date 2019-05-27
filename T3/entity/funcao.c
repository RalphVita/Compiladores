#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcao.h"


Funcao* create_funcao(char *name, int line, int aridade) {
    Funcao *f = malloc(sizeof(Funcao));
    strcpy(f->name,name);
    f->line = line;
    f->aridade = aridade;
    return f;
}

void print_funcao(Funcao *f){
	printf("aridade %d -- name: %s, line: %d\n", f->aridade, f->name, f->line);
}
