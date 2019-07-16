
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef enum {
    HLT,
    IN,
    OUT,
    ADD,
    SUB,
    MUL,
    DIV,
    LDA,    // Load address
    LDC,    // Load constant
    STO,     // Store
    JON,    // Jump On Negative (or zero)
    JNZ,    // Jump on Not Zero
    JMP,    // Unconditional jump
    JR
} OpCode;

static char* OpStr[] =
{ "HLT", "IN ", "OUT", "ADD", "SUB", "MUL", "DIV",
  "LDA", "LDC", "STO", "JON", "JNZ", "JMP", "JR" };

#define IADDR_SIZE  1024 /* increase for large programs */
#define DADDR_SIZE  1024 /* increase for large programs */
#define NO_REGS 64

typedef struct {
    OpCode op;
    int i1;
    int i2;
    int i3;
} Instruction;

#endif
