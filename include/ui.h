#pragma once

#include <defs.h>

static const int SCREEN_WIDTH = 1152;
static const int SCREEN_HEIGHT = 768;

void ui_init();
void ui_handle_events();
void ui_update();
void draw_tile(u16 startLocation, u16 tileNum, int x, int y);