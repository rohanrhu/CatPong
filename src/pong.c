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

#include "pong.h"
#include "util.h"
#include "input.h"
#include "pong.h"
#include "ui.h"
#include "scene.h"
#include "sugar.h"

catpong_pong_t* catpong_pong_new(catpong_window_t* window, catpong_game_mode_t mode) {
    catpong_pong_t* pong = malloc(sizeof(catpong_pong_t));
    pong->mode = mode;
    pong->is_server = IS(mode, CATPONG_GAME_MODE_SERVER);
    pong->is_running = false;
    pong->is_playing = false;
    pong->window = window;

    pong->ball = (SDL_Rect) {
        .x = pong->window->width / 2 - 10,
        .y = pong->window->height / 2 - 10,
        .w = 20,
        .h = 20
    };

    pong->paddle_left = (SDL_Rect) {
        .x = 10,
        .y = pong->window->height / 2 - 50,
        .w = 10,
        .h = 100
    };

    pong->paddle_right = (SDL_Rect) {
        .x = pong->window->width - 20,
        .y = pong->window->height / 2 - 50,
        .w = 10,
        .h = 100
    };

    pong->ball_velocity = 5;

    pong->last_paddle_left_move = 0;
    pong->last_paddle_right_move = 0;

    return pong;
}

void catpong_pong_init(catpong_pong_t* pong) {
    pong->title_label = catpong_label_new(pong->window, CATPONG_UI_FONT_PATH, 32, IS(pong->mode, CATPONG_GAME_MODE_SINGLEPLAYER) ? "Singleplayer" : "Multiplayer", (SDL_Color) {255, 255, 255, 255});
    catpong_label_set_position(pong->title_label, pong->window->width / 2 - 100, 25);

    pong->back_button = catpong_button_new(pong->window, CATPONG_UI_FONT_PATH, 16, "X", (SDL_Color) {255, 255, 255, 255}, (SDL_Color) {0, 0, 0, 255});
    catpong_button_set_position(pong->back_button, pong->window->width - 50, 25);
    catpong_button_set_size(pong->back_button, 25, 40);

    pong->ball_angle = 0;
    pong->ball_x = pong->window->width / 2 - 10;
    pong->ball_y = pong->window->height / 2 - 10;

    pong->ball = (SDL_Rect) {
        .x = pong->ball_x,
        .y = pong->ball_y,
        .w = 20,
        .h = 20
    };

    pong->paddle_left = (SDL_Rect) {
        .x = 10,
        .y = pong->window->height / 2 - 50,
        .w = 10,
        .h = 100
    };

    pong->paddle_right = (SDL_Rect) {
        .x = pong->window->width - 20,
        .y = pong->window->height / 2 - 50,
        .w = 10,
        .h = 100
    };

    pong->counter = 3;

    pong->game_state = CATPONG_PONG_GAME_STATE_STARTING;
    
    pong->w4p_label = catpong_label_new(pong->window, CATPONG_UI_FONT_PATH, 32, "Waiting for opponent...", (SDL_Color) {255, 255, 255, 255});
    catpong_label_set_position(pong->w4p_label, pong->window->width / 2 - 220, 225);

    pong->joining_label = catpong_label_new(pong->window, CATPONG_UI_FONT_PATH, 32, "Joiningthe game...", (SDL_Color) {255, 255, 255, 255});
    catpong_label_set_position(pong->joining_label, pong->window->width / 2 - 200, 225);
    
    pong->starting_label = catpong_label_new(pong->window, CATPONG_UI_FONT_PATH, 32, "Game is starting...", (SDL_Color) {255, 255, 255, 255});
    catpong_label_set_position(pong->starting_label, pong->window->width / 2 - 200, 225);

    pong->counter_label = catpong_label_new(pong->window, CATPONG_UI_FONT_PATH, 32, "3", (SDL_Color) {255, 255, 255, 255});
    catpong_label_set_position(pong->counter_label, pong->window->width / 2 - 10, 250);

    pong->t0 = SDL_GetTicks();
    pong->counter_t0 = SDL_GetTicks();

    pong->is_running = true;

    pong->p1_mouse_state = (catpong_input_mouse_state_t) {0};
    pong->p2_mouse_state = (catpong_input_mouse_state_t) {0};
}

