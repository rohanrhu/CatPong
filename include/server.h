/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <netinet/in.h>

typedef enum {
    CATPONG_SERVER_PACKET_JOIN = 1,
    CATPONG_SERVER_PACKET_DISCONNECTED,
    CATPONG_SERVER_PACKET_STARTING,
    CATPONG_SERVER_PACKET_START,
    CATPONG_SERVER_PACKET_END,
    CATPONG_SERVER_PACKET_MOVE,
    CATPONG_SERVER_PACKET_STATE
} catpong_server_packet_opcode_t;

typedef struct catpong_server_packet_header catpong_server_packet_header_t;
typedef struct catpong_server_packet_move catpong_server_packet_move_t;
typedef struct catpong_server_packet_state catpong_server_packet_state_t;
typedef struct catpong_server_peer catpong_server_peer_t;
typedef struct catpong_server catpong_server_t;

struct catpong_server_packet_header {
    uint8_t opcode;
};

struct catpong_server_packet_move {
    uint32_t y;
};

struct catpong_server_packet_state {
    uint32_t left_paddle;
    uint32_t right_paddle;
    uint32_t ball_x;
    uint32_t ball_y;
};

struct catpong_server_peer {
    catpong_server_t* server;
    struct sockaddr_in address;
    bool is_running;
    int socket;
    pthread_t thread;
    pthread_mutex_t mutex;
    void* owner;
    void (*on_join)(catpong_server_peer_t* peer);
    void (*on_disconnected)(catpong_server_peer_t* peer);
    void (*on_starting)(catpong_server_peer_t* peer);
    void (*on_start)(catpong_server_peer_t* peer);
    void (*on_end)(catpong_server_peer_t* peer);
    void (*on_move)(catpong_server_peer_t* peer, catpong_server_packet_move_t move);
    void (*on_state)(catpong_server_peer_t* peer, catpong_server_packet_state_t state);
};

struct catpong_server {
    catpong_server_peer_t* peer;
    catpong_server_peer_t* opponent_peer;
    bool is_running;
    struct sockaddr_in address;
    int socket;
    pthread_t thread;
    pthread_mutex_t mutex;
};

catpong_server_t* catpong_server_new(const char* address, int port);
void catpong_server_free(catpong_server_t* server);
void catpong_server_listen(catpong_server_t* server);
void catpong_server_stop(catpong_server_t* server);

catpong_server_peer_t* catpong_server_peer_new(catpong_server_t* server, struct sockaddr_in address, int socket);
void catpong_server_peer_free(catpong_server_peer_t* peer);
void catpong_server_peer_send(catpong_server_peer_t* peer, const unsigned char* message, size_t length);

catpong_server_peer_t* catpong_server_connect(const char* address, int port);
void catpong_server_disconnect(catpong_server_peer_t* peer);

void catpong_server_send(catpong_server_peer_t* peer, catpong_server_packet_opcode_t opcode, const void* data, size_t length);