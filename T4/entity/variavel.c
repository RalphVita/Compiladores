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

void declarar_variavel(char* yytext, int yylineno, int escopo){

	//Concatena o nome da variavel com escopo. Será a key da variavel na tabela de simbolos
	char nome_escopo[100];
	sprintf(nome_escopo, "%s_%d", yytext, escopo);

	int index = lookup_var(variaveis,nome_escopo);

	if(index < 0) {
		Variavel* v = create_variavel(yytext,yylineno,escopo,0);
		add_var(variaveis,nome_escopo,v);
	}
	else{
		Variavel* v = (Variavel*)get_data(variaveis,index);
		printf(MSG_005,yylineno,yytext,v->line);
		variavel_finalizar(1);
	}
}
void utilizar_variavel(char *nameVariavel, int line, int escopo){
	//Concatena o nome da variavel com escopo. É a key da variavel na tabela de simbolos
	char nome_escopo[100];
	sprintf(nome_escopo, "%s_%d", nameVariavel, escopo);

	int index = lookup_var(variaveis,nome_escopo);

	//Variavel não declarada
	if(index < 0){
		printf(MSG_003,line,nameVariavel);
		variavel_finalizar(1);
	}

}


void print_variavel(Variavel *v,int index){
	printf("Entry %d -- name: %s, line: %d, scope: %d, size: %d\n",index, v->name, v->line,v->escopo,v->tamanho);
}
