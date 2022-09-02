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