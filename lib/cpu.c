#include <cpu.h>
#include <bus.h>
#include <emu.h>
#include <interrupts.h>
#include <dbg.h>
#include <timer.h>

#define CPU_DEBUG 0

cpu_context ctx = { 0 };

void cpu_init()
{
   ctx.regs.PC = 0x100;
    ctx.regs.SP = 0xFFFE;
    *((short *)&ctx.regs.a) = 0xB001;
    *((short *)&ctx.regs.b) = 0x1300;
    *((short *)&ctx.regs.d) = 0xD800;
    *((short *)&ctx.regs.h) = 0x4D01;
    ctx.ie_register = 0;
    ctx.int_flags = 0;
    ctx.int_master_enabled = false;
    ctx.enabling_ime = false;

    timer_get_context()->div = 0xABCC;
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
        fetch_data();

#if CPU_DEBUG == 1
        char flags[16];
        sprintf(flags, "%c%c%c%c", ctx.regs.f & (1 << 7) ? 'Z' : '-',
                ctx.regs.f & (1 << 6) ? 'N' : '-',
                ctx.regs.f & (1 << 5) ? 'H' : '-',
                ctx.regs.f & (1 << 4) ? 'C' : '-');

        char inst[16];
        inst_to_str(&ctx, inst);

        printf("%08llX - %04X: %-12s (%02X %02X %02X) A: %02X F: %s BC: %02X%02X DE: %02X%02X HL: %02X%02X SP: %04X DIV: %02X\n",
               emu_get_context() -> ticks, pc, inst, ctx.cur_opcode,
               bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, flags,
               ctx.regs.b, ctx.regs.c, ctx.regs.d, ctx.regs.e,
               ctx.regs.h, ctx.regs.l, ctx.regs.SP, bus_read(0xFF04));
#endif

        // if (dbg_update())
        //     dbg_print();

        execute();

    }
    else
    {
        //is halted
        emu_cycles(1);

        if (ctx.int_flags)
        {
            ctx.halted = false;
        }
    }

    if (ctx.int_master_enabled)
    {
        cpu_handle_interrupts(&ctx);
        ctx.enabling_ime = false;
    }

    if (ctx.enabling_ime)
    {
        ctx.int_master_enabled = true;
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

void cpu_request_interrupt(interrupt_type it)
{
    ctx.int_flags |= it;
}

void dump(reg_type rt)
{
    u16 address = cpu_read_reg(rt) & 0xFFF0;
    for (int i = 0; i < 0x10; i++)
        printf("%02X ", i);
    printf("\n");
    for (int i = 0; i < 0x10; i++)
        printf("%02X ", bus_read(address + i));
    printf("%04X\n", address);
    
}