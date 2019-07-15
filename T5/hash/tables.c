
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tables.h"

// Literals Table
// ----------------------------------------------------------------------------

#define LITERAL_MAX_SIZE 128
#define LITERALS_TABLE_MAX_SIZE 100

struct lit_table {
    char t[LITERALS_TABLE_MAX_SIZE][LITERAL_MAX_SIZE];
    int size;
};

LitTable* create_lit_table() {
    LitTable *lt = malloc(sizeof * lt);
    lt->size = 0;
    return lt;
}

int add_literal(LitTable* lt, char* s) {
    for (int i = 0; i < lt->size; i++) {
        if (strcmp(lt->t[i], s) == 0) {
            return i;
        }
    }
    strcpy(lt->t[lt->size], s);
    int idx_added = lt->size;
    lt->size++;
    return idx_added;
}

char* get_literal(LitTable* lt, int i) {
    return lt->t[i];
}

void print_lit_table(LitTable* lt) {
    printf("Literals table:\n");
    for (int i = 0; i < lt->size; i++) {
        printf("Entry %d -- %s\n", i, get_literal(lt, i));
    }
}

void free_lit_table(LitTable* lt) {
    free(lt);
}

// Symbols Table
// ----------------------------------------------------------------------------

#define SYMBOL_MAX_SIZE 128
#define SYMBOL_TABLE_MAX_SIZE 100

typedef struct {
  char name[SYMBOL_MAX_SIZE];
  void*data;
} Entry;

struct sym_table {
    Entry t[SYMBOL_TABLE_MAX_SIZE];
    int size;
};

SymTable* create_sym_table() {
    SymTable *st = malloc(sizeof * st);
    st->size = 0;
    return st;
}

int lookup_var(SymTable* st, char* s) {
     for (int i = 0; i < st->size; i++) {
        if (strcmp(st->t[i].name, s) == 0)  {
            return i;
        }
    }
    return -1;
}

int add_var(SymTable* st, char* s, void* data) {
    strcpy(st->t[st->size].name, s);
    st->t[st->size].data = data;
    int idx_added = st->size;
    st->size++;
    return idx_added;
}

char* get_name(SymTable* st, int i) {
    return st->t[i].name;
}

void* get_data(SymTable* st, int i) {
    return st->t[i].data;
}

void* last_data(SymTable* st){
	return st->t[st->size - 1].data;
}

void each_sym_table(SymTable* st, void call_item_symboltable(void*,int)) {
    for (int i = 0; i < st->size; i++) {
         call_item_symboltable(get_data(st, i),i);
    }
}

void free_sym_table(SymTable* st) {
	for (int i = 0; i < st->size; i++) {
		free(st->t[i].data);
	}
    free(st);
}
