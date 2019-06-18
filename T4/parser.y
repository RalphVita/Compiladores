
/* Options to bison */
// File name of generated parser.
%output "parser.c"
// Produces a 'parser.h'
%defines "parser.h"
// Give proper error messages when a syntax error is found.
%define parse.error verbose
// Enable LAC (lookahead correction) to improve syntax error handling.
%define parse.lac full
// Enable the trace option so that debugging is possible.
%define parse.trace

%{
#include <stdio.h>
#include <stdlib.h>
#include "./hash/tables.h"
#include "./entity/variavel.h"
#include "./entity/funcao.h"
#include "./ast/ast.h"
#include "menssagem.h"

int yylex();
void yyerror(const char *s);


void finalizar(int erro);

extern int yylineno;
extern char* yytext;

//Salva o yytext do ID. É atualizado no arquivo scanner.l
extern char text_id[100];

//Armazena o escopo atual. Incremeta a cada declaração de função
int escopo;

//Usado para incremetar a aridade da função a cada parametro listado em sua declaração
int aridade;

//Tabela de Literais
LitTable* lt;

//Arvore de Sintaxe Abstrata
AST *ast;

%}

%define api.value.type {AST*} // Type of variable yylval;

%token ELSE IF INPUT INT OUTPUT RETURN VOID WHILE WRITE
%token ASSIGN
%token SEMI COMMA LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token NUM ID STRING

%left LT LE GT GE EQ NEQ
%left PLUS MINUS  /* Ops associativos a esquerda. */
%left TIMES OVER  /* Mais para baixo, maior precedencia. */
%%

program: func_decl_list         { ast = $1; };

func_decl_list:
	func_decl_list func_decl    { add_child($1, $2); $$ = $1; }
| 	func_decl                   { $$ = new_subtree(FUNC_LIST_NODE, 1, $1); }
;

func_decl:
    func_header func_body {$$ = new_subtree(FUNC_DECL_NODE, 2, $1, $2);}
;
func_header:
    ret_type func_name LPAREN params RPAREN {((Funcao*)last_data(funcoes))->aridade = aridade; aridade = 0;$$ = new_subtree(FUNC_HEADER_NODE, 2, $2, $4);}
;

func_name:
    ID  {aridade = 0;  $$ = new_node(FUNC_NAME_NODE, declarar_funcao(yytext, yylineno, &escopo));}
;

func_body:
    LBRACE opt_var_decl opt_stmt_list RBRACE {$$ = new_subtree(FUNC_BODY_NODE, 2, $2, $3);};

opt_var_decl:
    %empty          {$$ = new_subtree(VAR_LIST_NODE, 0);}
|   var_decl_list   {$$ = $1;}
;

opt_stmt_list:
    %empty          {$$ = new_subtree(BLOCK_NODE, 0);}
|   stmt_list       {$$ = $1;}
;

ret_type:
    INT
|   VOID
;

params:
    VOID        {$$ = new_subtree(PARAM_LIST_NODE, 0);}
|   param_list  {$$ = $1;}
;
param_list:
    param_list COMMA param  { add_child($1, $3); $$ = $1; }
|   param                   {$$ = new_subtree(PARAM_LIST_NODE, 1, $1);}
;
param:
    param_1                 { $$=$1; }
|   param_1 LBRACK RBRACK   { ((Variavel*)last_data(variaveis))->tamanho = -1; $$ = $1; }
;
param_1:
    INT ID {aridade++; $$ = new_node(VAR_DECL_NODE, declarar_variavel(yytext, yylineno, escopo));}
;

var_decl_list:
    var_decl_list var_decl  { add_child($1, $2); $$ = $1; }
|   var_decl                {$$ = new_subtree(VAR_LIST_NODE, 1, $1);};
;
var_decl:
    var_decl_1 SEMI
|   var_decl_1 LBRACK NUM{((Variavel*)last_data(variaveis))->tamanho = atoi(yytext);} RBRACK SEMI
;

var_decl_1: INT ID  {$$ = new_node(VAR_DECL_NODE, declarar_variavel(yytext, yylineno, escopo));};


