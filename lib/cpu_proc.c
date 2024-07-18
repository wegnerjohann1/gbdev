#include <cpu.h>
#include <emu.h>
#include <bus.h>
#include <stack.h>

// proccess cpu instruction

static bool check_cond(cpu_context *ctx)
{
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch (ctx->cur_inst->cond)
    {
        case CT_NONE: return true;
        case CT_C: return c;
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;
    }

    return false;
}

static void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c)
{
    if (z != -1)
    {
        BIT_SET(ctx->regs.f, 7, z)
    }
    if (n != -1)
    {
        BIT_SET(ctx->regs.f, 6, n)
    }
    if (h != -1)
    {
        BIT_SET(ctx->regs.f, 5, h)
    }
    if (c != -1)
    {
        BIT_SET(ctx->regs.f, 4, c)
    }
}

static bool is_16_bit(reg_type rt)
{
    return rt >= RT_AF;
}

static void proc_none(cpu_context *ctx)
{
    printf("INVALID INSTRUCTION!\n");
    exit(UNKNOWN_INSTRUCTION);
}

static void proc_nop(cpu_context *ctx)
{
    //do nothing
}

static void proc_ld(cpu_context *ctx)
{
    if (ctx->dest_is_mem)
    {
        if (is_16_bit(ctx->cur_inst->reg_2))
        {
            emu_cycles(1);
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }
        else
            bus_write(ctx->mem_dest, ctx->fetched_data);
        
        emu_cycles(1);

        return;
    }

    if (ctx->cur_inst->mode == AM_HL_SPR)
    {
        // get first 4 bits of both summands and check if any bit after 4th is 1 ie >= 0x10;
        bool hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0x000F) + (cpu_read_reg(ctx->fetched_data) & 0x000F) >= 0x10;
        // get first 8 bits of both summands and check if any bit after 8th is 1 ie >= 0x100;
        bool cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0x00FF) + (cpu_read_reg(ctx->fetched_data) & 0x00FF) >= 0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->cur_inst->reg_1, ctx->cur_inst->reg_2 + (s8)ctx->fetched_data);
        emu_cycles(1);
        return;
    }

    if (is_16_bit(ctx->cur_inst->reg_2))
    {
        //probaly only LD SP, HL : 0xF9
        cpu_set_reg(ctx->cur_inst->reg_1, ctx->cur_inst->reg_2);
        emu_cycles(1);
        return;
    }
    
    cpu_set_reg(ctx->cur_inst->reg_1, ctx->fetched_data);
    
}

static void proc_di(cpu_context *ctx)
{
    ctx->int_master_enabled = false;
}

static void proc_ei(cpu_context *ctx)
{
    ctx->enabling_ime = true;
}

static void proc_jp(cpu_context *ctx)
{
    if (check_cond(ctx))
    {
        ctx->regs.PC = ctx->fetched_data;
        emu_cycles(1);
    }
}

static void proc_jr(cpu_context *ctx)
{
    if (check_cond(ctx))
    {   
        ctx->regs.PC += (s8)((ctx->fetched_data) & 0x00FF);
        emu_cycles(1);
    }
}

static void proc_call(cpu_context *ctx)
{   
    if (check_cond(ctx))
    {
        emu_cycles(2);
        stack_push16(ctx->regs.PC);
        
        ctx->regs.PC = ctx->fetched_data;
        emu_cycles(1);
    }
}

static void proc_ret(cpu_context *ctx)
{
    if (ctx->cur_inst->cond != CT_NONE)
    {
        emu_cycles(1);
    }

    if (check_cond(ctx))
    {
        u16 lo = stack_pop();
        emu_cycles(1);

        u16 hi = stack_pop();
        emu_cycles(1);

        ctx->regs.PC = (hi << 8) | lo;
        emu_cycles(1);
    }
}

static void proc_reti(cpu_context *ctx)
{
    ctx->int_master_enabled = true;
    proc_ret(ctx);
}

static void proc_rst(cpu_context *ctx)
{
    emu_cycles(2);
    stack_push16(ctx->regs.PC);

    ctx->regs.PC = ctx->cur_inst->param;
    emu_cycles(1);
}

