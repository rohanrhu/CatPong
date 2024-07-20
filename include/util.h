/**
 * CatPong, Multiplayer Pong
 * https://github.com/rohanrhu/CatPong
 * 
 * Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
 * Licensed under the MIT License.
 */

#pragma once

#include <stdint.h>

uint64_t catpong_util_get_timestamp_ms();

double catpong_util_random_angle();
double catpong_util_random_angle_range(double min, double max);
double catpong_util_random_angle_righthalf();
double catpong_util_random_angle_lefthalf();
double catpong_util_random_angle_tophalf();
double catpong_util_random_angle_bottomhalf();