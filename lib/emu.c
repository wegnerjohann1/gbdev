#include <defs.h>
#include <emu.h>
#include <cpu.h>
#include <cart.h>
#include "raylib.h"

/*
    Emu components:

    |Cart|
    |CPU|
    |Address Bus|
    |PPU|
    |Timer|

*/

#define WIDTH 160
#define HEIGHT 144
#define PIXELSIZE 2
#define CYCLES 10

u8 memory[0xFFFF];

static emu_context ctx;

emu_context* emu_get_context()
{
    return &ctx;
}

void delay(u32 ms)
{
    WaitTime((double) ms / 1000.);
}

void emu_cycles(int m_cycles)
{
    //printf("emu_cycles not implemented yet\n");
}

int emu_run(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if(!cart_load(argv[1]))
    {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart loaded..\n");

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WIDTH * PIXELSIZE;
    const int screenHeight = HEIGHT * PIXELSIZE;

    SetTraceLogLevel(LOG_WARNING); 

    InitWindow(screenWidth, screenHeight, "gbdev");
    SetTargetFPS(60);

    cpu_init();
    
    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (!WindowShouldClose() && ctx.running)    // Detect window close button or ESC key
    {   
        if(ctx.paused)
        {
            delay(10);
            continue;
        }

        if (!cpu_step())
        {
            printf("CPU stopped\n");
            return -3;
        }
        
        BeginDrawing();
        EndDrawing();

        ctx.ticks++;
    }

    return 0;

}