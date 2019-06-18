#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcao.h"


//Salva o nome da função que está sendo chamada atualmente,
//para depois de contar os parametros, verificar a aridade
char funcao_atual[100];

Funcao* create_funcao(char *name, int line, int aridade) {
    Funcao *f = malloc(sizeof(Funcao));
    strcpy(f->name,name);
    f->line = line;
    f->aridade = aridade;
    return f;
}

int declarar_funcao(char *name, int line, int *escopo){
	(*escopo)++;
	int index = lookup_var(funcoes,name);

	if(index < 0) {
		Funcao* f = create_funcao(name,line,0);
		return add_var(funcoes,name,f);
	}
	else{
		Funcao* f = (Funcao*)get_data(funcoes,index);
		printf(MSG_006,line,name,f->line);
		funcao_finalizar(1);
	}

	return -1;
}

//Verifica se função já foi declarada
int utilizar_funcao(char * funcname, int line){
	strcpy(funcao_atual,funcname);
	int index = lookup_var(funcoes,funcname);
	if(index < 0){
		printf(MSG_004,line,funcname);
		funcao_finalizar(1);
	}
	return index;
}

void validar_aridade(int line, int nParametros){
	int index = lookup_var(funcoes,funcao_atual);
	Funcao* f = (Funcao*)get_data(funcoes,index);

	if(nParametros != f->aridade){
		printf(MSG_007,line,f->name,nParametros, f->aridade);
		funcao_finalizar(1);
	}
}


void print_funcao(Funcao *f,int index){
	printf("Entry %d -- name: %s, line: %d, arity: %d\n",index, f->name, f->line,f->aridade);
}
