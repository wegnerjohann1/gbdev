#pragma once

#include <defs.h>

typedef struct {
    bool start;
    bool select;
    bool a;
    bool b;
    bool up;
    bool down;
    bool left;
    bool right;
} gamepad_state;

void gamepad_init();
bool gamepad_button_sel();
bool gamepad_dir_sel();
void gamepad_set_sel(u8 value);
