#include <io.h>
#include <cpu.h>
#include <timer.h>
#include <dma.h>
#include <lcd.h>
#include <apu.h>

static char serial_data[2];

u8 io_read(u16 address)
{
    if (address == 0xFF01)
    {
        return serial_data[0];
    }

    if (address == 0xFF02)
    {
        return serial_data[1];
    }

    if (BETWEEN(address, 0xFF04, 0xFF07))
    {
        return timer_read(address);
    }
    
    if (address == 0xFF0F)
    {
        return cpu_get_int_flags();
    }

    if (BETWEEN(address, 0xFF10, 0xFF3F))
    {
        return apu_read(address);
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B))
    {
        return lcd_read(address);
    }

    printf("UNSUPPORTED IO_bus_read(%04X)\n", address);
        //TODO IO Registers
    return 0;
}

void io_write(u16 address, u8 value)
{
    if (address == 0xFF01)
    {
        serial_data[0] = value;
        return;
    }

    if (address == 0xFF02)
    {
        serial_data[1] = value;
        return;
    }

    if (BETWEEN(address, 0xFF04, 0xFF07))
    {
        return timer_write(address, value);
    }
    
    if (address == 0xFF0F)
    {
        return cpu_set_int_flags(value);
    }

    if (BETWEEN(address, 0xFF10, 0xFF3F))
    {
        apu_write(address, value);
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B))
    {
        return lcd_write(address, value);
    }

    printf("UNSUPPORTED IO_bus_write(%04X)\n", address);
        //TODO IO Registers
}