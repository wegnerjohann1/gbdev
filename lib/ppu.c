#include <ppu.h>
#include <lcd.h>
#include <string.h>
#include <ppu_sm.h>

static ppu_context ctx = {0};

ppu_context *ppu_get_context()
{
    return &ctx;
}

void ppu_init()
{
    ctx.current_frame = 0;
    ctx.line_ticks = 0;
    ctx.video_buffer = (u32*)malloc(YRES * XRES * sizeof(u32));

    lcd_init();
    LCDS_MODE_SET(MODE_OAM);

    memset(ctx.oam_ram, 0, sizeof(ctx.oam_ram));
    memset(ctx.video_buffer, 0, YRES * XRES * sizeof(u32));
}

void ppu_tick()
{
    ctx.line_ticks++;

    switch (LCDS_MODE)
    {
    case MODE_OAM:
        ppu_mode_oam();
        break;
    case MODE_XFER:
        ppu_mode_xfer();
        break;
    case MODE_HBLANK:
        ppu_mode_hblank();
        break;
    case MODE_VBLANK:
        ppu_mode_vblank();
        break;
    }
}

void ppu_oam_write(u16 address, u8 value)
{
    if (address >= 0xFEA0 || address < 0xFE00)
    {
        printf("Invalid OAM Address: %04X", address);
        exit(MEMORY_OUT_OF_RANGE);
    }
    else if (address >= 0xFE00)
    {
        address -= 0xFE00;
    }

    u8 *p = (u8 *)ctx.oam_ram;
    p[address] = value;
}

u8 ppu_oam_read(u16 address)
{
    if (address >= 0xFEA0 || address < 0xFE00)
    {
        printf("Invalid OAM Address: %04X", address);
        exit(MEMORY_OUT_OF_RANGE);
    }
    else if (address >= 0xFE00)
    {
        address -= 0xFE00;
    }

    u8 *p = (u8 *)ctx.oam_ram;
    return p[address];
}

void ppu_vram_write(u16 address, u8 value)
{
    if (address >= 0xA000 || address < 0x8000)
    {
        printf("Invalid VRAM Address: %04X", address);
        exit(MEMORY_OUT_OF_RANGE);
    }
    else if (address >= 0x8000)
    {
        address -= 0x8000;
    }
    ctx.vram[address] = value;
}

u8 ppu_vram_read(u16 address)
{
    if (address >= 0xA000 || address < 0x8000)
    {
        printf("Invalid VRAM Address: %04X", address);
        exit(MEMORY_OUT_OF_RANGE);
    }
    else if (address >= 0x8000)
    {
        address -= 0x8000;
    }
    return ctx.vram[address];
}