
// The Tiny Machine (TM) Simulator

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./porting/compiler/instruction.h"

// ----------------------------------------------------------------------------
// Macros ---------------------------------------------------------------------

#define LINE_SIZE 80

// ----------------------------------------------------------------------------
// Structures -----------------------------------------------------------------

typedef enum {
    OKAY,
    HLTED,
    IMEM_ERR,
    DMEM_ERR,
    DIV_ZERO
} StepResult;

// ----------------------------------------------------------------------------
// Memory ---------------------------------------------------------------------

Instruction iMem[IADDR_SIZE];
int dMem[DADDR_SIZE];
int reg[NO_REGS];
int pc;
int ofset;
int get_addrs(int addr){
    return ofset + addr + reg[10];
}

// ----------------------------------------------------------------------------
// Init -----------------------------------------------------------------------

void init() {
    ofset = 0;
    // Clear Instruction Memory
    for (int iAddr = 0; iAddr < IADDR_SIZE; iAddr++) {
        iMem[iAddr].op = HLT;
        iMem[iAddr].i1 = 0;
        iMem[iAddr].i2 = 0;
        iMem[iAddr].i3 = 0;
    }
    // Clear Data Memory
    for (int dAddr = 0; dAddr < DADDR_SIZE; dAddr++) {
        dMem[dAddr] = 0;
    }
    // Clear Registers
    for (int r = 0; r < NO_REGS; r++) {
        reg[r] = 0;
    }
    pc = 0;
}

// ----------------------------------------------------------------------------
// Read -----------------------------------------------------------------------

static int OpCount[] =
{ 0, // HLT
  1, // IN
  1, // OUT
  3, // ADD
  3, // SUB
  3, // MUL
  3, // DIV
  2, // LDA
  2, // LDC
  2, // STO
  2, // JON
  2, // JNZ
  1, // JMP
  1, // JR
  1, // PUSHM
  1  // POPM
};

bool read_line(char *line) {
    return !fgets(line, LINE_SIZE, stdin);
}

bool is_empty(char *line) {
    return line[0] == ';' || line[0] == ' ' || line[0] == '\n';
}

#define OP_ERR "Undefined operation in ASM line above!"
#define ARG_ERR "Wrong number of operands or invalid syntax!"

void parse_error(char *line, char *msg) {
    fprintf(stderr, "ERROR in line: %s\n TM says: %s\n", line, msg);
    exit(1);
}

OpCode get_opcode(char *line) {
    if (line[0] == 'A') {
        if (line[1] == 'D' && line[2] == 'D') return ADD;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'D') {
        if (line[1] == 'I' && line[2] == 'V') return DIV;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'H') {
        if (line[1] == 'L' && line[2] == 'T') return HLT;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'I') {
        if (line[1] == 'N') return IN;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'J') {
        if (line[1] == 'M' && line[2] == 'P') return JMP;
        else if (line[1] == 'N' && line[2] == 'Z') return JNZ;
        else if (line[1] == 'O' && line[2] == 'N') return JON;
        else if(line[1] == 'R') return JR;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'L') {
        if (line[1] == 'D' && line[2] == 'A') return LDA;
        else if (line[1] == 'D' && line[2] == 'C') return LDC;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'M') {
        if (line[1] == 'U' && line[2] == 'L') return MUL;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'O') {
        if (line[1] == 'U' && line[2] == 'T') return OUT;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'S') {
        if (line[1] == 'U' && line[2] == 'B') return SUB;
        else if (line[1] == 'T' && line[2] == 'O') return STO;
        else parse_error(line, OP_ERR);
    } else if (line[0] == 'P'){
        if(line[1] == 'U' && line[2] == 'S' && line[3] == 'H' && line[4] == 'M') return PUSHM;
        else if(line[1] == 'O' && line[2] == 'P' && line[3] == 'M') return POPM;
        else parse_error(line, OP_ERR);
    } else parse_error(line, OP_ERR);
    return HLT; // Dummy return, should never be reached.
}

#define eat_space() \
    while (*line == ' ' || *line == ',') line++

