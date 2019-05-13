%{ /* Reconhece os artigos em ingles a, an, the */
#include <stdio.h>
void Print(int line, char*  text, char *type) ;
int comentario = 0;
%}
%option yylineno
digito  [0-9]
naozero [1-9]
numero  ({naozero}{digito}*)|0
letra	[a-zA-Z]
id		({letra}+{digito}*{letra}*)
palavras [a-zA-Z]+
%%
\n 
[ \t]+      /* pula espaco em branco - acao: fazer nada. */
\/\/.*
\/\*		{comentario++;};
\*\/		{comentario--;};
void        Print(yylineno, yytext, "VOID");
write		Print(yylineno, yytext, "WRITE"); 
while		Print(yylineno, yytext, "WHILE"); 
return		Print(yylineno, yytext, "RETURN");
if			Print(yylineno, yytext, "IF");
input		Print(yylineno, yytext, "INPUT");
output		Print(yylineno, yytext, "OUTPUT");
else		Print(yylineno, yytext, "ELSE");
int			Print(yylineno, yytext, "INT");
{id}  		Print(yylineno, yytext, "ID");

\=\=		Print(yylineno, yytext, "EQ");
\+			Print(yylineno, yytext, "PLUS");
\-			Print(yylineno, yytext, "MINUS");
\=			Print(yylineno, yytext, "ASSIGN");
\<			Print(yylineno, yytext, "LT");
\*			Print(yylineno, yytext, "TIMES"); 
\/			Print(yylineno, yytext, "OVER");
\<=			Print(yylineno, yytext, "LE");
\>			Print(yylineno, yytext, "GT"); 
\>=			Print(yylineno, yytext, "GE");
\!\=		Print(yylineno, yytext, "NEQ");  

;			Print(yylineno, yytext, "SEMI");
,			Print(yylineno, yytext, "COMMA");
\(			Print(yylineno, yytext, "LPAREN"); 
\)			Print(yylineno, yytext, "RPAREN"); 
\{			Print(yylineno, yytext, "LBRACE"); 
\}			Print(yylineno, yytext, "RBRACE"); 
\[			Print(yylineno, yytext, "LBRACK"); 
\]			Print(yylineno, yytext, "RBRACK");

{numero}	Print(yylineno, yytext, "NUM");
\".*\"		Print(yylineno, yytext, "STRING");
.			Print(yylineno, yytext, "UNKNOWN");
%%
int main() { yylex(); return 0; } 
void Print(int line, char*  text, char *type) 
{ 
	if(comentario == 0)
		printf("%d: %s -> %s\n",line, text,type); 

}
