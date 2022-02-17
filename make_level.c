//
// Created by James Mullen on 04.02.22.
//
#define _XOPEN_SOURCE_EXTENDED 1
#include "make_render.h"
#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

input_key_t r_get_input() {
    /* DO NOT MODIFY THIS FUNCTION */
    int input = getch();

    if (input == KEY_UP) {
        return INPUT_UP;
    } else if (input == KEY_DOWN) {
        return INPUT_DOWN;
    } else if (input == KEY_LEFT) {
        return INPUT_LEFT;
    } else if (input == KEY_RIGHT) {
        return INPUT_RIGHT;
    }
    else if (input == 'w') {
        return INPUT_WALL;
    }
    else if (input == 'e') {
        return INPUT_EMPTY;
    }
    else if (input == 's') {
        return INPUT_SNAKE;
    }
    else if (input == '\n') {
        return INPUT_DONE;
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

board_t* initialize_board(board_t* board, int width, int height, char* rep) {
    board->height = height;
    board->width = width;
    board->cursor = 0;
    board->cells = calloc(height * width, sizeof(int));
    for (int i = 0; i < height * width; i++) {
        if ((i < width) || (i % width == 0) || ((i + 1) % width == 0) ||
            (i > (height - 1) * width)) { board->cells[i] = 2; }
    board->ready = 0;

    return board;
}

void update_cursor(board_t* board, input_key_t input) {
    if (input == INPUT_DOWN) {
        board->cursor += board->width;
    }
    else if (input == INPUT_UP) {
        board->cursor += -board->width;
    }
    else if (input == INPUT_RIGHT) {
        board->cursor += 1;
    }
    else if (input == INPUT_LEFT) {
        board->cursor += -1;
    }
    if (board->cursor < 0) { board->cursor = 0; }
    if (board->cursor > (board->width * board->height)) { board->cursor = 0; }
}

void update_cell(board_t* board, input_key_t input) {
    if (input == INPUT_EMPTY) {
        board->cells[board->cursor] = 0;
    }
    else if (input == INPUT_SNAKE) {
        board->cells[board->cursor] = 1;
    }
    else if (input == INPUT_WALL) {
        board->cells[board->cursor] = 2;
    }
    else if (input == INPUT_DONE) {
        board->ready = 1;
    }
}

void r_update(board_t* board, input_key_t input) {
    update_cursor(board, input);
    update_cell(board, input);
};

char flag_to_char(int i) {
    switch (i) {
        case(0):
            return 'E';
        case(1):
            return 'S';
        case(2):
            return 'W';
    }
}

int get_digits(int num) {
    int digits= 0;
    do {
        num /= 10;
        ++digits;
    } while (num != 0);
    return digits;
}

char* encode(board_t board) {
    char cell = flag_to_char(board.cells[0]);
    int run_length = 0;
    int height = board.height;
    int width = board.width;

    char* encoding = malloc(board.height * board.width * 2 + board.height + 20);

    int hdigits = get_digits(height);

    int wdigits= get_digits(width);

    int dim_string_length = 4 + wdigits + hdigits;
    int digits;
    snprintf(encoding, dim_string_length, "B%lux%lu|", board.height, board.width);
    int str_length = strlen(encoding);
    for (int i = 0; i < (board.height * board.width); i++) {
        if (run_length == 0) {
            cell = flag_to_char(board.cells[i]);
            snprintf(encoding+str_length, 2, "%c", cell);
            str_length++;
            run_length++;
        }
        else if (cell == flag_to_char(board.cells[i])) {
            run_length++;
        }
        else {
            digits = get_digits(run_length);
            snprintf(encoding+str_length, digits + 1, "%d", run_length);
            str_length += digits;
            cell = flag_to_char(board.cells[i]);
            snprintf(encoding+str_length, 2, "%c", cell);
            str_length++;
            run_length=1;

        }
        if (((i+1) % width == 0)) {
            digits = get_digits(run_length);
            if ((i+1) != (width * height)) {
                snprintf(encoding + str_length, digits + 2, "%d|", run_length);
            }
            else {
                snprintf(encoding + str_length, digits + 2, "%d\n", run_length);
            }
            run_length = 0;
            str_length += digits + 1;
        }
    }
    return encoding;
}

int main(int argc, char** argv) {
    int height;
    int width;
    board_t board;
    switch (argc) {
        case (3):
            width = atoi(argv[1]);
            height = atoi(argv[2]);
            initialize_board(&board, width, height);
            r_initialize_window(width, height);
            break;
        case (4):
            width = atoi(argv[1]);
            height = atoi(argv[2]);
        default:
            printf("usage: make_level <width> <height>");
            return 0;
    }


    while (!((&board)->ready)) {
        r_render_board(&board);
        r_update(&board, r_get_input());
    }
    char* encoding = encode(board);
    free((&board)->cells);
    endwin();
    printf("%s", encoding);
}
