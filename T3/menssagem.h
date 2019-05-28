#ifndef MENSSAGEM_H
#define MENSSAGEM_H

//Sucesso
#define MSG_001 "PARSE SUCCESSFUL!\n"

//Simbolo não reconhecido
#define MSG_002 "SCANNING ERROR (%d): Unknown symbol %s\n"

//Varivel utilizada, mas não foi declarada
#define MSG_003 "SEMANTIC ERROR (%d): variable '%s' was not declared.\n"

//Chamada de função não declarada
#define MSG_004 "SEMANTIC ERROR (%d): function '%s' was not declared.\n"

//Declarando variavel com mesmo nome de outra no mesmo escopo.
#define MSG_005 "SEMANTIC ERROR (%d): variable '%s' already declared at line %d.\n"

//Declarando função com mesmo nome
#define MSG_006 "SEMANTIC ERROR (%d): function '%s' already declared at line %d.\n"

//Chamando função com numero de argumentos diferente do declarado
#define MSG_007 "SEMANTIC ERROR (%d): function '%s' was called with %d arguments but declared with %d parameters.\n"

#endif // MENSSAGEM_H
