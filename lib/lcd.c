#include <lcd.h>
#include <ppu.h>
#include <dma.h>

static lcd_context ctx;

static u32 colors_default[4] = {0x081820FF, 0x346856FF, 0x88c070FF, 0xe0f8d0FF};

void lcd_init()
{
     //registers...
    ctx.lcdc = 0x91;
    ctx.lcds = 0x85;
    ctx.scroll_y = 0;
    ctx.scroll_x = 0;
    ctx.ly = 0;
    ctx.ly_compare = 0;
    ctx.dma = 0xFF;
    ctx.bg_palette = 0xFC;
    ctx.obj_palette[0] = 0xFF;
    ctx.obj_palette[1] = 0xFF;
    ctx.win_y = 0;
    ctx.win_x = 0;
    
    //other data...
    for (int i = 0; i < 4; i++)
    {
        ctx.bg_colors[i] = colors_default[i];
        ctx.sp1_colors[i] = colors_default[i];
        ctx.sp2_colors[i] = colors_default[i];
    }
}

lcd_context *lcd_get_context()
{
    return &ctx;
}

u8 lcd_read(u16 address)
{
    u8 offset = (address - 0xFF40);
    u8 *p = (u8 *)&ctx;

    return p[offset];
}

void update_palette(u8 palette_data, u8 pal)
{
    u32 *p_colors;

    switch (pal)
    {
    case 0:
        p_colors = ctx.bg_colors;
        break;
    case 1:
        p_colors = ctx.sp1_colors;
        break;
    case 2:
        p_colors = ctx.sp2_colors;
        break;
    default:
        printf("NOT A VALID PALETTE NUMBER\n");
        exit(UNDEFINED_BEHAVIOUR);
        break;
    }

    p_colors[0] = colors_default[palette_data & 0b11];
    p_colors[1] = colors_default[(palette_data >> 2) & 0b11];
    p_colors[2] = colors_default[(palette_data >> 4) & 0b11];
    p_colors[3] = colors_default[(palette_data >> 6) & 0b11];
    

}

void lcd_write(u16 address, u8 value)
{
    u8 offset = (address - 0xFF40);
    u8 *p = (u8 *)&ctx;
    p[offset] = value;
    
    if (offset == 6)
    {
        //0xFF46 DMA
        dma_start(value);
        printf("DMA START!\n");
    }
    if (offset == 7)
    {
        update_palette(value, 0);
    }
    if (offset == 8)
    {
        update_palette(value, 1); // & 0b11111100 because id = 0 is transparent for obj
    }
    if (offset == 9)
    {
        update_palette(value, 2); // & 0b11111100 because id = 0 is transparent for obj
    }

}