#include <bus.h>
#include <cart.h>

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
        //TODO Implement VRAM
        return 0xFF;
    }
    else if (address < 0xC000)
    {
        //TODO Implement external RAM
        return 0xFF;
    }
    else if (address < 0xE000)
    {
        //TODO Implement WRAM
        return 0xFF;
    }
    else if (address < 0xFE00)
    {
        //TODO Implement Echo RAM
        return 0xFF;
    }
    else if (address < 0xFF00)
    {
        //TODO Nintendo prohibited AREA
        return 0xFF;
    }
    else if (address < 0xFF80)
    {
        //TODO IO Registers
        return 0xFF;
    }
    else if (address < 0xFFFF)
    {
        //TODO Implement HRAM
        return 0xFF;
    }
    else if (address == 0xFFFF)
    {
        //TODO Interrupt enable register
        return 0xFF;
    }
    else
    {
        printf("Address outside of memory range: %04X\n", address);
        exit(-8);
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
        //TODO Implement VRAM

    }
    else if (address < 0xC000)
    {
        //TODO Implement external RAM

    }
    else if (address < 0xE000)
    {
        //TODO Implement WRAM

    }
    else if (address < 0xFE00)
    {
        //TODO Implement Echo RAM

    }
    else if (address < 0xFF00)
    {
        //TODO Nintendo prohibited AREA

    }
    else if (address < 0xFF80)
    {
        //TODO IO Registers

    }
    else if (address < 0xFFFF)
    {
        //TODO Implement HRAM

    }
    else if (address == 0xFFFF)
    {
        //TODO Interrupt enable register

    }
    else
    {
        printf("Address outside of memory range: %04X\n", address);
        exit(-8);
    }
}