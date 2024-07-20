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
#include "ui.h"
#include "scene.h"

void catpong_scene_menu(catpong_window_t* window) {
    catpong_label_t* title_label = catpong_label_new(window, CATPONG_UI_FONT_PATH, 32, "CatPong", (SDL_Color) {255, 255, 255, 255});
    catpong_label_set_position(title_label, window->width / 2 - 75, 25);

    catpong_button_t* host_button = catpong_button_new(window, CATPONG_UI_FONT_PATH, 16, "Host", (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255});
    catpong_button_set_position(host_button, window->width / 2 - 50, 150);
    catpong_button_set_size(host_button, 100, 50);

    catpong_button_t* connect_button = catpong_button_new(window, CATPONG_UI_FONT_PATH, 16, "Connect", (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255});
    catpong_button_set_position(connect_button, window->width / 2 - 100, 250);
    catpong_button_set_size(connect_button, 175, 50);

    catpong_button_t* play_singleplayer_button = catpong_button_new(window, CATPONG_UI_FONT_PATH, 16, "Play Singleplayer", (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255});
    catpong_button_set_position(play_singleplayer_button, window->width / 2 - 175, 350);
    catpong_button_set_size(play_singleplayer_button, 400, 50);

    catpong_button_t* exit_button = catpong_button_new(window, CATPONG_UI_FONT_PATH, 16, "Exit", (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255});
    catpong_button_set_position(exit_button, window->width / 2 - 50, 450);
    catpong_button_set_size(exit_button, 100, 50);

    SDL_Event event;

    while (window->is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                window->is_running = false;
            }
        }

        catpong_input_mouse_state_t mouse_state;
        catpong_input_get_mouse_state(&mouse_state);

        if (catpong_button_is_pressed_once(exit_button)) {
            window->is_running = false;
        }

        if (catpong_button_is_pressed_once(host_button)) {
            scene = CATPONG_SCENE_MULTIPLAYER;
            catpong_scene_multiplayer(window, true);
        }

        if (catpong_button_is_pressed_once(connect_button)) {
            scene = CATPONG_SCENE_MULTIPLAYER;
            catpong_scene_multiplayer(window, false);
        }

        if (catpong_button_is_pressed_once(play_singleplayer_button)) {
            scene = CATPONG_SCENE_SINGLEPLAYER;
            catpong_scene_singleplayer(window);
        }

        catpong_window_clear(window);

        catpong_label_update(title_label);

        catpong_button_use_mouse_state(host_button, &mouse_state);
        catpong_button_update(host_button);

        catpong_button_use_mouse_state(connect_button, &mouse_state);
        catpong_button_update(connect_button);
        
        catpong_button_use_mouse_state(play_singleplayer_button, &mouse_state);
        catpong_button_update(play_singleplayer_button);

        catpong_button_use_mouse_state(exit_button, &mouse_state);
        catpong_button_update(exit_button);

        catpong_window_update(window);

        SDL_Delay(16);
    }
    
    catpong_label_free(title_label);
    catpong_button_free(play_singleplayer_button);
    catpong_button_free(exit_button);
}