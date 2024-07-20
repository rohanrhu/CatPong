/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "server.h"

catpong_server_t* catpong_server_new(const char* address, int port) {
    catpong_server_t* server = malloc(sizeof(catpong_server_t));
    server->peer = catpong_server_peer_new(server, (struct sockaddr_in) {0}, 0);
    server->opponent_peer = NULL;
    server->is_running = true;
    server->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = inet_addr(address);
    server->address.sin_port = htons(port);

    return server;
}

void catpong_server_free(catpong_server_t* server) {
    free(server);
}

static void server_thread_f(catpong_server_t* server) {
    printf("Server is listening...\n");

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server->socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket() failed.");
        exit(EXIT_FAILURE);
    }

    server->peer->socket = server->socket;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = server->address.sin_port;

    bind(server->socket, (struct sockaddr *) &address, sizeof(address));
    listen(server->socket, 1);

    int new_socket;
    new_socket = accept(server->socket, (struct sockaddr*) &address, (socklen_t*) &addrlen);

    printf("Connection: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    catpong_server_peer_t* peer = catpong_server_peer_new(server, address, new_socket);
    peer->socket = new_socket;
    peer->owner = server->peer->owner;

    server->opponent_peer = peer;

    while (true) {
        uint32_t opcode = 0;

        recv(peer->socket, &opcode, sizeof(opcode), 0);

        if (opcode == CATPONG_SERVER_PACKET_JOIN) {
            pthread_mutex_lock(&server->mutex);
            server->peer->on_join(peer);
            pthread_mutex_unlock(&server->mutex);
        } else if (opcode == CATPONG_SERVER_PACKET_MOVE) {
            catpong_server_packet_move_t move;
            recv(peer->socket, &move, sizeof(catpong_server_packet_move_t), 0);

            pthread_mutex_lock(&server->mutex);
            server->peer->on_move(peer, move);
            pthread_mutex_unlock(&server->mutex);
        }
    }
}

void catpong_server_listen(catpong_server_t* server) {
    pthread_create(&server->thread, NULL, (void*) &server_thread_f, server);
    pthread_detach(server->thread);
}

void catpong_server_stop(catpong_server_t* server) {
    pthread_mutex_lock(&server->mutex);
    server->is_running = false;
    close(server->socket);
    pthread_mutex_unlock(&server->mutex);
}

catpong_server_peer_t* catpong_server_peer_new(catpong_server_t* server, struct sockaddr_in address, int socket) {
    catpong_server_peer_t* peer = malloc(sizeof(catpong_server_peer_t));
    peer->is_running = false;
    peer->server = server;
    peer->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    peer->owner = NULL;

    peer->on_join = NULL;
    peer->on_disconnected = NULL;
    peer->on_starting = NULL;
    peer->on_start = NULL;
    peer->on_end = NULL;
    peer->on_move = NULL;

    peer->address = address;
    peer->socket = socket;

    return peer;
}

void catpong_server_peer_free(catpong_server_peer_t* peer) {
    free(peer);
}

void catpong_server_peer_send(catpong_server_peer_t* peer, const unsigned char* message, size_t length) {
    send(peer->socket, message, length, 0);
}

static void peer_thread_f(catpong_server_peer_t* peer) {
    uint32_t opcode = 0;

    RECEIVE:
    recv(peer->socket, &opcode, sizeof(opcode), 0);

    if (opcode == CATPONG_SERVER_PACKET_STARTING) {
        pthread_mutex_lock(&peer->mutex);
        peer->on_starting(peer);
        pthread_mutex_unlock(&peer->mutex);
    } else if (opcode == CATPONG_SERVER_PACKET_START) {
        pthread_mutex_lock(&peer->mutex);
        peer->on_start(peer);
        pthread_mutex_unlock(&peer->mutex);
    } else if (opcode == CATPONG_SERVER_PACKET_END) {
        pthread_mutex_lock(&peer->mutex);
        peer->on_end(peer);
        pthread_mutex_unlock(&peer->mutex);
    } else if (opcode == CATPONG_SERVER_PACKET_STATE) {
        catpong_server_packet_state_t state;
        recv(peer->socket, &state, sizeof(catpong_server_packet_state_t), 0);
        
        pthread_mutex_lock(&peer->mutex);
        peer->on_state(peer, state);
        pthread_mutex_unlock(&peer->mutex);
    }

    if (peer->is_running) {
        goto RECEIVE;
    }
}

catpong_server_peer_t* catpong_server_connect(const char* address, int port) {
    catpong_server_peer_t* peer = catpong_server_peer_new(NULL, (struct sockaddr_in) {0}, 0);
    peer->is_running = true;

    struct sockaddr_in serv_addr;
    peer->socket = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, address, &serv_addr.sin_addr);

    if (connect(peer->socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed.\n");
        catpong_server_peer_free(peer);
        return NULL;
    }

    printf("Connected.\n");

    pthread_create(&peer->thread, NULL, (void*) &peer_thread_f, (void *) peer);
    pthread_detach(peer->thread);

    return peer;
}

void catpong_server_disconnect(catpong_server_peer_t* peer) {
    pthread_mutex_lock(&peer->mutex);
    peer->is_running = false;
    close(peer->socket);
    free(peer);
    pthread_mutex_unlock(&peer->mutex);
}

void catpong_server_send(catpong_server_peer_t* peer, catpong_server_packet_opcode_t opcode, const void* data, size_t length) {
    send(peer->socket, &opcode, sizeof(uint32_t), 0);

    if (data != NULL) {
        send(peer->socket, data, length, 0);
    }
}