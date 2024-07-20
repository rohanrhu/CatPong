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
#include <arpa/inet.h>
#include <pthread.h>

#include <SDL2/SDL.h>

#include "util.h"
#include "input.h"
#include "pong.h"
#include "ui.h"
#include "scene.h"
#include "server.h"

static void on_join(catpong_server_peer_t* peer) {
    catpong_pong_t* pong = peer->owner;

    if (pong->game_state == CATPONG_PONG_GAME_STATE_W4P) {
        pong->game_state = CATPONG_PONG_GAME_STATE_STARTING;

        catpong_server_send(peer, CATPONG_SERVER_PACKET_STARTING, NULL, 0);
    }
}

static void on_disconnected(catpong_server_peer_t* peer) {
    const catpong_pong_t* pong __attribute__((unused)) = peer->owner;
}

static void on_move(catpong_server_peer_t* peer, catpong_server_packet_move_t move) {
    catpong_pong_t* pong = peer->owner;

    pong->p2_mouse_state.y = move.y;
}

static void on_starting(catpong_server_peer_t* peer __attribute__((unused))) {
    catpong_pong_t* pong = peer->owner;

    printf("Game is starting.\n");

    pong->game_state = CATPONG_PONG_GAME_STATE_STARTING;
}

static void on_start(catpong_server_peer_t* peer __attribute__((unused))) {
    catpong_pong_t* pong = peer->owner;

    printf("Game started.\n");

    pong->game_state = CATPONG_PONG_GAME_STATE_PLAYING;
}

static void on_end(catpong_server_peer_t* peer __attribute__((unused))) {
    const catpong_pong_t* pong __attribute__((unused)) = peer->owner;

    printf("Game ended.\n");
}

static void on_state(catpong_server_peer_t* peer, catpong_server_packet_state_t state) {
    catpong_pong_t* pong = peer->owner;

    pong->paddle_left.y = state.left_paddle;
    pong->paddle_right.y = state.right_paddle;

    pong->ball_x = state.ball_x;
    pong->ball_y = state.ball_y;
}

bool catpong_scene_multiplayer(catpong_window_t* window, bool is_host) {
    catpong_pong_t* pong = catpong_pong_new(window, CATPONG_GAME_MODE_MULTIPLAYER | (is_host ? CATPONG_GAME_MODE_SERVER: 0));
    
    catpong_server_t* server = NULL;
    catpong_server_peer_t* peer = NULL;

    if (is_host) {
        server = catpong_server_new("127.0.0.1", 8090);
        peer = server->peer;

        if (!server) {
            printf("Failed to create the server.\n");
            return false;
        }

        peer->owner = pong;

        catpong_server_listen(server);
    } else {
        peer = catpong_server_connect("127.0.0.1", 8090);
        
        if (!peer) {
            printf("Failed to connect to the server.\n");
            return false;
        }

        peer->owner = pong;
    }

    peer->on_join = &on_join;
    peer->on_disconnected = &on_disconnected;
    peer->on_move = &on_move;
    peer->on_starting = &on_starting;
    peer->on_start = &on_start;
    peer->on_end = &on_end;
    peer->on_state = &on_state;

    catpong_pong_init(pong);

    pong->is_running = true;

    if (is_host) {
        pong->game_state = CATPONG_PONG_GAME_STATE_W4P;
    } else {
        pong->game_state = CATPONG_PONG_GAME_STATE_JOINING;
        catpong_server_send(peer, CATPONG_SERVER_PACKET_JOIN, NULL, 0);
    }

    bool is_started = false;

    while (pong->is_running) {
        pthread_mutex_lock(&peer->mutex);
        
        if (is_host) {
            catpong_input_get_mouse_state(&pong->p1_mouse_state);
            
            if ((pong->game_state == CATPONG_PONG_GAME_STATE_PLAYING) && !is_started) {
                is_started = true;
                catpong_server_send(peer->server->opponent_peer, CATPONG_SERVER_PACKET_START, NULL, 0);
            } else if  (pong->game_state == CATPONG_PONG_GAME_STATE_PLAYING) {
                catpong_server_packet_state_t state = {
                    .left_paddle = pong->paddle_left.y,
                    .right_paddle = pong->paddle_right.y,
                    .ball_x = pong->ball_x,
                    .ball_y = pong->ball_y
                };

                catpong_server_send(peer->server->opponent_peer, CATPONG_SERVER_PACKET_STATE, &state, sizeof(catpong_server_packet_state_t));
            }
        } else {
            catpong_input_mouse_state_t mouse_state;
            catpong_input_get_mouse_state(&mouse_state);
            
            catpong_server_packet_move_t move = {.y = mouse_state.y};
            catpong_server_send(peer, CATPONG_SERVER_PACKET_MOVE, &move, sizeof(move));
        }

        catpong_pong_update(pong);

        pthread_mutex_unlock(&peer->mutex);

        SDL_Delay(16);
    }

    return true;
}