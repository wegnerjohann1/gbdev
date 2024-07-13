#include <ram.h>

static ram_context ctx;

u8 wram_read(u16 address)
{
    address -= 0xC000;

    if (address >= 0x2000)
    {
        printf("INVALID WRAM ADDR %04X\n", address + 0x2000);
        exit(MEMORY_OUT_OF_RANGE);
    }

    return ctx.wram[address];
}

void wram_write(u16 address, u8 value)
{
    address -= 0xC000;
    
    if (address >= 0x2000)
    {
        printf("INVALID WRAM ADDR %04X\n", address + 0x2000);
        exit(MEMORY_OUT_OF_RANGE);
    }

    ctx.wram[address] = value;
}

u8 hram_read(u16 address)
{
    address -= 0xFF80;

    if (address >= 80)
    {
        printf("INVALID HRAM ADDR %04X\n", address + 0xFF80);
        exit(MEMORY_OUT_OF_RANGE);
    }

    return ctx.hram[address];
}

void hram_write(u16 address, u8 value)
{
    address -= 0xFF80;

    if (address >= 80)
    {
        printf("INVALID HRAM ADDR %04X\n", address + 0xFF80);
        exit(MEMORY_OUT_OF_RANGE);
    }

    ctx.hram[address] = value;
}
