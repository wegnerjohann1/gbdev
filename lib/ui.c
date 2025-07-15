#include <ui.h>
#include <bus.h>
#include <raylib.h>
#include <ppu.h>

static int scale = 4;

void ui_init()
{
    SetTraceLogLevel(LOG_WARNING);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gbdev");
    SetTargetFPS(60);
}

static u32 tile_colors[4] = {0xFFFFFFFF, 0xAAAAAAFF, 0x555555FF, 0x000000FF}; //BLACK AND WHITE
//static u32 tile_colors[4] = {0x081820FF, 0x346856FF, 0x88c070FF, 0xe0f8d0FF}; // GB GREEN
//static u32 tile_colors[4] = {0xe0f8d0FF, 0x88c070FF, 0x346856FF, 0x081820FF}; // GB GREEN REVERSED

void draw_tile(u16 startLocation, u16 tileNum, int x, int y)
{
    Rectangle rc;

    for (int tileY = 0; tileY < 16; tileY += 2) {
        u8 b1 = bus_read(startLocation + (tileNum * 16) + tileY);
        u8 b2 = bus_read(startLocation + (tileNum * 16) + tileY + 1);

        for (int bit = 7; bit >= 0; bit--) {
            u8 hi = !!(b1 & (1 << bit)) << 1;
            u8 lo = !!(b2 & (1 << bit));

            Color cl = GetColor(tile_colors[hi | lo]);

            rc.x = x + (7 - bit) * scale;
            rc.y = y + (tileY / 2) * scale;
            rc.width = scale;
            rc.height = scale;

            DrawRectangleRec(rc, cl);
        }
    }
}

u32 get_ticks()
{
    return (u32)(GetTime() * 1000);
}

void delay(u32 ms)
{
    WaitTime((double) ms / 1000.);
}

void ui_handle_events()
{

}

void ui_update()
{
    Rectangle rc;
    rc.x = rc.y = 0;
    rc.width = rc.height = 2048;
    int offset_x = 32 * 16;

    u32 *videobuffer = ppu_get_context()->video_buffer;

    for (int line_num = 0; line_num < YRES; line_num++) {
        for (int x = 0; x < XRES; x++) {
            rc.x = x * scale + offset_x;
            rc.y = line_num * scale;
            rc.width = scale;
            rc.height = scale;

            Color cl = GetColor(videobuffer[x + (line_num * XRES)]);

            DrawRectangleRec(rc, cl);
        }
    }
}
