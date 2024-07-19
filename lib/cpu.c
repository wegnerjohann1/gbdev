#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = { 0 };

void cpu_init()
{
    ctx.regs.PC = 0x100;
}

static void fetch_instruction()
{
    ctx.cur_opcode = bus_read(ctx.regs.PC++);
    ctx.cur_inst = instruction_by_opcode(ctx.cur_opcode);

    if (ctx.cur_inst == NULL)
    {
        printf("Unknown Instruction! %d (%02X)\n", ctx.cur_inst->mode, ctx.cur_opcode);
        exit(UNKNOWN_INSTRUCTION);
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
        emu_cycles(1);
        
        //printf("Fetching Data for Instruction: %02X    PC: %04X\n", ctx.cur_opcode, pc);
        
        //Debug print
        // prints state of state before the instruction displayed is executed

        char flags[16];
        sprintf(flags, "%c%c%c%c", ctx.regs.f & (1 << 7) ? 'Z' : '-',
                ctx.regs.f & (1 << 6) ? 'N' : '-',
                ctx.regs.f & (1 << 5) ? 'H' : '-',
                ctx.regs.f & (1 << 4) ? 'C' : '-');

        printf("%08llX - %04X: %-7s (%02X %02X %02X) A: %02X F: %s BC: %02X%02X DE: %02X%02X HL: %02X%02X SP: %04X\n",
               emu_get_context() -> ticks, pc, inst_name(ctx.cur_inst->type), ctx.cur_opcode,
               bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, flags,
               ctx.regs.b, ctx.regs.c, ctx.regs.d, ctx.regs.e,
               ctx.regs.h, ctx.regs.l, ctx.regs.SP);

        fetch_data();

        //printf("Executing Instruction: %02X    PC: %04X\n", ctx.cur_opcode, pc);

        execute();
    }
    
    return true;
}


u8 cpu_get_ie_register()
{
    return ctx.ie_register;
}

void cpu_set_ie_register(u8 val)
{
    ctx.ie_register = val;
}