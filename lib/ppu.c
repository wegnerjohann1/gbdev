#include <ppu.h>

static ppu_context ctx = {0};

void ppu_init()
{

}

void ppu_tick()
{

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