LDC r5, 50
JMP 111
ADD r10, r10, r5
STO r1, 23
STO r13, 27
STO r12, 25
STO r11, 24
LDA r21, 25
STO r21, 33
LDA r23, 25
ADD r3, r10, r0
SUB r10, r10, r10
LDA r22, 1(r23)
ADD r10, r10, r3
STO r22, 31
LDC r24, 1
LDA r25, 25
ADD r26, r25, r24
STO r26, 29
LDA r27, 27
LDA r28, 29
SUB r29, r27, r28
JON r29, 22
LDA r31, 29
ADD r3, r10, r0
SUB r10, r10, r10
LDA r30, 1(r31)
ADD r10, r10, r3
LDA r32, 31
SUB r33, r32, r30
JON r33, 9
LDA r35, 29
ADD r3, r10, r0
SUB r10, r10, r10
LDA r34, 1(r35)
ADD r10, r10, r3
STO r34, 31
LDA r36, 29
STO r36, 33
LDC r37, 1
LDA r38, 29
ADD r39, r38, r37
STO r39, 29
JMP 19
LDA r40, 33
ADD r2, r40, r0
LDA r1, 23
SUB r10, r10, r5
JR r1
ADD r10, r10, r5
STO r1, 23
STO r13, 27
STO r12, 25
STO r11, 24
LDA r41, 25
STO r41, 29
LDC r42, 1
LDA r43, 27
SUB r44, r43, r42
LDA r45, 29
SUB r46, r44, r45
JON r46, 47
LDC r12, 24
ADD r12, r12, r10
LDA r3, 0
STO r12, 0
LDA r11, 0
STO r3, 0
LDA r13, 29
LDA r3, 0
STO r13, 0
LDA r12, 0
STO r3, 0
LDA r14, 27
LDA r3, 0
STO r14, 0
LDA r13, 0
STO r3, 0
LDC r1, 80
JMP 2
STO r2, 31
LDA r48, 31
ADD r3, r10, r0
SUB r10, r10, r10
LDA r47, 1(r48)
ADD r10, r10, r3
STO r47, 33
LDA r50, 29
ADD r3, r10, r0
SUB r10, r10, r10
LDA r49, 1(r50)
ADD r10, r10, r3
LDA r51, 31
ADD r3, r10, r0
SUB r10, r10, r10
STO r49, 1(r51)
ADD r10, r10, r3
LDA r52, 33
LDA r53, 29
ADD r3, r10, r0
SUB r10, r10, r10
STO r52, 1(r53)
ADD r10, r10, r3
LDC r54, 1
LDA r55, 29
ADD r56, r55, r54
STO r56, 29
JMP 56
LDA r1, 23
SUB r10, r10, r5
JR r1
LDC r57, 0
STO r57, 13
LDC r58, 10
LDA r59, 13
SUB r60, r58, r59
JON r60, 9
IN  r61
LDA r62, 13
STO r61, 1(r62)
LDC r63, 1
LDA r21, 13
ADD r22, r21, r63
STO r22, 13
JMP 113
LDC r12, 1
ADD r12, r12, r10
LDA r3, 0
STO r12, 0
LDA r11, 0
STO r3, 0
LDC r13, 0
LDA r3, 0
STO r13, 0
LDA r12, 0
STO r3, 0
LDC r14, 10
LDA r3, 0
STO r14, 0
LDA r13, 0
STO r3, 0
LDC r1, 143
JMP 49
LDC r23, 0
STO r23, 13
LDC r24, 10
LDA r25, 13
SUB r26, r24, r25
JON r26, 9
LDA r28, 13
LDA r27, 1(r28)
OUT r27
LDC r29, 1
LDA r30, 13
ADD r31, r30, r29
STO r31, 13
JMP 145
HLT
