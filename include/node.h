/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

typedef enum {
    CATPONG_NODE_TYPE_NONE,
    CATPONG_NODE_TYPE_WINDOW,
    CATPONG_NODE_TYPE_LABEL,
    CATPONG_NODE_TYPE_BUTTON
} catpong_node_type_t;

typedef struct catpong_node catpong_node_t;
struct catpong_node {
    catpong_node_type_t type;
};