static void proc_pop(cpu_context *ctx)
{
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);
    u16 n = (hi << 8) | lo;
    if (ctx->cur_inst->reg_1 == RT_AF)
        cpu_set_reg(RT_AF, n & 0xFFF0);
    else
        cpu_set_reg(ctx->cur_inst->reg_1, n);
}

static void proc_push(cpu_context *ctx)
{
    u16 hi = (cpu_read_reg(ctx->cur_inst->reg_1) >> 8) & 0x00FF;
    emu_cycles(1);
    stack_push(hi);

    u16 lo = cpu_read_reg(ctx->cur_inst->reg_1) & 0x00FF;
    emu_cycles(1);
    stack_push(lo);
    
    emu_cycles(1);
}

static void proc_inc(cpu_context *ctx)
{
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) + 1;

    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR)
    {
        val = bus_read(cpu_read_reg(RT_HL)) + 1;
        emu_cycles(1);
        val &= 0x00FF;
        bus_write(cpu_read_reg(RT_HL), val);
        emu_cycles(1);
    }
    else
    {
        cpu_set_reg(ctx->cur_inst->reg_1, val);
    }

    if ((ctx->cur_opcode & 0x0F) == 0x03)
    {
        emu_cycles(1);
        return;
    }

    cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}


static void proc_dec(cpu_context *ctx)
{
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) - 1;

    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR)
    {
        val = bus_read(cpu_read_reg(RT_HL)) - 1;
        emu_cycles(1);
        val &= 0x00FF;
        bus_write(cpu_read_reg(RT_HL), val);
        emu_cycles(1);
    }
    else
    {
        cpu_set_reg(ctx->cur_inst->reg_1, val);
    }

    if ((ctx->cur_opcode & 0x0F) == 0x0B)
    {
        emu_cycles(1);
        return;
    }

    cpu_set_flags(ctx, val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

static void proc_add(cpu_context *ctx)
{
    u32 val = cpu_read_reg(ctx->cur_inst->reg_1) + ctx->fetched_data;

    bool is_16bit = is_16_bit(ctx->cur_inst->reg_1);

    if (is_16bit)
        emu_cycles(1);
    
    if (ctx->cur_inst->reg_1 == RT_SP)
    {
        val = cpu_read_reg(ctx->cur_inst->reg_1) + (s8)ctx->fetched_data;
        emu_cycles(1);
    }

    int z = (val & 0xFF) == 0;
    int h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
    int c = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF) + (ctx->fetched_data & 0xFF) >= 0x100;
    
    if (ctx->cur_inst->reg_1 == RT_SP)
    {
        z = 0;
    }
    else if (is_16bit)
    {
        z = -1;
        h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xFFF) + (ctx->fetched_data & 0xFFF) >= 0x1000;
        u32 n = (u32)cpu_read_reg(ctx->cur_inst->reg_1) + (u32)ctx->fetched_data;
        c = n >= 0x10000;
    }

    cpu_set_reg(ctx->cur_inst->reg_1, val);
    cpu_set_flags(ctx, z, 0, h, c);
}

static void proc_adc(cpu_context *ctx)
{
    u16 v = ctx->fetched_data;
    u16 a = ctx->regs.a;
    u16 c = CPU_FLAG_C;

    ctx->regs.a = ((a + v + c) & 0xFF);

    cpu_set_flags(ctx, ctx->regs.a == 0, 0, (a & 0xF) + (v & 0xF) + c >= 0x10, a + v + c >= 0x100);

}

static void proc_sub(cpu_context *ctx)
{
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) - ctx->fetched_data;

    int z = (val & 0xFF) == 0;
    int h = ((int)cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) - ((int)ctx->fetched_data & 0xF) < 0;
    int c = (int)cpu_read_reg(ctx->cur_inst->reg_1) - (int)ctx->fetched_data < 0;
    

    cpu_set_reg(ctx->cur_inst->reg_1, val);
    cpu_set_flags(ctx, z, 1, h, c);
}

