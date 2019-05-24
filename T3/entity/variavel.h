#ifndef VARIAVEIS_H
#define VARIAVEIS_H


struct variavel;
typedef struct variavel Variavel;

struct variavel{
  char name[100];
  int line;
  int escopo;
  int tamanho;
};


Variavel* create_variavel(char *name, int line, int escopo, int tamanho);

void print_variavel(Variavel *v);

#endif // VARIAVEIS_H

