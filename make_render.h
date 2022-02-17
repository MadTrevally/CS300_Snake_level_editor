//
// Created by James Mullen on 04.02.22.
//
#ifndef MAKE_RENDER_H
#define MAKE_RENDER_H
#define FLAG_PLAIN_CELL 0x0
#define FLAG_SNAKE 0x1
#define FLAG_WALL 0x2
#define FLAG_FOOD 0x4
#include <stddef.h>

typedef struct board {
    size_t width;
    size_t height;
    int cursor;
    int* cells;
    int ready;
} board_t;

typedef enum input_key {
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_WALL,
    INPUT_EMPTY,
    INPUT_SNAKE,
    INPUT_DONE
} input_key_t;

void r_initialize_window(int width, int height);
void r_render_board(board_t* board);

#endif

