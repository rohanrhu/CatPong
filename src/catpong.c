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

#include "catpong.h"
#include "util.h"
#include "input.h"
#include "ui.h"
#include "scene.h"

int main() {
    printf("CatPong, Multiplayer Pong\n");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    catpong_window_t *window = catpong_window_new("CatPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    catpong_scene_menu(window);
    
    catpong_window_free(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}