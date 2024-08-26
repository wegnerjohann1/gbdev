#include <ppu.h>
#include <lcd.h>

void pixel_fifo_push(u32 value)
{
    fifo_entry *next = malloc(sizeof(fifo_entry));
    next->next = NULL;
    next->value = value;

    if (!ppu_get_context()->pfc.pixel_fifo.head)
    {
        //first entry...
        ppu_get_context()->pfc.pixel_fifo.head =  ppu_get_context()->pfc.pixel_fifo.tail = next;
    }
    else
    {
        ppu_get_context()->pfc.pixel_fifo.tail->next = next;
        ppu_get_context()->pfc.pixel_fifo.tail = next;
    }

    ppu_get_context()->pfc.pixel_fifo.size++;
}

u32 pixel_fifo_pop()
{
    if (ppu_get_context()->pfc.pixel_fifo.size <= 0)
    {
        fprintf(stderr, "ERR IN PIXEL FIFO!\n");
        exit(-4);
    }

    fifo_entry *popped = ppu_get_context()->pfc.pixel_fifo.head;
    ppu_get_context()->pfc.pixel_fifo.head = popped->next;
    ppu_get_context()->pfc.pixel_fifo.size--;

    u32 val = popped->value;
    free(popped);

    return val;
}

void pipeline_fetch()
{

}

void pipeline_push_pixel()
{
    if (ppu_get_context()->pfc.pixel_fifo.size > 8)
    {
        u32 pixel_data = pixel_fifo_pop();

        if (ppu_get_context()->pfc.line_x >= (lcd_get_context()->scroll_x % 8))
        {
            ppu_get_context()->video_buffer[ppu_get_context()->pfc.pushed_x + (lcd_get_context()->ly * XRES)] = pixel_data; // place pixel data onto video buffer

            ppu_get_context()->pfc.pushed_x++;
        }

        ppu_get_context()->pfc.line_x++;
    }
}

void pipeline_process()
{
    ppu_get_context()->pfc.map_y = (lcd_get_context()->ly + lcd_get_context()->scroll_y);
    ppu_get_context()->pfc.map_x = (ppu_get_context()->pfc.fetch_x + lcd_get_context()->scroll_x);
    ppu_get_context()->pfc.tile_y = ((lcd_get_context()->ly + lcd_get_context()->scroll_y) % 8) * 2;

    if (!(ppu_get_context()->line_ticks & 1))
    {
        pipeline_fetch();
    }

    pipeline_push_pixel();
}

void pipeline_fifo_reset()
{

}