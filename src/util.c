/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

uint64_t catpong_util_get_timestamp_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

unsigned int catpong_random_int() {
    unsigned int seed;
    
    FILE* file = fopen("/dev/random", "r");
    if (!file) {
        return 0;
    }
    fread(&seed, 4, 1, file);
    fclose(file);

    return seed;
}

unsigned int catpong_random_int_range(unsigned int min, unsigned int max) {
    return min + (catpong_random_int() % (max - min));
}

double catpong_util_random_angle() {
    return(catpong_random_int() / UINT32_MAX) * M_PI*2;
}

double catpong_util_random_angle_range(double min, double max) {
    return min + (catpong_random_int() / UINT32_MAX) * (max - min);
}

double catpong_util_random_angle_tophalf() {
    unsigned int degrees = catpong_random_int_range(30, 150);
    return (M_PI - (degrees * (M_PI / 180))) * -1;
}

double catpong_util_random_angle_bottomhalf() {
    unsigned int degrees = catpong_random_int_range(30, 150);
    return M_PI - (degrees * (M_PI / 180));
}

double catpong_util_random_angle_lefthalf() {
    unsigned int degrees = catpong_random_int_range(30, 150);
    return (degrees * (M_PI / 180)) * -1;
}

double catpong_util_random_angle_righthalf() {
    unsigned int degrees = catpong_random_int_range(30, 150);
    return degrees * (M_PI / 180);
}