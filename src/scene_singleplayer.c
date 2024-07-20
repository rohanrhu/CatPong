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
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "util.h"
#include "input.h"
#include "pong.h"
#include "ui.h"
#include "scene.h"
#include "pong.h"

void catpong_scene_singleplayer(catpong_window_t* window) {
    catpong_pong_t* pong = catpong_pong_new(window, CATPONG_GAME_MODE_SINGLEPLAYER);
    catpong_pong_init(pong);

    pong->is_running = true;

    while (pong->is_running) {
        catpong_input_mouse_state_t mouse_state;
        catpong_input_get_mouse_state(&mouse_state);
        
        if (mouse_state.x < window->width / 2) {
            catpong_input_get_mouse_state(&pong->p1_mouse_state);
        } else {
            catpong_input_get_mouse_state(&pong->p2_mouse_state);
        }

        catpong_pong_update(pong);
    }
}