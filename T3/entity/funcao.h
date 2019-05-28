
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

SymTable* funcoes;
int aridade;
int escopo;
int parametros;


Funcao* create_funcao(char *name, int line, int aridade);

void declarar_funcao();

void utilizar_funcao();

void validar_aridade();

void print_funcao(Funcao *f,int index);

#endif // FUNCAO_H

