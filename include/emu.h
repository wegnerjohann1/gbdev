#include <defs.h>

typedef struct 
{
    bool paused;
    bool running;
    u64 ticks;
} emu_context;

int emu_run(int argc, char** argv);

emu_context* emu_get_context();
