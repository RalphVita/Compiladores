#include <stdlib.h>
#include <stdio.h>
#include "code.h"
#include "instruction.h"
#include "../../hash/tables.h"
#include "../../entity/funcao.h"
#include "../../entity/variavel.h"

extern SymTable *lt;
extern SymTable* variaveis;
extern SymTable* funcoes;

Instruction code[IADDR_SIZE];

int func_row[100];
int func_size_memory[100];
int func_atual;
int index_main;

void init_func_row(){
    for (size_t i = 0; i < 100; i++)
    {
        func_size_memory[i] = 0;
        func_row[i] = 0;
    }
    
}



int instruction_count;

#define new_op() \
    instruction_count++

void emit(OpCode op, int i1, int i2, int i3) {
    code[instruction_count].op = op;
    code[instruction_count].i1 = i1;
    code[instruction_count].i2 = i2;
    code[instruction_count].i3 = i3;
    new_op();
}

#define emit0(op) \
    emit(op, 0, 0, 0)

#define emit1(op, i1) \
    emit(op, i1, 0, 0)

#define emit2(op, i1, i2) \
    emit(op, i1, i2, 0)

#define emit3(op, i1, i2, i3) \
    emit(op, i1, i2, i3)

void backpatch(int ic, OpCode op, int i1, int i2) {
    code[ic].op = op;
    code[ic].i1 = i1;
    code[ic].i2 = i2;
}

void print_instruction(int i) {
    OpCode op = code[i].op;
    int i1 = code[i].i1;
    int i2 = code[i].i2;
    int i3 = code[i].i3;

    // Print OpCode
    printf(OpStr[op]);

    // Zero args instructions
    if (op == HLT) {
        printf("\n");
        return;
    }

    // One arg instructions
    if (op == IN || op == OUT) {
        printf(" r%d\n", i1);
        return;
    }
    if (op == JMP) {
        printf(" %d\n", i1);
        return;
    }

    if (op == JR) {
        printf(" r%d\n", i1);
        return;
    }

    if (op == PUSHM || op == POPM) {
        printf(" %d\n", i1);
        return;
    }

    // Three args instructions with registers
    if (op == ADD || op == SUB || op == MUL || op == DIV) {
        printf(" r%d, r%d, r%d\n", i1, i2, i3);
        return;
    }

    if((op == STO || op == LDA) && i3 > 0){
        printf(" r%d, %d(r%d)\n", i1, i2, i3);
        return;
    }

    // Else: Two args instructions
    printf(" r%d, %d\n", i1, i2);
}

void print_code() {
    for (int i = 0; i < instruction_count; i++) {
        print_instruction(i);
    }
}

int temp_count;

#define new_temp() \
    temp_count = temp_count == 63 ? 21 : temp_count + 1;

//R0 = 0
//R1 = Salva donde veio, para retornar
//R2 = Armazena o valor de return
//R3 = Usado para auxiliar troca
//R5 = Increment de OffSet
//R10 = OffSet da memória
int r0 = 0, r1=1, r2 = 2, r3 = 3, r4 = 4,r5 = 5, r10 = 10;

//-------------------Memory----------------
typedef struct{
    int offset;
    int end;
}frame;

frame frames[100];

int stack;

void init_men(int size){
    stack = 0;
    frames[stack].offset = 0;
    frames[stack].end =  size + 20;
}

void push_mem(int size){
    stack++;
    frames[stack].offset = frames[stack-1].end + 1;
    frames[stack].end = frames[stack].offset + size + 20;
    
    emit3(ADD,r10,r10,r5);
}

void pop_mem(){
    stack--;
    emit3(SUB,r10,r10,r5);
}

int getOffSet(){
    return frames[stack].offset;
}

//-------------------Memory----------------

int rec_emit_code(AST *ast);

int emit_paran_list(AST* ast);

int emit_stmt_seq(AST *ast) {
    int size = get_child_count(ast);
    for (int i = 0; i < size; i++) {
        rec_emit_code(get_child(ast, i));
    }
    return -1;
}