void read_params(char *line, OpCode op, int *i1, int *i2, int *i3) {
    *i1 = 0;
    *i2 = 0;
    *i3 = 0;
    int consumed;
    int n = OpCount[op];
    

    if (n == 0) return;

    if (op == IN || op == JR) {
        line += 2;
    }else if(op == PUSHM) {
        line += 5;
    }
    else if(op == POPM) {
        line += 4;
    }
    else {
        line += 3;
    }

    eat_space();
    if (op == JMP || op == PUSHM || op == POPM) {
        sscanf(line, "%d%n", i1, &consumed);
    } else {
        sscanf(line, "r%d%n", i1, &consumed);
    }
    line += consumed;

    if (n == 1) return;

    eat_space();
    if (op == ADD || op == SUB || op == MUL || op == DIV) {
        sscanf(line, "r%d%n", i2, &consumed);
        line += consumed;
    } else {
        *i3 = -1;
        sscanf(line, "%d%n", i2, &consumed);
        line += consumed;
        
        if(*(line) == 40){
            sscanf(line, "(r%d%n)", i3, &consumed);
            line += consumed;
        }
    }

    if (n == 2) return;

    eat_space();
    sscanf(line, "r%d", i3);
}

int ia = 0; // For program loading.

void parse_line(char *line) {
    OpCode op;
    int i1;
    int i2;
    int i3;

    if (is_empty(line)) return;

    op = get_opcode(line);
    read_params(line, op, &i1, &i2, &i3);

    // Store instruction
    iMem[ia].op = op;
    iMem[ia].i1 = i1;
    iMem[ia].i2 = i2;
    iMem[ia].i3 = i3;
    ia++;
}

 void read_program() {
    bool done;
    char line[LINE_SIZE];

    done = false;
    ia = 0;
    done = read_line(line);
    while (!done) {
        parse_line(line);
        done = read_line(line);
    }
}

void get_instruction_string(Instruction instr, char *s) {
    OpCode op = instr.op;
    s += sprintf(s, "%s", OpStr[op]);

    if (op == HLT) return;

    if (op == IN || op == OUT || op == JR) {
        sprintf(s, " r%d", instr.i1);
    } else if (op == JMP || op == PUSHM || op == POPM) {
        sprintf(s, " %d", instr.i1);
    } else if (op == ADD || op == SUB || op == MUL || op == DIV) {
        sprintf(s, " r%d, r%d, r%d", instr.i1, instr.i2, instr.i3);
    } else if(op == STO || op == LDA){
        if(instr.i3 < 0){
            sprintf(s, " r%d, %d", instr.i1, instr.i2);
            
        }else
            sprintf(s, " r%d, %d(r%d)", instr.i1, instr.i2,instr.i3);
        
    }else {
        sprintf(s, " r%d, %d", instr.i1, instr.i2);
    }
}

void write_instruction(int iAddr) {
    Instruction instr = iMem[iAddr];
    char instr_str[LINE_SIZE];
    get_instruction_string(instr, instr_str);
    printf("%5d: %s\n", iAddr, instr_str);
}

void dump_program() {
    printf("*** Instruction Memory:\n\n");
    for (int iAddr = 0; iAddr < ia; iAddr++) {
        write_instruction(iAddr);
    }
    printf("\n");
}

// ----------------------------------------------------------------------------
// Run ------------------------------------------------------------------------

#define inc_PC() pc++

StepResult run_hlt(Instruction instr) {
    return HLTED;
}

StepResult run_in(Instruction instr) {
    int x;
    printf("IN: ");
    scanf("%d", &x);
    reg[instr.i1] = x;
    inc_PC();
    return OKAY;
}

StepResult run_out(Instruction instr) {
    printf("OUT: %d\n", reg[instr.i1]);
    inc_PC();
    return OKAY;
}

#define bin_op() \
    int l = reg[instr.i2];\
    int r = reg[instr.i3]

StepResult run_add(Instruction instr) {
    bin_op();
    reg[instr.i1] = l + r;
    inc_PC();
    return OKAY;
}

