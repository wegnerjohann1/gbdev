#include <apu.h>

static apu_context ctx;


void apu_init()
{
    ctx.ticks = 0;
}

void apu_write(u16 address, u8 value)
{

}

u8 apu_read(u16 address)
{
    return 0;
}

void apu_tick()
{
    //256 Hz actions
    ctx.ticks++;

    if ((ctx.ticks & 3) == 0)
    {
        //64 Hz actions
    }

    if (ctx.ticks & 1)
    {
        //128 Hz actions
    }
}