int emit_func_dec(AST *ast){
    AST *func_header = get_child(ast, 0);
    AST *func_name = get_child(func_header, 0);
    int func_idex = ast_get_data(func_name);
    func_row[func_idex] = instruction_count;

    func_atual = func_idex;

    
    //Main não salva donde veio pois vem do além
    if(index_main != func_atual){
        push_mem(func_size_memory[func_idex]);
    //Salva donde veio r1
    emit2(STO, r1, getOffSet());//func_size_memory[func_idex]);
    }

    //Atribui os argumestos aos parametros
    emit_paran_list(get_child(func_header, 1));

    //DEBUG
    //emit1(OUT,1);

    //Emite a função
    AST *func_body = get_child(ast, 1); 
    rec_emit_code(func_body);

    

    //Main não retorna donde veio senão vira um loop infinito
    if(index_main != func_atual){
    //Retorna donde veio
    emit2(LDA, r1, getOffSet());//func_size_memory[func_idex]);
    pop_mem();
    emit1(JR,r1);
    }
    //else pop_mem();

    
    return -1;

}
int emit_func_call(AST *ast){
    
    int save_temp = temp_count;
    //Argumetos são passsados por r11 até r20
    temp_count = 11;
    int r_offset = 11;
    //Carrega argumentos na pilha
    int aridade = get_child_count(ast);
    if(aridade){
        AST *arg_list = get_child(ast, 0);
        int size = get_child_count(arg_list);
        for (int i = 0; i < size; i++) {
            AST *param = get_child(arg_list, i); 
            int r = rec_emit_code(param);
            emit2(LDA,r3,0);
            emit2(STO,r,0);
            emit2(LDA,i+r_offset,0);
            emit2(STO,r3,0);
        }
    }

    temp_count = save_temp;
    //Salva posição de retorno em r1
    emit2(LDC, r1, instruction_count+2);

    //Go to função
    emit1(JMP,func_row[ast_get_data(ast)]);

    //Return é salvo em r2
    return r2;
}

int emit_paran_list(AST* ast){
    //Argumetos são passsados por r11 até r20
    int r_offset = 11;
    for(int i =get_child_count(ast)-1; i>=0 ;i--){
        AST *param = get_child(ast, i);
        Variavel* v = (Variavel*)get_data(variaveis,ast_get_data(param));
        
        int index = ast_get_data(param);

        index=v->posicao;

        //Passagem por referência. Quando é um vetor
        if(v->tamanho == -1){
           int a = getOffSet() + index + 1;
           emit2(STO, i+r_offset, a);
        }else//Caso seja passagem por cópia
        {
        int a = getOffSet() + index + 1;
        emit2(STO, i+r_offset, a);
        }  
    }       
}


int emit_if(AST *ast) {
    AST *test = get_child(ast, 0);
    int result = rec_emit_code(test);

    int cond_jump_instr = new_op(); // Leave it empty for now.
    if (get_kind(test) == LE_NODE || get_kind(test) == GE_NODE){ new_op();new_op();}
    else if(get_kind(test) == NEQ_NODE)new_op();

    int true_branch_start = instruction_count;
    rec_emit_code(get_child(ast, 1)); // Generate TRUE branch.
    int false_branch_start;
    if (get_child_count(ast) == 3) {
        // We have an else.
        int uncond_jump = new_op(); // Leave it empty for now.
        false_branch_start = instruction_count;
        rec_emit_code(get_child(ast, 2)); // Generate FALSE branch
        backpatch(uncond_jump, JMP, instruction_count, 0);
    } else {
        false_branch_start = instruction_count;
    }

        
    // Backpatch test.
    if (get_kind(test) == EQ_NODE) {
        backpatch(cond_jump_instr, JNZ, result, false_branch_start - true_branch_start + 1);
    }//GT é o inverso de LT 
    else if (get_kind(test) == LT_NODE || get_kind(test) == GT_NODE) {
        backpatch(cond_jump_instr, JON, result, false_branch_start - true_branch_start + 1);
    }//LE e GE usa uma combinação de JNZ seguido de JON e JON 
    else if (get_kind(test) == LE_NODE || get_kind(test) == GE_NODE) {
        backpatch(cond_jump_instr, JNZ, result, 2);
        backpatch(cond_jump_instr+1, JON, result, 2);
        backpatch(cond_jump_instr+2, JON, result, false_branch_start - true_branch_start + 1);
    }//NEQ faz um JNZ depois JON 
    else if(get_kind(test) == NEQ_NODE){
        backpatch(cond_jump_instr, JNZ, result, 2);
        backpatch(cond_jump_instr+1, JON, result, false_branch_start - true_branch_start + 1);
    }
    else {
        fprintf(stderr, "Invalid test kind: %s!\n", kind2str(get_kind(ast)));
        exit(1);
    }
    return -1;
}