stmt_list:
    stmt_list stmt  { add_child($1, $2); $$ = $1; }
|   stmt            {$$ = new_subtree(BLOCK_NODE, 1, $1);}
;
stmt:
    assign_stmt | if_stmt | while_stmt | return_stmt | func_call SEMI;
assign_stmt: lval ASSIGN arith_expr SEMI    {$$ = new_subtree(ASSIGN_NODE, 2, $1, $3);};

lval: lval_1 | lval_1 LBRACK NUM RBRACK | lval_1 LBRACK lval_1 RBRACK;
lval_1: ID  {$$ =  new_node(VAR_USE_NODE, utilizar_variavel(text_id, yylineno, escopo));};

if_stmt:
    IF LPAREN bool_expr RPAREN block                {$$ = new_subtree(IF_NODE, 2, $3, $5);}
|   IF LPAREN bool_expr RPAREN block ELSE block     {$$ = new_subtree(IF_NODE, 3, $3, $5, $7);}
;
block: LBRACE opt_stmt_list RBRACE  				{$$ = $2;};
while_stmt: WHILE LPAREN bool_expr RPAREN block     {$$ = new_subtree(WHILE_NODE, 2, $3, $5);};
return_stmt:
    RETURN SEMI             {$$ = new_subtree(RETURN_NODE, 0);}
|   RETURN arith_expr SEMI  {$$ = new_subtree(RETURN_NODE, 1, $2);}
;
func_call:
    output_call
|   write_call
|   user_func_call
;
input_call: INPUT LPAREN RPAREN                 { $$ = new_subtree(INPUT_NODE, 0);};
output_call: OUTPUT LPAREN arith_expr RPAREN    { $$ = new_subtree(OUTPUT_NODE, 1, $3);};
write_call: WRITE LPAREN STRING {$$ = new_node(STRING_NODE,add_literal(lt,yytext));} RPAREN {$$ = $4;};
user_func_call:
    ID {$$ = new_node(FUNC_CALL_NODE,utilizar_funcao(text_id, yylineno));} LPAREN {aridade=0;} opt_arg_list  RPAREN {validar_aridade(yylineno, aridade); add_child($2, $5); $$ = $2;}
;
opt_arg_list:
    %empty      {$$ = new_subtree(ARG_LIST_NODE, 0);}
|   arg_list
;
arg_list:
    arg_list COMMA arith_expr 	{aridade++; add_child($1, $3); $$ = $1; }
|   arith_expr 					{aridade++;$$ = new_subtree(ARG_LIST_NODE, 1, $1);}
;
bool_expr:
    arith_expr LT arith_expr    { $$ = new_subtree(LT_NODE, 2, $1, $3); }
|   arith_expr LE arith_expr    { $$ = new_subtree(LE_NODE, 2, $1, $3); }
|   arith_expr GT arith_expr    { $$ = new_subtree(GT_NODE, 2, $1, $3); }
|   arith_expr GE arith_expr    { $$ = new_subtree(GE_NODE, 2, $1, $3); }
|   arith_expr EQ arith_expr    { $$ = new_subtree(EQ_NODE, 2, $1, $3); }
|   arith_expr NEQ arith_expr   { $$ = new_subtree(NEQ_NODE, 2, $1, $3); }
;
arith_expr:
    arith_expr PLUS arith_expr  { $$ = new_subtree(PLUS_NODE, 2, $1, $3); }
|   arith_expr MINUS arith_expr { $$ = new_subtree(MINUS_NODE, 2, $1, $3); }
|   arith_expr TIMES arith_expr { $$ = new_subtree(TIMES_NODE, 2, $1, $3); }
|   arith_expr OVER arith_expr  { $$ = new_subtree(OVER_NODE, 2, $1, $3); }
|   LPAREN arith_expr RPAREN    { $$ = $2; }
|   lval
|   input_call
|   user_func_call
|   NUM													{ $$ = new_node(NUM_NODE, atoi(yytext)); }
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
    funcao_finalizar = variavel_finalizar = &finalizar;
    int ret = yyparse();

    if (ret == 0) {
        print_dot(ast);
        free_tree(ast);
    }
    else
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
