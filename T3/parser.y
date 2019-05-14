
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

int yylex();
void yyerror(const char *s);
void ValidarDeclararVarivel();
void ValidarUtilizarVariavel();

extern int yylineno;
extern char* yytext;

LitTable* lt;
SymTable* st;

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
func_header: ret_type ID LPAREN params RPAREN;
func_body: LBRACE opt_var_decl opt_stmt_list RBRACE;
opt_var_decl: %empty | var_decl_list;
opt_stmt_list: %empty | stmt_list;
ret_type: INT | VOID;
params: VOID | param_list;
param_list: param_list COMMA param | param;
param: INT ID | INT ID LBRACK RBRACK;
var_decl_list: var_decl_list var_decl | var_decl;
var_decl: INT ID SEMI | INT ID LBRACK NUM RBRACK SEMI;
stmt_list: stmt_list stmt | stmt;
stmt: assign_stmt | if_stmt | while_stmt | return_stmt | func_call SEMI;
assign_stmt: lval ASSIGN arith_expr SEMI;

lval: ID | ID LBRACK NUM RBRACK | ID LBRACK ID RBRACK;
if_stmt: IF LPAREN bool_expr RPAREN block | IF LPAREN bool_expr RPAREN block ELSE block;
block: LBRACE opt_stmt_list RBRACE;
while_stmt: WHILE LPAREN bool_expr RPAREN block;
return_stmt: RETURN SEMI | RETURN arith_expr SEMI;
func_call: output_call | write_call | user_func_call;
input_call: INPUT LPAREN RPAREN;
output_call: OUTPUT LPAREN arith_expr RPAREN;
write_call: WRITE LPAREN STRING RPAREN;
user_func_call: ID LPAREN opt_arg_list RPAREN;
opt_arg_list: %empty | arg_list;
arg_list: arg_list COMMA arith_expr | arith_expr;
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

void ValidarDeclararVarivel(){
	int index = lookup_var(st,yytext);
	if(index < 0) 
		add_var(st,yytext,yylineno); 
	else{ 
		printf("SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n",yylineno,yytext,get_line(st,index));
		exit(1);
	}
}
void ValidarUtilizarVariavel(){
	int index = lookup_var(st,yytext);
	if(index < 0){
		printf("SEMANTIC ERROR (%d): variable '%s' was not declared.\n",yylineno,yytext);
		exit(1);
	}
}


// Main.
int main() {
    lt = create_lit_table();
    st = create_sym_table();
    int ret = yyparse();

    if (ret == 0) {
        //printf("PARSE SUCCESSFUL!\n");
        printf("\n\n");
        print_lit_table(lt);
	    printf("\n\n");
	    print_sym_table(st);
    }
    
    free_lit_table(lt);
    free_sym_table(st);
    return 0;
}