int emit_while(AST *ast) {
    int begin_repeat = instruction_count;
    AST *test = get_child(ast, 0);    // Emit code for test.
    int result = rec_emit_code(test);

    int cond_jump_instr = new_op();
    if (get_kind(test) == LE_NODE || get_kind(test) == GE_NODE){ new_op();new_op();}
    else if(get_kind(test) == NEQ_NODE)new_op();
    
    int after_test = instruction_count;

    rec_emit_code(get_child(ast, 1)); // Emit code for body.
    
    int end_while = instruction_count;

    emit1(JMP, begin_repeat);

    if (get_kind(test) == EQ_NODE) {
        emit2(JNZ, result, end_while - after_test +2);
    }//GT é o inverso de LT 
     else if (get_kind(test) == LT_NODE  || get_kind(test) == GT_NODE) {
        backpatch(cond_jump_instr, JON, result, end_while - after_test +2);
    }//LE e GE usa uma combinação de JNZ seguido de JON e JON
    else if (get_kind(test) == LE_NODE || get_kind(test) == GE_NODE) {
        backpatch(cond_jump_instr, JNZ, result, 2);
        backpatch(cond_jump_instr+1, JON, result, 2);
        backpatch(cond_jump_instr+2, JON, result, end_while - after_test +2);
    }//NEQ faz um JNZ depois JON
    else if(get_kind(test) == NEQ_NODE){
        backpatch(cond_jump_instr, JNZ, result, 2);
        backpatch(cond_jump_instr+1, JON, result, end_while - after_test + 2);
    } else {
        fprintf(stderr, "Invalid test kind: %s!\n", kind2str(get_kind(ast)));
        exit(1);
    }
    return -1;
}

int emit_assign(AST *ast) {
    int r = rec_emit_code(get_child(ast, 1));
    AST *child = get_child(ast, 0);
    int a = ast_get_data(child);

    Variavel* v = (Variavel*)get_data(variaveis,a);
    a=v->posicao;

    //Caso seja do tipo: x[i]
    if(get_child_count(child) > 0){
        int rindex = rec_emit_code(get_child(child, 0));
        a+= getOffSet() + 1;

        //Referêcia
        if(v->tamanho == - 1){

            //Zera o offSet de memória para pegar o endereço absoluto
            emit3(ADD,r3,r10,r0);
            emit3(SUB,r10,r10,r10);

            emit3(STO,r,1,rindex);

            //Restaura o OffSet de memória
            emit3(ADD,r10,r10,r3);

        }else
            emit3(STO,r,a,rindex);
    }else{
        a+= getOffSet() + 1;
        emit2(STO, r, a);
    }
    return -1;
}

int emit_input(AST *ast) {
    int r = new_temp();
    emit1(IN, r);
    return r;
}

int emit_output(AST *ast) {
    int r = rec_emit_code(get_child(ast, 0));
    emit1(OUT, r);
    return -1;
}

//Aqui faço uma verificação de quem tem filhos, e executo primeiro
//Pois ao carregar um registrador, e chamar uma função recursivamente
//esse resgistrador pode ser alterado. Então eu carrego registrador depois de fazer a recursão
#define bin_op() \
    int t,s;\
    if(get_child_count(get_child(ast, 0)) > 0) s = rec_emit_code(get_child(ast, 0));\
    else  t = rec_emit_code(get_child(ast, 1));\
    if(get_child_count(get_child(ast, 0)) == 0) s = rec_emit_code(get_child(ast, 0));\
    else  t = rec_emit_code(get_child(ast, 1));\
    int r = new_temp()\

int emit_plus(AST *ast) {
    bin_op();
    emit3(ADD, r, s, t);
    return r;
}

int emit_minus(AST *ast) {
    bin_op();
    emit3(SUB, r, s, t);
    return r;
}

int emit_times(AST *ast) {
    bin_op();
    emit3(MUL, r, s, t);
    return r;
}

int emit_over(AST *ast) {
    bin_op();
    emit3(DIV, r, s, t);
    return r;
}

int emit_lt(AST *ast) {
    bin_op();
    emit3(SUB, r, t, s);
    return r;
}

int emit_le(AST *ast) {
    bin_op();
    emit3(SUB, r, t, s);
    return r;
}

int emit_gt(AST *ast) {
    bin_op();
    emit3(SUB, r, s, t);
    return r;
}

