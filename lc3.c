#include "stdint.h"
//maximum memory in LC3 is 65536
#define MEMORY_MAX (1<<16)
uint16_t memory[MEMORY_MAX];


//Registers
//here enum variables have global scope
enum{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,//program counter (stores address of next instruction to be executed)
    R_COND,
    R_COUNT
};
uint16_t reg[R_COUNT];

//R_COND register stores information about last executed condition

//Opcodes
//operation codes are the atomic tasks cpu can perform 
//here opcodes are left most 4bits in 16bit instruction
enum{
    OP_BR = 0, //branch
    OP_ADD, //add
    OP_LD, //load
    OP_ST, //store
    OP_JSR, //jump register
    OP_AND, //bitwise and
    OP_LDR, //load register
    OP_STR, //store register
    OP_RTI, //unused
    OP_NOT, // bitwise not
    OP_LDI, // load indirect
    OP_STI, // store indirect 
    OP_JMP, // jump
    OP_RES, // reserved
    OP_LEA, // load effective address
    OP_TRAP, // executive trap
};

//condition flags
enum{
    FL_POS = 1<<0,
    FL_ZRO = 1<<1,
    FL_NEG = 1<<2,
};

//sign extend
//convert lower bit number to a 16 bit number
uint16_t sign_extend(uint16_t x,int bit_count){
    if((x>>(bit_count-1))&(1)){
        x |= (0xFFFF <<bit_count);
    }
    return x;
}
int main(int argc,char *argv[]){

    //load arguments
    if(argc<2){
        printf("lc3 [image-file1] \n");
        exit(2);
    }
    //j from 1 because ./a.out is also in arguments.
    for(int j=1;j<argc;j++){
        if(!read_image(argv[j])){
            printf("failed to load image %s\n",argv[j]);
            exit(1);
        }
    }

    reg[R_COND] = FL_ZRO;

    //default address
    enum{ PC_START = 0x3000};
    reg[R_PC] = PC_START;

    int running = 1;
    while(running){
        uint16_t instr = mem_read(reg[R_PC]++);

        //opcodes are stored in first 4 bits other operands are stored in the next
        uint16_t op = instr>>12;
        switch (op)
        {
        case OP_ADD:
            //add
            //there are two types of add instructions.
            //immediate mode and register mode
            break;
        case OP_AND:
            //bitwise and
            break;
        case OP_NOT:
            //bitwise not
            break;
        case OP_BR:
            //branch
            break;
        case OP_JMP:
            //jump
            break;
        case OP_JSR:
            //jump register
            break;
        case OP_LD:
            //load
            break;
        case OP_LDI:
            break;
        case OP_LDR:
            break;
        case OP_LEA:
            break;
        case OP_ST:
            break;
        case OP_STI:
            break;
        case OP_STR:
            break;
        case OP_TRAP:
            break;
        case OP_RES:
        case OP_RTI:
        default:
            //bad opcode
            break;
        }
    }
}