static void proc_sbc(cpu_context *ctx)
{
    u16 v = ctx->fetched_data;
    u16 a = ctx->regs.a;

    ctx->regs.a = ((a - v - CPU_FLAG_C) & 0xFF);

    int z = ctx->regs.a == 0;
    int h = ((int)a & 0xF) - ((int)v & 0xF) - CPU_FLAG_C < 0;
    int c = (int)a - (int)v - CPU_FLAG_C < 0;

    cpu_set_flags(ctx, z, 1, h, c);

}

static void proc_and(cpu_context *ctx)
{
    ctx->regs.a &= ctx->fetched_data;
    cpu_set_flags(ctx, ctx->cur_inst->reg_1==0, 0,1,0);
}

static void proc_xor(cpu_context *ctx)
{
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->cur_inst->reg_1==0, 0,1,0);
}

static void proc_or(cpu_context *ctx)
{
    ctx->regs.a |= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->cur_inst->reg_1==0, 0,1,0);
}

static void proc_cp(cpu_context *ctx)
{
    int n = (int)ctx->regs.a - (int)ctx->fetched_data;
    cpu_set_flags(ctx, n==0, 1, ((int)ctx->regs.a & 0xF) - ((int)ctx->fetched_data & 0xF) < 0, n < 0);
}

static void proc_rrca(cpu_context *ctx)
{
    bool b = BIT(ctx->regs.a, 0);
    ctx->regs.a >>= 1;
    BIT_SET(ctx->regs.a, 7, b);

    cpu_set_flags(ctx, 0 , 0, 0, b);
}

static void proc_rra(cpu_context *ctx)
{
    cpu_set_flags(ctx, 0, 0, 0, BIT(ctx->regs.a, 0)); // first set flags before bit is shifted out

    ctx->regs.a >>= 1;
    BIT_SET(ctx->regs.a, 7, CPU_FLAG_C);
}

static void proc_rlca(cpu_context *ctx)
{
    bool b = BIT(ctx->regs.a, 7);
    ctx->regs.a <<= 1;
    BIT_SET(ctx->regs.a, 0, b);

    cpu_set_flags(ctx, 0 , 0, 0, b);
}

static void proc_rla(cpu_context *ctx)
{
    cpu_set_flags(ctx, 0, 0, 0, BIT(ctx->regs.a, 7)); // first set flags before bit is shifted out

    ctx->regs.a <<= 1;
    ctx->regs.a |= CPU_FLAG_C;
}

static void proc_stop(cpu_context *ctx) 
{
    //TODO implement stop
    fprintf(stderr, "STOPPING!\n");
}

static void proc_cpl(cpu_context *ctx)
{
    ctx->regs.a = ~ctx->regs.a;
    cpu_set_flags(ctx, -1, 1, 1, -1);
}

static void proc_scf(cpu_context *ctx)
{
    cpu_set_flags(ctx, -1, 0, 0, 1);
}

static void proc_ccf(cpu_context *ctx)
{
    cpu_set_flags(ctx, -1, 0, 0, !CPU_FLAG_C);
}

static IN_PROC processors[] = 
{
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_DI] = proc_di,
    [IN_EI] = proc_ei,
    [IN_XOR] = proc_xor,
    [IN_POP] = proc_pop,
    [IN_PUSH] = proc_push,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_JR] = proc_jr,
    [IN_RET] = proc_ret,
    [IN_RETI] = proc_reti,
    [IN_CALL] = proc_call,
    [IN_RST] = proc_rst,
    [IN_INC] = proc_inc,
    [IN_DEC] = proc_dec,
    [IN_ADD] = proc_add,
    [IN_ADC] = proc_adc,
    [IN_SUB] = proc_sub,
    [IN_SBC] = proc_sbc,
    [IN_AND] = proc_and,
    [IN_XOR] = proc_xor,
    [IN_OR] = proc_or,
    [IN_CP] = proc_cp,
    [IN_RRCA] = proc_rrca,
    [IN_RRA] = proc_rra,
    [IN_RLCA] = proc_rlca,
    [IN_RLA] = proc_rla,
    [IN_STOP] = proc_stop,
    [IN_CPL] = proc_cpl,
    [IN_SCF] = proc_scf,
    [IN_CCF] = proc_ccf
    //TODO add rest of proc functions for instructions
};

IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}