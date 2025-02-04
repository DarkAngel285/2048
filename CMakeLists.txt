cmake_minimum_required(VERSION 3.31.3)
project(2048)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES ${PROJECT_SOURCE_DIR}/src/termios_init.c
            ${PROJECT_SOURCE_DIR}/src/game.c)
add_executable(2048 ${SOURCES})
target_include_directories(2048 PRIVATE ${PROJECT_SOURCE_DIR}/include)
