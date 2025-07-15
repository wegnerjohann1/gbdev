#include <apu.h>

static apu_context ctx;

void apu_init()
{
    ctx.ticks = 0;
}

void apu_write(u16 address, u8 value)
{
    // u8 offset = (address - 0xFF10);
    // u8 *p = NULL;
    // if (offset < 5) {
    //     p = (u8 *)&ctx.square1;
    // } else if (offset < 10) {
    //     p = (u8 *)&ctx.square2;
    // } else if (offset < 15) {
    //     p = (u8 *)&ctx.wave;
    // } else if (offset > 15 && offset < 20) {
    //     p = (u8 *)&ctx.noise;
    // } else if (offset > 0x1F) {
    //     p = (u8 *)ctx.wave_pattern;
    // }

    // p[offset] = value;

}

u8 apu_read(u16 address)
{
    switch (address) {
    case 0xFF26:
        return ctx.audio_master_control;

    case 0xFF25:
        return ctx.sound_panning;

    case 0xFF24:
        return ctx.master_volume;
    }

    return 0;
}

void apu_tick()
{
    //256 Hz actions
    ctx.ticks++;

    if ((ctx.ticks & 3) == 0) {
        //64 Hz actions
    }

    if (ctx.ticks & 1) {
        //128 Hz actions
    }
}

