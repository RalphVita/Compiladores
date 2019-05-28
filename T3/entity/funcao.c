#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcao.h"

extern char text_id[100];
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
		finalizar();		
	}
}
void utilizar_funcao(){
	strcpy(funcao_atual,text_id);
	int index = lookup_var(funcoes,text_id);
	if(index < 0){
		printf(MSG_004,yylineno,text_id);
		finalizar();
	}
}

void validar_aridade(){
	//printf(funcao_atual);
	
	//return;
	int index = lookup_var(funcoes,funcao_atual);
	Funcao* f = (Funcao*)get_data(funcoes,index);

	if(parametros != f->aridade){
		printf(MSG_007,yylineno,f->name,parametros, f->aridade);
		finalizar();
	}
}


void print_funcao(Funcao *f,int index){
	//Entry 0 -- name: sum, line: 3, arity: 2
	printf("Entry %d -- name: %s, line: %d, arity: %d\n",index, f->name, f->line,f->aridade);
}
