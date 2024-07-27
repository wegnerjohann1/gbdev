#include <dbg.h>
#include <bus.h>
#include <string.h>

static char dbg_msg[1024] = {0};
static int msg_size = 0;

bool passed = false;

bool dbg_update()
{
    if (bus_read(0xFF02) == 0x81)
    {
        char c = bus_read(0xFF01);

        dbg_msg[msg_size++] = c;

        bus_write(0xFF02, 1);
        return true;
    }
    return false;
}

void dbg_print()
{
    if (dbg_msg[0])
    {
        printf("DBG: %s\n", dbg_msg);
    }

    // if (strstr(dbg_msg, "Passed") && !passed)
    // {
    //     passed = true;
    //     printf("DBG: %s\n", dbg_msg);
    // }
        
}