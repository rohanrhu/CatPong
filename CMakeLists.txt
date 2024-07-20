# CatPong, Multiplayer Pong
# https://github.com/rohanrhu/CatPong
# 
# Copyright (C) 2024, Meowing Cat (https://meowingcat.io) <meowingcate@gmail.com>
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)

project(CatPong VERSION 1.0 DESCRIPTION "CatPong, Multiplayer Pong")

set(PROJECT_HOMEPAGE_URL https://github.com/rohanrhu/CatPong)
set(PROJECT_DESCRIPTION "Multiplayer Pong for Cat's tutorial")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_compile_options(-O0 -ggdb -Wno-unused-command-line-argument -Wunused-variable -Wmissing-field-initializers -Werror=return-type)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/opt/homebrew/include)

execute_process(COMMAND find /opt/homebrew/Cellar -type d -name lib OUTPUT_VARIABLE SDL2_LIB_PATHS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE "\n" ";" SDL2_LIB_PATHS_LIST ${SDL2_LIB_PATHS})

find_library(SDL2_LIBRARY NAMES SDL2 HINTS ${SDL2_LIB_PATHS_LIST})
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf HINTS ${SDL2_LIB_PATHS_LIST})

add_definitions(-DCATPONG_VERBOSE=1)

execute_process(COMMAND date "+%d.%m.%Y %H:%M" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE REVISION OUTPUT_STRIP_TRAILING_WHITESPACE)
file(READ "${PROJECT_SOURCE_DIR}/build.txt" BUILD_NUMBER_RAW)
math(EXPR BUILD_NUMBER "${BUILD_NUMBER_RAW}+1")
file(WRITE "${PROJECT_SOURCE_DIR}/build.txt" "${BUILD_NUMBER}")
add_definitions(-DCATPONG_BUILD_DATE="${BUILD_DATE}")
add_definitions(-DCATPONG_BUILD=${BUILD_NUMBER})
add_definitions(-DCATPONG_REVISION="${REVISION}")

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")

list(FILTER SOURCES EXCLUDE REGEX ".*catpong.c$")

add_executable(catpong ${PROJECT_SOURCE_DIR}/src/catpong.c ${SOURCES})

target_link_libraries(catpong ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})

set_target_properties(catpong PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_custom_target(clean_custom COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/build)

add_custom_command(TARGET catpong POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo "Build: ${BUILD_NUMBER}")