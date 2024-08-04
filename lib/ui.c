#include <ui.h>
#include <bus.h>
#include <raylib.h>

static int scale = 4;

void ui_init()
{
    SetTraceLogLevel(LOG_WARNING); 

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gbdev");
    SetTargetFPS(60);
}

static u32 tile_colors[4] = {0xFFFFFFFF, 0xAAAAAAFF, 0x555555FF, 0x000000FF};

void draw_tile(u16 startLocation, u16 tileNum, int x, int y)
{
    Rectangle rc;

        for (int tileY = 0; tileY < 16; tileY += 2)
        {
            u8 b1 = bus_read(startLocation + (tileNum * 16) + tileY);
            u8 b2 = bus_read(startLocation + (tileNum * 16) + tileY + 1);
            
            for (int bit = 7; bit >= 0; bit--)
            {
                u8 hi = !!(b1 & (1 << bit)) << 1;
                u8 lo = !!(b2 & (1 << bit));

                Color cl = GetColor(tile_colors[hi | lo]);

                rc.x = x + (7-bit) * scale;
                rc.y = y + (tileY / 2) * scale;
                rc.width = scale;
                rc.height = scale;

                DrawRectangleRec(rc, cl);
            }
        }
}

void ui_handle_events()
{

}

void ui_update()
{

}