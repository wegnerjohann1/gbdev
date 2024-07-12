#include <cpu.h>

extern cpu_context ctx;

u16 reverse(u16 n)
{
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);  
}


u16 cpu_read_reg(reg_type rt)
{
    switch (rt)
    {
    case RT_A: return ctx.regs.a;
    case RT_F: return ctx.regs.f;
    case RT_B: return ctx.regs.b;
    case RT_C: return ctx.regs.c;
    case RT_D: return ctx.regs.d;
    case RT_E: return ctx.regs.e;
    case RT_H: return ctx.regs.h;
    case RT_L: return ctx.regs.l;
    
    case RT_AF: return reverse(*((u16 *)&ctx.regs.a));
    case RT_BC: return reverse(*((u16 *)&ctx.regs.b));
    case RT_DE: return reverse(*((u16 *)&ctx.regs.d));
    case RT_HL: return reverse(*((u16 *)&ctx.regs.h));

    case RT_PC: return ctx.regs.PC;
    case RT_SP: return ctx.regs.SP;

    default: return 0;
    }
}

void cpu_set_reg(reg_type rt, u16 value)
{
        printf("%04X\n", value);
    switch (rt)
    {
    case RT_A: ctx.regs.a = value & 0x00FF; break;
    case RT_F: ctx.regs.f = value & 0x00FF; break;
    case RT_B: ctx.regs.b = value & 0x00FF; break;
    case RT_C: ctx.regs.c = value & 0x00FF; break;
    case RT_D: ctx.regs.d = value & 0x00FF; break;
    case RT_E: ctx.regs.e = value & 0x00FF; break;
    case RT_H: ctx.regs.h = value & 0x00FF; break;
    case RT_L: ctx.regs.l = value & 0x00FF; break;
    
    case RT_AF: *((u16 *)&ctx.regs.a) = reverse(value); break;
    case RT_BC: *((u16 *)&ctx.regs.b) = reverse(value); break;
    case RT_DE: *((u16 *)&ctx.regs.d) = reverse(value); break;
    case RT_HL: *((u16 *)&ctx.regs.h) = reverse(value); break;

    case RT_PC: ctx.regs.PC = value;
    case RT_SP: ctx.regs.SP = value;

    case RT_NONE: break;
    }
}