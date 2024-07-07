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

static void fetch_data()
{
    ctx.mem_dest = 0;
    ctx.dest_is_mem = false;

    switch (ctx.cur_inst -> mode)
    {
    case AM_IMP: 
        return;

    case AM_R:
        ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_1);
        return;
    
    case AM_R_R:
        ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);
        return;
    
    case AM_R_D8:
        ctx.fetched_data = bus_read(ctx.regs.PC);
        emu_cycles(1);
        ctx.regs.PC++;
        return;
    
    case AM_R_D16:
        {
            u16 lo = bus_read(ctx.regs.PC);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.PC + 1);
            emu_cycles(1);

            ctx.fetched_data = lo | (hi << 8);
            ctx.regs.PC += 2;
        }
        return;
    
    case AM_A16:
        {
            u16 lo = bus_read(ctx.regs.PC);
            emu_cycles(1);

            u16 hi = bus_read(ctx.regs.PC + 1);
            emu_cycles(1);

            ctx.fetched_data = lo | (hi << 8);
            ctx.regs.PC += 2;
        }
        return;

    case AM_MR:
        ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
        ctx.dest_is_mem = true;
        ctx.fetched_data = bus_read(ctx.mem_dest);
        emu_cycles(1);
        return;
    
    case AM_MR_R:
        ctx.mem_dest = cpu_read_reg(ctx.cur_inst->reg_1);
        ctx.dest_is_mem = true;
        ctx.fetched_data = cpu_read_reg(ctx.cur_inst->reg_2);

        if (ctx.cur_inst->reg_1 == RT_C)
        {
            ctx.mem_dest |= 0xFF00;
        }

        return;

    case AM_R_MR:
        u16 addr = cpu_read_reg(ctx.cur_inst->reg_2);

        if (ctx.cur_inst->reg_2 == RT_C)
        {
            addr |= 0xFF00;
        }
        ctx.fetched_data = bus_read(addr);
        emu_cycles(1);
        return; 

    //TODO IMPLEMENT OTHER ADDRESSING MODES       

    default:
        printf("Unknown Addressing Mode! %d (%02X)\n", ctx.cur_inst->mode, ctx.cur_opcode);
        exit(-7);
        return;
    }
}

static void execute()
{
    printf("Not executing yet...\n");
}

bool cpu_step()
{
    if (!ctx.halted)
    {
        fetch_instruction();
        fetch_data();
        execute();
    }
    
    return false;
}
