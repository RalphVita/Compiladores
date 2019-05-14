%option outfile="scanner.c"
%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <math.h>
#include "parser.h"
#define process_token(type) if(!comentario) return type
void Token(int line, char*  text, char *type) ;
int comentario = 0;
%}



digito  [0-9]
naozero [1-9]
numero  ({naozero}{digito}*)|0
letra	[a-zA-Z]
letranumero ({letra}+{digito}*)
id		({letranumero}*)
%%
\n 
[ \t]+      /* pula espaco em branco - acao: fazer nada. */
\/\/.*
\/\*		{comentario++;};
\*\/		{comentario--;};
else		{ process_token(ELSE); }
if			{ process_token(IF); }
input		{ process_token(INPUT); }
int			{ process_token(INT); }
output		{ process_token(OUTPUT); }
return		{ process_token(RETURN); }
void        { process_token(VOID); }
while		{ process_token(WHILE); } 
write		{ process_token(WRITE); } 

\+			{ process_token(PLUS); }
\-			{ process_token(MINUS); }
\*			{ process_token(TIMES); } 
\/			{ process_token(OVER); }
\<			{ process_token(LT); }
\<=			{ process_token(LE); }
\>			{ process_token(GT); } 
\>=			{ process_token(GE); }
\=\=		{ process_token(EQ); }
\!\=		{ process_token(NEQ); }  
\=			{ process_token(ASSIGN); }

;			{ process_token(SEMI); }
,			{ process_token(COMMA); }
\(			{ process_token(LPAREN); } 
\)			{ process_token(RPAREN); } 
\[			{ process_token(LBRACK); } 
\]			{ process_token(RBRACK); }
\{			{ process_token(LBRACE); } 
\}			{ process_token(RBRACE); } 

{numero}	{ process_token(NUM); }
{id}  		{ process_token(ID); }
\".*\"		{ process_token(STRING); }
.			{ if(!comentario) {printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext); exit(1); }}
%%
