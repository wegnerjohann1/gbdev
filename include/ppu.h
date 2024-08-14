#pragma once

#include <defs.h>

static const int LINES_PER_FRAME = 154;
static const int TICKS_PER_LINE = 456;
static const int YRES = 144;
static const int XRES = 160;

typedef struct
{
    u8 y;
    u8 x;
    u8 tile;

    u8 f_cgb_pn : 3;
    u8 f_cgb_vram_bank : 1;
    u8 f_pn : 1;
    u8 f_x_flip : 1;
    u8 f_y_flip : 1;
    u8 f_bgp : 1;

} oam_entry;

typedef struct 
{
    oam_entry oam_ram[40];
    u8 vram[0x2000];

    u32 current_frame;
    u32 line_ticks;
    u32 *video_buffer;
} ppu_context;

ppu_context *ppu_get_context();

void ppu_init();
void ppu_tick();

void ppu_oam_write(u16 address, u8 value);
u8 ppu_oam_read(u16 address);

void ppu_vram_write(u16 address, u8 value);
u8 ppu_vram_read(u16 address);