#include "stdint.h"
#include "stdio.h"
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

//update flags
void update_flags(uint16_t r){
    //updating condition register based on 
    //previously executed instruction.
    if(reg[r]==0){
        reg[R_COND] = FL_ZRO;
    }
    else if(reg[r]>>15){
        //left most bit is set if it is one
        reg[R_COND] = FL_NEG;
    }else{
        reg[R_COND] = FL_POS;
    }
}

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
            {   
                //destination register (DR)
                uint16_t r0 = (instr>>9)&(0x7);
                //first operand (SR1)
                uint16_t r1 = (instr>>6)&(0x7);
                
                //check for register mode of immediate mode
                uint16_t imm_flag = (instr>>5)&(0x1);
                if(imm_flag){
                    uint16_t imm5 = sign_extend((instr>>5)&(0x1F),5);
                    reg[r0] = reg[r1]+imm5;
                }else{
                    uint16_t r2 = instr & 0x7;
                    reg[r0] = reg[r1]+reg[r2];
                }
                update_flags(r0);
            }
            break;
        case OP_AND:
            //bitwise and
            {
                uint16_t r0 = (instr>>9) & (0x7);
                uint16_t r1 = (instr>>6) & (0x7);
                uint16_t imm_flag = (instr>>5) & (0x1);
                if(imm_flag){
                    uint16_t imm5 = sign_extend(instr & 0x1F,5);
                    reg[r0] = reg[r1] & imm5;
                }else{
                    uint16_t r2 = instr & 0x7;
                    reg[r0] = reg[r1] & reg[r2];
                }
                update_flags(r0);
            }
            break;
        case OP_NOT:
            //bitwise not
            {
                uint16_t r0 = (instr >> 9) & (0x7);
                uint16_t r1 = (instr >> 6) & (0x7);
                reg[r0] = ~reg[r1];
                update_flags(r0);
            }
            break;
        case OP_BR:
            //branch
            {
                uint16_t pc_offset = sign_extend(instr & 0x1FF,9);
                uint16_t cond_flag = (instr>>9) & (0x7);
                if(cond_flag & reg[R_COND]){
                    reg[R_PC] += pc_offset;
                }
            }
            break;
        case OP_JMP:
            //jump
            //program jumps to the specified memory address
            {
                uint16_t r1 = (instr >> 6) & 0x7;
                reg[R_PC] = reg[r1];
            }
            break;
        case OP_JSR:
            //jump register
            uint16_t long_flag = (instr>>11) & 1;
            reg[R_R7] = reg[R_PC];
            if(long_flag){
                uint16_t long_pc_offset = sign_extend(instr & 0x7FF,11);
                reg[R_PC] += long_pc_offset;
                //jsr
            }else{
                uint16_t r1 = (instr >>6) & 0x7;
                reg[R_PC] = reg[r1];
                //jsrr
            }
            break;
        case OP_LD:
            //load
            {
                uint16_t r0 = (instr >> 9) & (0x7);
                uint16_t pc_offset = sign_extend(instr & 0x1FF,9);
                reg[r0] = mem_read(reg[R_PC]+pc_offset);
                update_flags(r0);
            }
            break;
        case OP_LDI:
            {
                //destination register
                uint16_t r0 = (instr>>9)&(0x7);
                //program couunter offset
                uint16_t pc_offset = sign_extend(instr & (0x1FF),9);

                reg[r0] = mem_read(meme_read(reg[R_PC]+pc_offset));
                update_flags(r0);
            }
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
            {   
                //unused
                abort();
            }
            break;  
        case OP_RTI:
            {
                //unused
                abort();
            }
            break;
        default:
            //bad opcode
            abort();
            break;
        }
    }
}