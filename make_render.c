//
// Created by James Mullen on 04.02.22.
//

#define _XOPEN_SOURCE_EXTENDED 1

#include <locale.h>
#include <curses.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "make_render.h"

#define COLOR_BASE 1
#define COLOR_SNAKE 2
#define COLOR_WALL 3
#define COLOR_FOOD 4
#define COLOR_TEXT 5

#define BOARD_OFFSET_X 0
#define BOARD_OFFSET_Y 0

#define ADD(Y, X, C) mvaddch(Y + BOARD_OFFSET_Y, X + BOARD_OFFSET_X, C)
#define ADDW(Y, X, C) mvadd_wch(Y + BOARD_OFFSET_Y, X + BOARD_OFFSET_X, C)
#define WRITEW(Y, X, ...) \
    mvprintw(Y + BOARD_OFFSET_Y, X + BOARD_OFFSET_X, __VA_ARGS__)

/** Helper function that checks the terminal size against the game board
 * dimensions. Arguments:
 *  - `game`: a pointer to the relevant game struct.
 */
void r_check_terminal_size(int width, int height) {
    /* DO NOT MODIFY THIS FUNCTION */
    // use ncurses to get terminal dimensions.
    int req_h = height;
    int req_w = width;
    if (LINES < req_h || COLS < req_w) {
        endwin();
        printf(
            "Terminal window must be at least %d by %d characters in size! "
            "Yours is %d by %d.\n",
            req_w, req_h, COLS, LINES);
        exit(1);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Helper function that initializes the ncurses window and checks the terminal
 * size. Arguments:
 *  - `game`: a pointer to the current game struct.
 */
void r_initialize_window(int width, int height) {
    /* DO NOT MODIFY THIS FUNCTION */
    // Ncurses setup
    setlocale(LC_ALL, "");

    initscr();

    // set keypad option to true (so getch returns a value representing a
    // pressed function key, instead of an escape sequence representing a
    // function key)
    keypad(stdscr, true);

    r_check_terminal_size(height, width);


    WINDOW* window = newwin(height, width, 0, 0);
    leaveok(window, 0);
    refresh();

    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLACK, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_BLUE, -1);
    init_pair(4, COLOR_RED, -1);
    init_pair(5, COLOR_WHITE, -1);
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Renders the current game's board.
 * Arguments:
 *  - `game`: a pointer to the current game struct.
 */
void r_render_board(board_t* board) {
    /* DO NOT MODIFY THIS FUNCTION */

    for (unsigned i = 0; i < board->width * board->height; ++i) {
        if (board->cells[i] & FLAG_SNAKE) {
            char c = 'S';
            ADD(i / board->width, i % board->width,
                c | COLOR_PAIR(COLOR_SNAKE));
        } else if (board->cells[i] & FLAG_FOOD) {
            char c = 'O';
            ADD(i / board->width, i % board->width, c | COLOR_PAIR(COLOR_FOOD));
        } else if (board->cells[i] & FLAG_WALL) {
            cchar_t c;
            setcchar(&c, L"\u2588", WA_NORMAL, COLOR_WALL, NULL);
            ADDW(i / board->width, i % board->width, &c);
        } else {
            char c = ' ';
            ADD(i / board->width, i % board->width, c);
        }
    }
    /*int cell_at_cursor = board->cells[board->cursor];
    char c;
    cchar_t c_w;
    switch(cell_at_cursor) {
        case(0):
            c = ' ';
            ADD(board->cursor / board->width, board->cursor / board->width, c);
        case(1):
            c = 'S';
            ADD(board->cursor / board->width, board->cursor / board->width, c);
        case(2):
            setcchar(&c_w, L"\u2588", WA_NORMAL, COLOR_WALL, NULL);
            // ADDW(board->cursor / board->width, board->cursor % board->width, &c);
    }*/
    move(board->cursor / board->width, board->cursor % board->width);
    setsyx(board->cursor / board->width, board->cursor % board->width);
    refresh();
    /* DO NOT MODIFY THIS FUNCTION */
}


