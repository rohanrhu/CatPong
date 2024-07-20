/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "ui.h"

#define CATPONG_PONG_PADDLE_VELICOTY_TIME 100

typedef enum {
    CATPONG_PONG_GAME_STATE_W4P,
    CATPONG_PONG_GAME_STATE_JOINING,
    CATPONG_PONG_GAME_STATE_STARTING,
    CATPONG_PONG_GAME_STATE_PLAYING,
    CATPONG_PONG_GAME_STATE_FINISHED
} catpong_pong_game_state_t;

typedef enum {
    CATPONG_GAME_MODE_SINGLEPLAYER = 0b1,
    CATPONG_GAME_MODE_MULTIPLAYER = 0b10,
    CATPONG_GAME_MODE_SERVER = 0b100
} catpong_game_mode_t;

typedef struct catpong_pong catpong_pong_t;

struct catpong_pong {
    bool is_server;
    bool is_running;
    bool is_playing;
    catpong_game_mode_t mode;
    catpong_window_t* window;
    catpong_label_t* title_label;
    catpong_button_t* back_button;
    catpong_label_t* w4p_label;
    catpong_label_t* starting_label;
    catpong_label_t* joining_label;
    catpong_label_t* counter_label;
    double ball_angle;
    int ball_x;
    int ball_y;
    float ball_velocity;
    SDL_Rect ball;
    SDL_Rect paddle_left;
    SDL_Rect paddle_right;
    int counter;
    uint32_t last_paddle_left_move;
    uint32_t last_paddle_right_move;
    catpong_input_mouse_state_t p1_mouse_state;
    catpong_input_mouse_state_t p2_mouse_state;
    catpong_pong_game_state_t game_state;
    uint32_t counter_t0;
    SDL_Event event;
    uint32_t t0;
};

catpong_pong_t* catpong_pong_new(catpong_window_t* window, catpong_game_mode_t mode);
void catpong_pong_init(catpong_pong_t* pong);
void catpong_pong_update(catpong_pong_t* pong);
void catpong_pong_finish(catpong_pong_t* pong);