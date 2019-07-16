### Fluxo Normal:

$ ./trab5 < program.cm

### Gera código Assembly:

$ ./trab5 -d < program.cm

### Obs:
Para fazer funçoes recusivas e vetores precisei alterar a tm.c
#### Adicionei a instrunção:

| Instrução | Efeito   |
| --------- | ---------|
| JR ri     | PC <- ri |

#### Alterei as instrunções:
| Instrução     | Efeito            |
| ---------     | ---------         |
| STO ri, a(rj) | dMen[a+rj] <- ri  |
| LDA ri, a(rj) | ri <- dMen[a+rj]  | 