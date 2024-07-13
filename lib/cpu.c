#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = { 0 };

void cpu_init()
{
    
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
        printf("Process for %s not implemented\n", inst_name(ctx.cur_inst->type));
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
        
        //Debug print
        printf("%04X: %-7s (%02X %02X %02X) AF: %04X BC: %04X DE: %04X HL: %04X SP: %04X\n",
            pc, inst_name(ctx.cur_inst->type), ctx.cur_opcode,
            bus_read(pc + 1), bus_read(pc + 2), cpu_read_reg(RT_AF),
            cpu_read_reg(RT_BC), cpu_read_reg(RT_DE),
            cpu_read_reg(RT_HL),  ctx.regs.SP);

        fetch_data();

        
        
        //printf("Executing Instruction: %02X    PC: %04X\n", ctx.cur_opcode, pc);

        execute();
    }
    
    return true;
}
