#include <ram.h>

static ram_context ctx;

u8 wram_read(u16 address)
{
    address -= 0xC000;

    if (address >= 2000)
    {
        printf("INVALID WRAM ADDR %04X\n", address + 2000);
        exit(MEMORY_OUT_OF_RANGE);
    }
}
void wram_write(u16 address, u8 value);

u8 hram_read(u16 address);
void hram_write(u16 address, u8 value);