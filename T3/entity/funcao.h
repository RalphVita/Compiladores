
#ifndef FUNCOES_H
#define FUNCOES_H


struct funcoes;
typedef struct funcoes FuncoesTable;

FuncoesTable* create_FuncoesTable();



// Symbols Table
// ----------------------------------------------------------------------------

// Opaque structure.
// For simplicity, the table is implemented as a sequential list.
// This table only stores the variable name and the declaration line.
struct sym_table;
typedef struct sym_table SymTable;

// Creates an empty symbol table.
SymTable* create_sym_table();


int add_funcao(SymTable* st, char* name, int line, int aridade);


int lookup_var(SymTable* st, char* names);

// Returns the variable name stored at the given index.
// No check is made by this function, so make sure that the index is valid first.
char* get_name(SymTable* st, int i);

// Returns the declaration line of the variable stored at the given index.
// No check is made by this function, so make sure that the index is valid first.
int get_line(SymTable* st, int i);

// Prints the given table to stdout.
void print_sym_table(SymTable* st);

// Clears the allocated structure.
void free_sym_table(SymTable* st);

#endif // TABLES_H

