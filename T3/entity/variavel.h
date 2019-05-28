#ifndef VARIAVEIS_H
#define VARIAVEIS_H
#include "../hash/tables.h"
#include "../menssagem.h"

struct variavel;
typedef struct variavel Variavel;

struct variavel{
  char name[100];
  int line;
  int escopo;
  int tamanho;
};

SymTable* variaveis;

extern int escopo;


Variavel* create_variavel(char *name, int line, int escopo, int tamanho);

void declarar_variavel();
void utilizar_variavel();

void print_variavel(Variavel *v,int index);

#endif // VARIAVEIS_H

