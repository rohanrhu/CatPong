/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct catpong_input_mouse_state catpong_input_mouse_state_t;
struct catpong_input_mouse_state {
    int x;
    int y;
    bool left_button;
    bool right_button;
};

void catpong_input_get_mouse_state(catpong_input_mouse_state_t *state);