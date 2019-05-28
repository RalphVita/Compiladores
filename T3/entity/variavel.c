#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "variavel.h"

extern int yylineno;
extern char* yytext;
extern char text_id[100];


Variavel* create_variavel(char *name, int line, int escopo, int tamanho) {
    Variavel *v = malloc(sizeof(Variavel));
    strcpy(v->name,name);
    v->line = line;
    v->escopo = escopo;
    v->tamanho = tamanho;
    return v;
}

void declarar_variavel(){

	char nome_escopo[100];
	sprintf(nome_escopo, "%s_%d", yytext, escopo);
	
	//printf(nome_escopo(yytext));
	//return;

	int index = lookup_var(variaveis,nome_escopo);
	//return;
	//printf("----%d-----",index);
	if(index < 0) {
		Variavel* v = create_variavel(yytext,yylineno,escopo,0);
		add_var(variaveis,nome_escopo,v); 
	}
	else{ 
		Variavel* v = (Variavel*)get_data(variaveis,index);
		printf(MSG_005,yylineno,yytext,v->line);
		finalizar();
	}
}
void utilizar_variavel(){
	char nome_escopo[100];
	sprintf(nome_escopo, "%s_%d", text_id, escopo);
	
	int index = lookup_var(variaveis,nome_escopo);
	
	//Variavel não declarada
	if(index < 0){
		printf(MSG_003,yylineno,text_id);
		finalizar();
	}

}


void print_variavel(Variavel *v,int index){
	//Entry 0 -- name: x, line: 3, scope: 0, size: 0
	printf("Entry %d -- name: %s, line: %d, scope: %d, size: %d\n",index, v->name, v->line,v->escopo,v->tamanho);
}




