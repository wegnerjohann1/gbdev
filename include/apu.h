#include <defs.h>

typedef struct
{
    u8 length_timer;
    u8 envelope;
    u8 frequency;
    u8 control;
} noise_channel;

typedef struct
{
    u8 dac;
    u8 length_timer;
    u8 volume_level;
    u8 period_low;
    u8 period_high;

} wave_channel;

typedef struct
{
    u8 sweep;
    u8 length_timer;
    u8 envelope;
    u8 period_low;
    u8 period_high;

    u16 period_divider;
} square_channel;


typedef struct
{
    u64 ticks;
    square_channel square1;
    square_channel square2; //has no sweep;
    wave_channel wave;
    noise_channel noise;
    u8 wave_pattern[16];

    u8 audio_master_control;
    u8 sound_panning;
    u8 master_volume;
    
} apu_context;

void apu_write(u16 address, u8 value);
u8 apu_read(u16 address);
void apu_tick();
void apu_init();
