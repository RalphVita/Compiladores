
#ifndef FUNCAO_H
#define FUNCAO_H


struct funcao;
typedef struct funcao Funcao;

struct funcao{
  char name[100];
  int line;
  int aridade;
};


Funcao* create_funcao(char *name, int line, int aridade);

void print_funcao(Funcao *f);

#endif // FUNCAO_H

