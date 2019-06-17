
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

//Tabela de funções
SymTable* funcoes;

//Limpa memória e finaliza
void (*funcao_finalizar)(int);


//Cria uma função
Funcao* create_funcao(char *name, int line, int aridade);


//Adiciona na tabela de simbolos e  verifica se já foi declarada antes
int declarar_funcao(char *name, int line, int *escopo);

//Verifica se função já foi declarada
void utilizar_funcao(char * funcname, int line);

//Valida aridade da função
void validar_aridade(int line, int nParametros);

//Imprime as propriedas da função na tela
void print_funcao(Funcao *f,int index);

#endif // FUNCAO_H
