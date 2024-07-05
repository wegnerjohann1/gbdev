#include <instructions.h>

instruction instructions[0x100] =
{
    //0x0.
    [0x00] = {IN_NOP, AM_IMP},
    [0x01] = {IN_LD, AM_R_D16, RT_BC},
    [0x02] = {IN_LD, AM_MR_R, RT_BC, RT_A},
    [0x03] = {IN_INC, AM_R, RT_BC},
    [0x04] = {IN_INC, AM_R, RT_B},
    

    //0x1.

    //0x2.
    [0x21] = {IN_LD, AM_R_D16, RT_HL},
    
    //0x3.
    [0x31] = {IN_LD, AM_R_D16, RT_SP},
    
    //0x4.
    //0x5.
    //0x6.
    //0x7.
    //0x8.
    //0x9.
    //0xA.
    [0xAF] = {IN_XOR, AM_R, RT_A},

    //0xB.
    //0xC.
    [0xC3] = {IN_JP, AM_A16},

    //0xD.
    //0xE.
    //0xF.


};