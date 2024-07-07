#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = { 0 };

void cpu_init()
{
    ctx.stepping = false;
}

static void fetch_instruction()
{
    ctx.cur_opcode = bus_read(ctx.regs.PC++);
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);

    if (ctx.cur_inst == NULL)
    {
        printf("Unknown Instruction! %d (%02X)\n", ctx.cur_inst->mode, ctx.cur_opcode);
        exit(-7);
    }

}

void fetch_data();

static void execute()
{
    IN_PROC proc = inst_get_processor(ctx.cur_inst->type);

    if (!proc) // check if proc == NULL
    {
        printf("Process for Instruction %02X not implemented\n", ctx.cur_opcode);
        return;
    }

    proc(&ctx);
}

bool cpu_step()
{
    if (ctx.stepping)
        getchar();
    if (!ctx.halted)
    {
        u16 pc = ctx.regs.PC;
         
        fetch_instruction();
        fetch_data();

        printf("Executing Instruction: %02X    PC: %04X\n", ctx.cur_opcode, pc);

        execute();
    }
    
    return true;
}
