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

static void proc_none(cpu_context *ctx)
{
    printf("INVALID INSTRUCTION!\n");
    exit(-7);
}

static void proc_nop(cpu_context *ctx)
{
    //TODO...
}

static void proc_ld(cpu_context *ctx)
{
    if (ctx->dest_is_mem)
    {
        bus_write(ctx->mem_dest, ctx->fetched_data);
    }
    else
    {
        cpu_set_reg(ctx->cur_inst->reg_1, ctx->fetched_data);
    }
    
    //TODO check if its correct on github or maybe just leave it until i get errors :)
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
    [IN_JP] = proc_jp
    //TODO add rest of proc functions for instructions
};

IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}