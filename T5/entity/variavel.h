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

//Tabela de variaveis
SymTable* variaveis;

//Limpa memória e finaliza
void (*variavel_finalizar)(int);


//Cria variavel
Variavel* create_variavel(char *name, int line, int escopo, int tamanho);

//Declarar variavel
int declarar_variavel(char* yytext, int yylineno, int escopo);

//Verifica se variavel utilizada existe
int utilizar_variavel(char *nameVariavel, int line, int escopo);

//Imprime a tabela de variaveis
void print_variavel(Variavel *v,int index);

#endif // VARIAVEIS_H

