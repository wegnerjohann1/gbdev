#include <bus.h>
#include <cart.h>
#include <ram.h>
#include <cpu.h>
#include <io.h>
#include <ppu.h>
#include <dma.h>

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

u8 bus_read(u16 address)
{
    if (address < 0x8000)
    {
        return cart_read(address);
    } 
    else if (address < 0xA000)
    {
        return ppu_vram_read(address);
    }
    else if (address < 0xC000)
    {
        return cart_read(address);
    }
    else if (address < 0xE000)
    {
        return wram_read(address);
    }
    else if (address < 0xFE00)
    {
        printf("UNSUPPORTED bus_read(%04X)\n", address);
        //TODO Implement Echo RAM
        //TODO WHY DO I GET INTO HERE AND IT SPAMS THIS FUNCTION OVER THE WHOLE RANGE?!?!??!?!
        return 0;
    }
    else if (address < 0xFEA0)
    {
        if (dma_transferring())
            return 0xFF;
        
        return ppu_oam_read(address);
    }
    else if (address < 0xFF00)
    {
        printf("UNSUPPORTED bus_read(%04X)\n", address);
        printf("ACCESS OF NINTENDO PROHIBITED MEMORY AREA\n");
        //TODO Nintendo prohibited AREA
        return 0;
    }
    else if (address < 0xFF80)
    {
        return io_read(address);
    }
    else if (address < 0xFFFF)
    {
        return hram_read(address);
    }
    else if (address == 0xFFFF)
    {
        return cpu_get_ie_register();
    }
    else
    {
        printf("Address outside of memory range: %04X\n", address);
        exit(MEMORY_OUT_OF_RANGE);
    }

}

void bus_write(u16 address, u8 value)
{
    if (address < 0x8000)
    {
        cart_write(address, value);
    }
    else if (address < 0xA000)
    {
        ppu_vram_write(address, value);
    }
    else if (address < 0xC000)
    {
        cart_write(address, value);
    }
    else if (address < 0xE000)
    {
        wram_write(address, value);
    }
    else if (address < 0xFE00)
    {
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        //TODO Implement Echo RAM
    }
    else if (address < 0xFEA0)
    {
        if (dma_transferring())
            return;
        
        ppu_oam_write(address, value);
    }
    else if (address < 0xFF00)
    {
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        printf("ACCESS OF NINTENDO PROHIBITED MEMORY AREA\n");
        //TODO Nintendo prohibited AREA
    }
    else if (address < 0xFF80)
    {
        io_write(address, value);
    }
    else if (address < 0xFFFF)
    {
        hram_write(address, value);
    }
    else if (address == 0xFFFF)
    {
        cpu_set_ie_register(value);
    }
    else
    {
        printf("Address outside of memory range: %04X\n", address);
        exit(MEMORY_OUT_OF_RANGE);
    }
}

u16 bus_read16(u16 address)
{
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);

    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value)
{
    // write 0xABCD in memory as 0xCD 0xAB
    bus_write(address + 1, (value >> 8) & 0x00FF);
    bus_write(address, value & 0x00FF);
}