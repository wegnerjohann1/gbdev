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
    if (!ctx.halted)
    {
        u16 pc = ctx.regs.PC;
         
        fetch_instruction();
        
        //printf("Fetching Data for Instruction: %02X    PC: %04X\n", ctx.cur_opcode, pc);

        fetch_data();

        //Debug print
        printf("%04X: %-7s (%02X %02X %02X) A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X\n",
            pc, inst_name(ctx.cur_inst->type), ctx.cur_opcode,
            bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, ctx.regs.f, ctx.regs.b,
            ctx.regs.c, ctx.regs.d, ctx.regs.e, ctx.regs.h, ctx.regs.l);
        
        //printf("Executing Instruction: %02X    PC: %04X\n", ctx.cur_opcode, pc);

        execute();
    }
    
    return true;
}
