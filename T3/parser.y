
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
#include "tables.h"
#include "parser.h"


int yylex();
void yyerror(const char *s);
void ValidarDeclararVarivel();
void ValidarUtilizarVariavel();

extern int yylineno;
extern char* yytext;

LitTable* lt;
SymTable* st;

%}

%token IF THEN ELSE END REPEAT UNTIL READ WRITE INT PUTS STRING
%token SEMI LPAREN RPAREN
%token ASSIGN

%token NUM
%token ID

%left LT EQ
// Precedence of operators (* and / have higher precedence).
// All four operators are left associative.
%left PLUS MINUS
%left TIMES OVER

// Start symbol for the grammar.
%start program

%%

program:
  stmt_sequence
;

stmt_sequence:
  stmt_sequence stmt
| stmt
;

stmt:
  if_stmt
| repeat_stmt
| assign_stmt
| read_stmt
| write_stmt
| var_decl
| puts_stmt
;

if_stmt:
  IF expr THEN stmt_sequence END
| IF expr THEN stmt_sequence ELSE stmt_sequence END
;

repeat_stmt:
  REPEAT stmt_sequence UNTIL expr
;

assign_stmt:
  ID{ValidarUtilizarVariavel();} ASSIGN expr SEMI
;

read_stmt:
  READ ID{ValidarUtilizarVariavel();} SEMI
;

write_stmt:
  WRITE expr SEMI
;

expr:
  expr LT expr
| expr EQ expr
| expr PLUS expr
| expr MINUS expr
| expr TIMES expr
| expr OVER expr
| LPAREN expr RPAREN
| NUM
| ID{ValidarUtilizarVariavel();}
;

var_decl:INT ID{ValidarDeclararVarivel();} SEMI;
puts_stmt:PUTS STRING {add_literal(lt,yytext); printf("PUTS: %s.\n",yytext);} SEMI;


%%

// Error handling.
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
