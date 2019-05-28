
#ifndef FUNCAO_H
#define FUNCAO_H
#include "../hash/tables.h"
#include "../menssagem.h"

struct funcao;
typedef struct funcao Funcao;

struct funcao{
  char name[100];
  int line;
  int aridade;
};

extern int yylineno;
extern char* yytext;

//Tabela de funções
SymTable* funcoes;

//----Variaveis globais----
//Usado para incremetar a aridade da função a cada parametro listado em sua declaração
int aridade;

//Armazena o escopo atual. Incremeta a cada declaração de função
int escopo;

//Incremeta a cada parameto listado na chamada de uma função
int parametros;


//Cria uma função
Funcao* create_funcao(char *name, int line, int aridade);

/*
* Adiciona na tabela de simbolos e  verifica se já foi declarada antes
*/
void declarar_funcao();

//Verifica se função já foi declarada
void utilizar_funcao();

//Valida aridade da função
void validar_aridade();

//Imprime as propriedas da função na tela
void print_funcao(Funcao *f,int index);

#endif // FUNCAO_H