StepResult run_sub(Instruction instr) {
    bin_op();
    reg[instr.i1] = l - r;
    inc_PC();
    return OKAY;
}

StepResult run_mul(Instruction instr) {
    bin_op();
    reg[instr.i1] = l * r;
    inc_PC();
    return OKAY;
}

StepResult run_div(Instruction instr) {
    bin_op();
    if (r == 0) return DIV_ZERO;
    reg[instr.i1] = l / r;
    inc_PC();
    return OKAY;
}

StepResult run_lda(Instruction instr) {
    int addr = instr.i2;
    addr = instr.i3 >= 0 ? addr+reg[instr.i3] : addr;
    if (addr < 0 || addr >= DADDR_SIZE) return DMEM_ERR;
    reg[instr.i1] = dMem[get_addrs(addr)];
    inc_PC();
    return OKAY;
}

StepResult run_ldc(Instruction instr) {
    reg[instr.i1] = instr.i2;
    inc_PC();
    return OKAY;
}

StepResult run_sto(Instruction instr) {
    int addr = instr.i2;
    addr = instr.i3 >= 0 ? addr+reg[instr.i3] : addr;
    if (addr < 0 || addr >= DADDR_SIZE) return DMEM_ERR;
    dMem[get_addrs(addr)] = reg[instr.i1];
    inc_PC();
    return OKAY;
}

StepResult run_jon(Instruction instr) {
    if (reg[instr.i1] <= 0) {
        pc += instr.i2;
    } else {
        inc_PC();
    }
    return OKAY;
}

StepResult run_jnz(Instruction instr) {
    if (reg[instr.i1] != 0) {
        pc += instr.i2;
    } else {
        inc_PC();
    }
    return OKAY;
}

StepResult run_jmp(Instruction instr) {
    pc = instr.i1;
    return OKAY;
}

StepResult run_jr(Instruction instr) {
    pc = reg[instr.i1];
    return OKAY;
}

StepResult run_pushm(Instruction instr) {
    ofset+= instr.i1;
    inc_PC();
    return OKAY;
}

StepResult run_popm(Instruction instr) {
    ofset-= instr.i1;
    inc_PC();
    return OKAY;
}

#define TRACE

StepResult step() {
    if (pc < 0 || pc >= IADDR_SIZE) return IMEM_ERR;

    #ifdef TRACE
        printf("TRACE: Next instruction is - ");
        write_instruction(pc);
    #endif

    Instruction instr = iMem[pc];

    switch(instr.op) {
        case HLT: return run_hlt(instr);
        case IN:  return run_in(instr);
        case OUT: return run_out(instr);
        case ADD: return run_add(instr);
        case SUB: return run_sub(instr);
        case MUL: return run_mul(instr);
        case DIV: return run_div(instr);
        case LDA: return run_lda(instr);
        case LDC: return run_ldc(instr);
        case STO: return run_sto(instr);
        case JON: return run_jon(instr);
        case JNZ: return run_jnz(instr);
        case JMP: return run_jmp(instr);
        case JR: return run_jr(instr);
        case PUSHM: return run_pushm(instr);
        case POPM: return run_popm(instr);
        default:  return OKAY; // Dummy return, should not be reached.
    }
}

void run() {
    StepResult result;
    do {
        result = step();
    } while (result == OKAY);

    switch(result) {
        case HLTED:
            printf("SUCCESS: Execution finished!\n");
            break;
        case IMEM_ERR:
            printf("ERROR: Invalid access to Instruction Memory!\n");
            break;
        case DMEM_ERR:
            printf("ERROR: Invalid access to Data Memory!\n");
            break;
        case DIV_ZERO:
            printf("ERROR: Division by zero!\n");
            break;
        default:
            printf("ERROR: Fall-through in run..?\n");
    }
}

// ----------------------------------------------------------------------------
// Main -----------------------------------------------------------------------

#define DEBUG

int main(void) {
    init();
    read_program(); // From STDIN
    #ifdef DEBUG
        dump_program();
    #endif
    stdin = fopen(ctermid(NULL), "r"); // Reset STDIN so IN commands will work.
    run();
    return 0;
}
