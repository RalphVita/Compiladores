
/* Options to bison */
// File name of generated parser.
%output "parser.c"
// Produces a 'parser.h'
%defines "parser.h"
// Give proper error messages when a syntax error is found.
%define parse.error verbose
// Enable LAC (lookahead correction) to improve syntax error handling.
%define parse.lac full

%{
#include <stdio.h>
#include <stdlib.h>
#include "./hash/tables.h"
#include "./entity/variavel.h"
#include "./entity/funcao.h"
#include "menssagem.h"

int yylex();
void yyerror(const char *s);


void finalizar(int erro);

extern int yylineno;
extern char* yytext;
extern char text_id[100];


LitTable* lt;
%}

%token ELSE IF INPUT INT OUTPUT RETURN VOID WHILE WRITE
%token ASSIGN
%token SEMI COMMA LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE 
%token NUM ID STRING

%left LT LE GT GE EQ NEQ
%left PLUS MINUS  /* Ops associativos a esquerda. */
%left TIMES OVER  /* Mais para baixo, maior precedencia. */
%%

program: func_decl_list;

func_decl_list:
	func_decl_list func_decl
| 	func_decl
;

func_decl: func_header func_body;
func_header: ret_type ID{declarar_funcao();} LPAREN params RPAREN {((Funcao*)last_data(funcoes))->aridade = aridade; aridade = 0;};
func_body: LBRACE opt_var_decl opt_stmt_list RBRACE;

opt_var_decl: %empty | var_decl_list;
opt_stmt_list: %empty | stmt_list;
ret_type: INT | VOID;
params: VOID | param_list;
param_list: param_list COMMA param | param;
param: param_1 | param_1 LBRACK RBRACK{((Variavel*)last_data(variaveis))->tamanho = -1;};
param_1: INT ID {aridade++;declarar_variavel();};

var_decl_list: var_decl_list var_decl | var_decl;
var_decl: var_decl_1 SEMI | var_decl_1 LBRACK NUM{((Variavel*)last_data(variaveis))->tamanho = atoi(yytext);} RBRACK SEMI;

var_decl_1: INT ID{declarar_variavel();};


stmt_list: stmt_list stmt | stmt;
stmt: assign_stmt | if_stmt | while_stmt | return_stmt | func_call SEMI;
assign_stmt: lval ASSIGN arith_expr SEMI;

lval: lval_1 | lval_1 LBRACK NUM RBRACK | lval_1 LBRACK ID RBRACK;
lval_1: ID{utilizar_variavel();};

if_stmt: IF LPAREN bool_expr RPAREN block | IF LPAREN bool_expr RPAREN block ELSE block;
block: LBRACE opt_stmt_list RBRACE;
while_stmt: WHILE LPAREN bool_expr RPAREN block;
return_stmt: RETURN SEMI | RETURN arith_expr SEMI;
func_call: output_call | write_call | user_func_call;
input_call: INPUT LPAREN RPAREN;
output_call: OUTPUT LPAREN arith_expr RPAREN;
write_call: WRITE LPAREN STRING{add_literal(lt,yytext);} RPAREN;
user_func_call: ID{utilizar_funcao();} LPAREN{parametros=0;} opt_arg_list  RPAREN{validar_aridade();};
opt_arg_list: %empty | arg_list;
arg_list: arg_list COMMA arith_expr{parametros++;} | arith_expr{parametros++;} ;
bool_expr: arith_expr LT arith_expr 
| arith_expr LE arith_expr 
| arith_expr GT arith_expr
| arith_expr GE arith_expr 
| arith_expr EQ arith_expr 
| arith_expr NEQ arith_expr
;
arith_expr: arith_expr PLUS arith_expr 
| arith_expr MINUS arith_expr
| arith_expr TIMES arith_expr 
| arith_expr OVER arith_expr
| LPAREN arith_expr RPAREN 
| lval
| input_call 
| user_func_call 
| NUM
;

%%

void yyerror (char const *s) {
    printf("PARSE ERROR (%d): %s\n", yylineno, s);
}

//Liberar memória e finaliza
void finalizar(int erro){
	free_lit_table(lt);
    free_sym_table(variaveis);
    free_sym_table(funcoes);
    
    if(erro)
    	exit(erro);
}


// Main.
int main() {
	aridade = 0;
	escopo = -1;
    lt = create_lit_table();
    variaveis = create_sym_table();
    funcoes = create_sym_table();
    int ret = yyparse();

    if (ret == 0) {
        printf(MSG_001);
        printf("\n");
        
        //Writes
        print_lit_table(lt);
	    printf("\n\n");
	    
	    //Variaveis
	    printf("Variables table:\n");
	    print_sym_table(variaveis, print_variavel);
	    printf("\n\n");
	    
	    //Funções
	    printf("Functions table:\n");
	    print_sym_table(funcoes, print_funcao);
    }
    
    finalizar(0);
    return 0;
}
