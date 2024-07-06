#include <defs.h>


typedef enum
{
    IN_NONE,
    IN_NOP,
    IN_LD,
    IN_INC,
    IN_DEC,
    IN_JP,
    IN_RRCA,
    IN_RLCA,
    IN_RLA,
    IN_RRA,
    IN_STOP,
    IN_JR,
    IN_DAA,
    IN_SCF,
    IN_HALT,
    IN_CPL,
    IN_CCF,
    IN_ADD,
    IN_ADC,
    IN_SUB,
    IN_SBC,
    IN_AND,
    IN_XOR,
    IN_OR,
    IN_CP,
    IN_RET,
    IN_RETI,
    IN_RST,
    IN_CALL,
    IN_POP,
    IN_PUSH,
    IN_DI,
    IN_EI,
    IN_CB,
    //CB INSTRUCTION..
    IN_RLC,
    IN_RRC,
    IN_RL,
    IN_RR,
    IN_SLA,
    IN_SRA,
    IN_SWAP,
    IN_SRL,
    IN_BIT,
    IN_RES,
    IN_SET
} in_type;

typedef enum
{
    AM_IMP,
    AM_R,
    AM_R_R,
    AM_R_D8,
    AM_R_D16,
    AM_MR,
    AM_MR_R,
    AM_R_MR,
    AM_A16_R,
    AM_R_A16,
    AM_HLI_R, // increment HL
    AM_HLD_R, // decrement HL
    AM_R_HLI,
    AM_R_HLD,
    AM_A8_R,
    AM_R_A8,
    AM_D8,
    AM_A16,
    AM_MR_D8,
    AM_HL_SPR, //0xF8
    AM_S8, // signed 8bit data for relative jumps
    AM_R_S8
} addr_mode;

typedef enum
{
    RT_NONE,
    RT_A,
    RT_F,
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_AF,
    RT_BC,
    RT_DE,
    RT_HL,
    RT_SP,
    RT_PC
} reg_type;

typedef enum 
{ 
    CT_NONE, CT_NZ, CT_Z, CT_C, CT_NC
} cond_type;


typedef struct 
{
    in_type type;
    addr_mode mode;
    reg_type reg_1;
    reg_type reg_2;
    cond_type cond;
    u8 param;

} instruction;
