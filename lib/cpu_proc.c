#include <cpu.h>

// proccess cpu instruction

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
    //TODO...
}

static void proc_jp(cpu_context *ctx)
{
    //TODO...
}

static IN_PROC processors[] = 
{
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp
};