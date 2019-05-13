
%option outfile="scanner.c"
%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include "parser.h"

#define process_token(type) return type
%}

number      [0-9]+
identifier  [a-zA-Z]+
ignore      [ \t\n]+

/* Multi-line comments are handled in the rules section.
   First we need to define a start condition. */
%x          MLC

%%

"{"             { BEGIN(MLC); } /* Enter special state. */
<MLC>"}"        { BEGIN(INITIAL); } /* Go back to normal state. */
<MLC>.|\n       { } /* Ignore everything that is inside a comment. */

{ignore}        { } /* Eat up whitespace. */

"if"            { process_token(IF); }
"then"          { process_token(THEN); }
"else"          { process_token(ELSE); }
"end"           { process_token(END); }
"repeat"        { process_token(REPEAT); }
"until"         { process_token(UNTIL); }
"read"          { process_token(READ); }
"write"         { process_token(WRITE); }

"int"         	{ process_token(INT); }
"puts"         	{ process_token(PUTS); }

"+"             { process_token(PLUS); }
"-"             { process_token(MINUS); }
"*"             { process_token(TIMES); }
"/"             { process_token(OVER); }

"="             { process_token(EQ); }
"<"             { process_token(LT); }

"("             { process_token(LPAREN); }
")"             { process_token(RPAREN); }
";"             { process_token(SEMI); }

":="            { process_token(ASSIGN); }

{number}        { process_token(NUM); }
{identifier}    { process_token(ID); }
\".*\"			{ process_token(STRING); }

                /* Be sure to keep this as the last rule */
.               { printf("SCANNING ERROR (%d): Unknown symbol %s\n", yylineno, yytext);
                  exit(1); }

%%