int emit_ge(AST *ast) {
    bin_op();
    emit3(SUB, r, s, t);
    return r;
}

int emit_eq(AST *ast) {
    bin_op();
    emit3(SUB, r, s, t);
    return r;
}

int emit_neq(AST *ast) {
    bin_op();
    emit3(SUB, r, s, t);
    return r;
}

int emit_num(AST *ast) {
    int r = new_temp();
    int c = ast_get_data(ast);
    emit2(LDC, r, c);
    return r;
}

int emit_var_use(AST *ast) {
    int r = new_temp();
    int a = ast_get_data(ast);

    Variavel* v = (Variavel*)get_data(variaveis,a);
    a=v->posicao;

    //x[i]
    if(get_child_count(ast) > 0){
        int rindex = rec_emit_code(get_child(ast,0));
        a+= getOffSet() + 1;

        //Referêcia
        if(v->tamanho == -1){

            //Reseta o OffSet da memória
            emit3(ADD,r3,r10,r0);
            emit3(SUB,r10,r10,r10);
            
            //Pega o endereço absoluto da variavel
            emit3(LDA,r,1,rindex);

            //Restaura o OffSet da memória
            emit3(ADD,r10,r10,r3);
        }
        else
            emit3(LDA,r,a,rindex);
    }else{
        if(v->tamanho == 0){
            a+= getOffSet() + 1;
            emit2(LDA, r, a);
        }
        else
        {
            a+= getOffSet() + 1;
            emit2(LDC, r, a);
            emit3(ADD,r,r,r10);
        }
        
    }
    return r;
}
int emit_return(AST *ast) {
    int r = rec_emit_code(get_child(ast,0));
    //Return é salvo em r2
    emit3(ADD,r2,r,r0);
    return -1;
}


int code_recurso(AST *ast){
    int i = 0;
    int ret = -1;
    for(i =0; i< get_child_count(ast);i++)
                ret = rec_emit_code(get_child(ast,i));
    return ret;
}

int rec_emit_code(AST *ast) {
    switch(get_kind(ast)) {
        case FUNC_DECL_NODE:    return emit_func_dec(ast);
        case FUNC_CALL_NODE:    return emit_func_call(ast);
        case RETURN_NODE:       return emit_return(ast);
        case IF_NODE:           return emit_if(ast);
        case WHILE_NODE:        return emit_while(ast);
        case WRITE_NODE:        return -1;
        case ASSIGN_NODE:       return emit_assign(ast);
        case INPUT_NODE:        return emit_input(ast);
        case OUTPUT_NODE:       return emit_output(ast);
        case PLUS_NODE:     return emit_plus(ast);
        case MINUS_NODE:    return emit_minus(ast);
        case TIMES_NODE:    return emit_times(ast);
        case OVER_NODE:     return emit_over(ast);
        case LT_NODE:       return emit_lt(ast);
        case LE_NODE:       return emit_le(ast);
        case GT_NODE:       return emit_gt(ast);
        case GE_NODE:       return emit_ge(ast);
        case EQ_NODE:       return emit_eq(ast);
        case NEQ_NODE:      return emit_neq(ast);
        case NUM_NODE:      return emit_num(ast);
        case VAR_USE_NODE:  return emit_var_use(ast);
        default:
            return code_recurso(ast);
    }
}

//Aloca espaço na memória pra cada variavel por escopo
Variavel *anterior;
void get_memory_size_funcao(Variavel *v,int index){
    func_size_memory[v->escopo]++;
    if(index > 0 && v->escopo == anterior->escopo)
        v->posicao = anterior->posicao + 2 + anterior->tamanho;
    else
        v->posicao = 0;
    
    anterior = v;
}

void Init(){
    init_func_row();
    func_atual=0;

    //Varre variaveis para inicializar as posições na memória
    each_sym_table(variaveis,get_memory_size_funcao);

    init_men(func_size_memory[index_main]);
}

void emit_code(AST *ast) {
    index_main = lookup_var(funcoes,"main");
    Init();
    
    

    instruction_count = 2;
    temp_count = 20; // Usable registers start at 20
    rec_emit_code(ast);
    emit0(HLT);
    
    int aux = instruction_count;

    //Set o increment de memória a cada call
    instruction_count = 0;
    emit2(LDC,r5,50);

    //Inicializa indo para a Mains
    instruction_count = 1;
    emit1(JMP,func_row[index_main]);

    instruction_count = aux;

    print_code();
}
