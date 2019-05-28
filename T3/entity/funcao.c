#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcao.h"

//Salva o yytext do ID. É atualizado no arquivo .l
extern char text_id[100];

//Salva o nome da função que está sendo chamada atualmenta,
//para depois de contar os parametros, verificar a aridade
char funcao_atual[100];

Funcao* create_funcao(char *name, int line, int aridade) {
    Funcao *f = malloc(sizeof(Funcao));
    strcpy(f->name,name);
    f->line = line;
    f->aridade = aridade;
    return f;
}

void declarar_funcao(){
	aridade = 0;
	escopo++;
	int index = lookup_var(funcoes,yytext);

	if(index < 0) {
		Funcao* f = create_funcao(yytext,yylineno,0);
		add_var(funcoes,yytext,f);
	}
	else{
		Funcao* f = (Funcao*)get_data(funcoes,index);
		printf(MSG_006,yylineno,yytext,f->line);
		finalizar(1);
	}
}

//Verifica se função já foi declarada
void utilizar_funcao(){
	strcpy(funcao_atual,text_id);
	int index = lookup_var(funcoes,text_id);
	if(index < 0){
		printf(MSG_004,yylineno,text_id);
		finalizar(1);
	}
}

void validar_aridade(){
	int index = lookup_var(funcoes,funcao_atual);
	Funcao* f = (Funcao*)get_data(funcoes,index);

	if(parametros != f->aridade){
		printf(MSG_007,yylineno,f->name,parametros, f->aridade);
		finalizar(1);
	}
}


void print_funcao(Funcao *f,int index){
	printf("Entry %d -- name: %s, line: %d, arity: %d\n",index, f->name, f->line,f->aridade);
}
