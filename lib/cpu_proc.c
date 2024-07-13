#include <cpu.h>
#include <emu.h>
#include <bus.h>

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

static void proc_none(cpu_context *ctx)
{
    printf("INVALID INSTRUCTION!\n");
    exit(UNKNOWN_INSTRUCTION);
}

static void proc_nop(cpu_context *ctx)
{
    //TODO...
}

static void proc_ld(cpu_context *ctx)
{
    if (ctx->dest_is_mem)
    {
        if (ctx->cur_inst->reg_2 >= RT_AF)
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
        // TODO understand why this work also for subtraction in binary
        // get first 4 bits of both summands and check if any bit after 4th is 1 ie >= 0x10;
        bool hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0x000F) + (cpu_read_reg(ctx->fetched_data) & 0x000F) >= 0x10;
        // get first 8 bits of both summands and check if any bit after 8th is 1 ie >= 0x100;
        bool cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0x00FF) + (cpu_read_reg(ctx->fetched_data) & 0x00FF) >= 0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->cur_inst->reg_1, ctx->cur_inst->reg_2 + (s8)ctx->fetched_data);
        emu_cycles(1);
        return;
    }

    if (ctx->cur_inst->reg_2 >= RT_AF)
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

static void proc_xor(cpu_context *ctx)
{
    ctx->regs.a ^= ctx->fetched_data;
    cpu_set_flags(ctx, ctx->regs.a, 0 , 0, 0);

}

static void proc_jp(cpu_context *ctx)
{
    if (check_cond(ctx))
    {
        ctx->regs.PC = ctx->fetched_data;
        emu_cycles(1);
    }
}

static IN_PROC processors[] = 
{
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
    [IN_EI] = proc_ei,
    [IN_XOR] = proc_xor
    //TODO add rest of proc functions for instructions
};

IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}