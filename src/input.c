/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "input.h"

void catpong_input_get_mouse_state(catpong_input_mouse_state_t *state) {
    int x;
    int y;

    uint32_t buttons = SDL_GetMouseState(&x, &y);
    state->x = x;
    state->y = y;
    state->left_button = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    state->right_button = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
}