void catpong_pong_update(catpong_pong_t* pong) {
    int elapsed = SDL_GetTicks() - pong->t0;
    float delta = elapsed / 16.0;
    
    while (SDL_PollEvent(&pong->event)) {
        if (pong->event.type == SDL_QUIT) {
            pong->is_running = false;
        }
    }

    catpong_input_mouse_state_t mouse_state;
    catpong_input_get_mouse_state(&mouse_state);

    if (catpong_button_is_pressed_once(pong->back_button)) {
        scene = CATPONG_SCENE_MENU;
        pong->is_running = false;
    }

    catpong_window_clear(pong->window);

    if (pong->game_state == CATPONG_PONG_GAME_STATE_W4P) {
        catpong_label_update(pong->w4p_label);
    } else if (pong->game_state == CATPONG_PONG_GAME_STATE_JOINING) {
        catpong_label_update(pong->joining_label);
    } else if (pong->game_state == CATPONG_PONG_GAME_STATE_STARTING) {
        uint32_t counter_t1 = SDL_GetTicks();

        if (IS(pong->mode, CATPONG_GAME_MODE_SINGLEPLAYER) || IS(pong->mode, CATPONG_GAME_MODE_SERVER)) {
            if (pong->counter == 0) {
                pong->game_state = CATPONG_PONG_GAME_STATE_PLAYING;
            } else {
                if ((counter_t1 - pong->counter_t0) >= 1000) {
                    pong->counter_t0 = counter_t1;
                    pong->counter--;

                    if (IS(pong->mode, CATPONG_GAME_MODE_SINGLEPLAYER)) {
                        catpong_label_set_text(pong->counter_label, (char[]) {(char) (pong->counter + '0'), '\0'});
                    }
                }

                catpong_label_update(pong->counter_label);
            }
        } else {
            catpong_label_update(pong->starting_label);
        }
        
    } else if ((pong->game_state == CATPONG_PONG_GAME_STATE_PLAYING) && (IS(pong->mode, CATPONG_GAME_MODE_SERVER) || IS(pong->mode, CATPONG_GAME_MODE_SINGLEPLAYER))) {
        bool is_left_collided = SDL_HasIntersection(&pong->ball, &pong->paddle_left);
        bool is_right_collided = SDL_HasIntersection(&pong->ball, &pong->paddle_right);

        if (is_left_collided) {
            pong->ball_angle = catpong_util_random_angle_righthalf();
            pong->ball_x = pong->paddle_left.x + pong->paddle_left.w + 2;
        } else if (is_right_collided) {
            pong->ball_angle = catpong_util_random_angle_lefthalf();
            pong->ball_x = (pong->paddle_right.x - pong->ball.w) - 2;
        }

        bool is_top_collided = (pong->ball_y - pong->ball.h/2) <= 0;
        bool is_bottom_collided = (pong->ball_y + pong->ball.h/2) >= (pong->window->height - pong->ball.h);

        if (is_top_collided) {
            pong->ball_angle = catpong_util_random_angle_bottomhalf();
        } else if (is_bottom_collided) {
            pong->ball_angle = catpong_util_random_angle_tophalf();
        }

        pong->paddle_left.y = pong->p1_mouse_state.y - pong->paddle_left.h / 2;
        pong->paddle_right.y = pong->p2_mouse_state.y - pong->paddle_right.h / 2;

        double vx = cos(pong->ball_angle);
        double vy = sin(pong->ball_angle);
        double magnitude = sqrt(vx * vx + vy * vy);
    
        if (magnitude != 0) {
            vx /= magnitude;
            vy /= magnitude;
        }
        
        pong->ball_x += vx * pong->ball_velocity * delta;
        pong->ball_y += vy * pong->ball_velocity * delta;

        if (pong->ball_x <= 0) {
            printf("Player Two wins!\n");
            pong->is_running = false;
        } else if (pong->ball_x >= pong->window->width) {
            printf("Player One wins!\n");
            pong->is_running = false;
        }
    }

    pong->ball.x = pong->ball_x;
    pong->ball.y = pong->ball_y;

    SDL_SetRenderDrawColor(pong->window->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(pong->window->sdl_renderer, &pong->ball);

    SDL_SetRenderDrawColor(pong->window->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(pong->window->sdl_renderer, &pong->paddle_left);

    SDL_SetRenderDrawColor(pong->window->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(pong->window->sdl_renderer, &pong->paddle_right);

    catpong_label_update(pong->title_label);
    
    catpong_button_use_mouse_state(pong->back_button, &mouse_state);
    catpong_button_update(pong->back_button);

    catpong_window_update(pong->window);

    if (elapsed >= 16) {
        pong->t0 = SDL_GetTicks();
    }
}

void catpong_pong_finish(catpong_pong_t* pong) {
    catpong_label_free(pong->title_label);
    catpong_button_free(pong->back_button);
    catpong_label_free(pong->counter_label);
}