
#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "../hash/tables.h"
#include "../entity/funcao.h"
#include "../entity/variavel.h"
#include "../util/string_util.h"

extern SymTable *lt;
extern SymTable* variaveis;
extern SymTable* funcoes;

// Data stack -----------------------------------------------------------------

#define STACK_SIZE 100

int stack[STACK_SIZE];
int sp; // stack pointer

void push(int x) {
    stack[++sp] = x;
}

int pop() {
    return stack[sp--];
}

void init_stack() {
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
    }
    sp = -1;
}

void print_stack() {
    printf("*** STACK: ");
    for (int i = 0; i <= sp; i++) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

// ----------------------------------------------------------------------------

// Variables memory -----------------------------------------------------------

#define MEM_SIZE 100

int mem[MEM_SIZE];

void store(int addr, int val) {
    mem[addr] = val;
}

int load(int addr) {
    return mem[addr];
}

void init_mem() {
    for (int addr = 0; addr < MEM_SIZE; addr++) {
        mem[addr] = 0;
    }
}

// ----------------------------------------------------------------------------

//#define TRACE
#ifdef TRACE
#define trace(msg) printf("TRACE: %s\n", msg)
#else
#define trace(msg)
#endif

void rec_run_ast(AST *ast);

void run_var_decl(ast){
    trace("var_decl");
    int var_idx = ast_get_data(ast);
}

void run_stmt_seq(AST *ast) {
    trace("stmt_seq");
    int size = get_child_count(ast);
    for (int i = 0; i < size; i++) {
        rec_run_ast(get_child(ast, i));
    }
}

void run_if(AST *ast) {
    trace("if");
    rec_run_ast(get_child(ast, 0));
    int test = pop();
    if (test == 1) {
        rec_run_ast(get_child(ast, 1));
    } else if (test == 0 && get_child_count(ast) == 3) {
        rec_run_ast(get_child(ast, 2));
    }
}

void run_while(AST *ast) {
    trace("while");
    int again = 1;
    while (again) {
        rec_run_ast(get_child(ast, 1)); // Run body.
        rec_run_ast(get_child(ast, 0)); // Run test.
        again = pop();
    }
}

void run_assign(AST *ast) {
    trace("assign");
    rec_run_ast(get_child(ast, 1));
    AST *child = get_child(ast, 0);
    int var_idx = ast_get_data(child);
    store(var_idx, pop());
}

void run_input(AST *ast) {
    trace("input");
    int x;
    printf("input: ");
    scanf("%d", &x);
    push(x);
}

void removeSubstring(char *s,const char *toremove)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

void run_write(AST *ast) {
    trace("write");
    
    AST *child = get_child(ast, 0);
    int index_lt = ast_get_data(child);

    printf("%s", replace(replace(get_literal(lt,index_lt),"\"",""),"\\n","\n"));    
}

void run_output(AST *ast) {
    trace("output");
    rec_run_ast(get_child(ast, 0));

    printf("%d", pop());    
}

#define bin_op() \
    rec_run_ast(get_child(ast, 0)); \
    rec_run_ast(get_child(ast, 1)); \
    int r = pop(); \
    int l = pop()

void run_plus(AST *ast) {
    trace("plus");
    bin_op();
    push(l + r);
}

void run_minus(AST *ast) {
    trace("minus");
    bin_op();
    push(l - r);
}

void run_times(AST *ast) {
    trace("times");
    bin_op();
    push(l * r);
}

void run_over(AST *ast) {
    trace("over");
    bin_op();
    push((int) l / r);
}

void run_lt(AST *ast) {
    trace("lt");
    bin_op();
    push(l < r);
}

void run_le(AST *ast) {
    trace("le");
    bin_op();
    push(l <= r);
}

void run_gt(AST *ast) {
    trace("gt");
    bin_op();
    push(l > r);
}

void run_ge(AST *ast) {
    trace("ge");
    bin_op();
    push(l >= r);
}

void run_eq(AST *ast) {
    trace("eq");
    bin_op();
    push(l == r);
}

void run_neq(AST *ast) {
    trace("neq");
    bin_op();
    push(l != r);
}

void run_num(AST *ast) {
    trace("num");
    push(ast_get_data(ast));
}

void run_var_use(AST *ast) {
    trace("var_use");
    int var_idx = ast_get_data(ast);
    push(load(var_idx));
}

void recurso(AST *ast){
    int i = 0;
    for(i =0; i< get_child_count(ast);i++)
                rec_run_ast(get_child(ast,i));
}

void rec_run_ast(AST *ast) {
    switch(get_kind(ast)) {
       /* case VAR_DECL_NODE:
            run_var_decl(ast);
            break;
       /* case STMT_SEQ_NODE:
            run_stmt_seq(ast);
            break;*/
        case IF_NODE:
            run_if(ast);
            break;
        case WHILE_NODE:
            run_while(ast);
            break;
        case ASSIGN_NODE:
            run_assign(ast);
            break;
        case INPUT_NODE:
            run_input(ast);
            break;
        case OUTPUT_NODE:
            run_output(ast);
            break;
        case WRITE_NODE:
            run_write(ast);
            break;
         case PLUS_NODE:
            run_plus(ast);
            break;
        case MINUS_NODE:
            run_minus(ast);
            break;
        case TIMES_NODE:
            run_times(ast);
            break;
        case OVER_NODE:
            run_over(ast);
            break;
        case LT_NODE:
            run_lt(ast);
            break;
        case LE_NODE:
            run_le(ast);
            break;
        case GT_NODE:
            run_gt(ast);
            break;
        case GE_NODE:
            run_ge(ast);
            break;
        case EQ_NODE:
            run_eq(ast);
            break;
        case NEQ_NODE:
            run_neq(ast);
            break;
       case NUM_NODE:
            run_num(ast);
            break;
        case VAR_USE_NODE:
            run_var_use(ast);
            break;
        default:
            //fprintf(stderr, "Invalid kind: %s!\n", kind2str(get_kind(ast)));
            //exit(1);
            recurso(ast);
    }
}

void run_ast(AST *ast) {
    init_stack();
    init_mem();
    rec_run_ast(ast);
}