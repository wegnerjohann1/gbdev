#include <defs.h>
#include <emu.h>
#include <cpu.h>
#include <cart.h>
#include "raylib.h"
#include <pthread.h>
#include <unistd.h>

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

void *cpu_run(void *p)
{
    cpu_init();
    
    ctx.running = true;
    ctx.paused = false;
    ctx.stepped = false;
    ctx.ticks = 0;

    bool did_step = false;
    bool did_dump = false;

    while (ctx.running)    // Detect window close button or ESC key
    {   
        if(ctx.paused)
        {
            delay(10);
            continue;
        }

        if (IsKeyUp(KEY_M))
            did_step = false;
        
        if (IsKeyUp(KEY_D))
            did_dump = false;

        if (ctx.stepped)
        {
            if (IsKeyPressed(KEY_M) && !did_step)
            {
                if (!cpu_step())
                {
                    printf("CPU stopped\n");
                    return 0;
                }
                did_step = true;
            }

            if (IsKeyPressed(KEY_D) && !did_dump)
            {
                dump(RT_DE);
                did_dump = true;
            }
        }
        else if (!cpu_step())
        {
            printf("CPU stopped\n");
            return 0;
        }

        ctx.ticks++;
    }
    return 0;
}

int emu_run(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: emu <rom_file>\n");
        return WRONG_EXE_USAGE;
    }

    if(!cart_load(argv[1]))
    {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return FAILED_ROM_LOAD;
    }

    printf("Cart loaded..\n");

    pthread_t t1;

    if (pthread_create(&t1, NULL, cpu_run, NULL))
    {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
        return -1;
    }

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WIDTH * PIXELSIZE;
    const int screenHeight = HEIGHT * PIXELSIZE;

    SetTraceLogLevel(LOG_WARNING); 

    InitWindow(screenWidth, screenHeight, "gbdev");
    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        BeginDrawing();
        EndDrawing();
    }
    return 0;

}