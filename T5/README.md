Fluxo Normal:

$ ./trab5 < program.cm

Gera código Assembly:

$ ./trab5 -d < program.cm

Obs:
Para fazer funçoes recusivas precisei alterar a tm.c
Adicionei a intrunção:
JR ri    -->     PC <- ri

Alterei as instrunções:
STO ri, a(rj)   --> dMen[a+rj] <- ri
LDA ri, a(rj)   --> ri <- dMen[a+rj]