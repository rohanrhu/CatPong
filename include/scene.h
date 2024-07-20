/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

#include "ui.h"

typedef enum {
    CATPONG_SCENE_MENU,
    CATPONG_SCENE_SINGLEPLAYER,
    CATPONG_SCENE_MULTIPLAYER
} catpong_scene_t;

static catpong_scene_t scene = CATPONG_SCENE_MENU;

void catpong_scene_menu(catpong_window_t* window);
void catpong_scene_singleplayer(catpong_window_t* window);
bool catpong_scene_multiplayer(catpong_window_t* window, bool is_host);