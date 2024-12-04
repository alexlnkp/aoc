#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define WORDSEARCH_PATH "input/4.txt"

#define NUM_ROWS 140
#define NUM_COLS 142

char** init_board(void) {
    char** board = malloc(NUM_ROWS * sizeof(char*));
    for (int i = 0; i < NUM_ROWS; ++i)
        board[i] = malloc(NUM_COLS * sizeof(char));

    return board;
}

void free_board(char** board) {
    for (int i = 0; i < NUM_ROWS; ++i) free(board[i]);
    free(board);
}

void read_into_board(char** board, const char* path) {
    FILE* fp = fopen(path, "r");

    char* buf = NULL;
    size_t len = 0;
    int board_counter = 0;

    while (getline(&buf, &len, fp) != -1 && board_counter < NUM_ROWS) {
        strncpy(board[board_counter], buf, NUM_COLS);
        board[board_counter][NUM_COLS - 1] = '\0';
        board_counter++;
    }
    free(buf);
    fclose(fp);
}

int find_xmas_word(char search_buf[5], char** board, int x, int y, int dir_x, int dir_y) {
    for (int i = 0; i < 4; ++i) {
        if (x + (i * dir_x) < 0 || x + (i * dir_x) >= NUM_COLS ||
            y + (i * dir_y) < 0 || y + (i * dir_y) >= NUM_ROWS) {
            return 0;
        }
        search_buf[i] = board[y + (i * dir_y)][x + (i * dir_x)];
    }

    search_buf[4] = '\0';
    return strcmp(search_buf, "XMAS") == 0 ? 1 : 0;
}

void search_xmas_in_all_directions(int *xmas_counter, char search_buf[5], char** board, int x, int y) {
    if (board[y][x] != 'X') return;
    for (int y_dir = -1; y_dir <= 1; ++y_dir) {
        for (int x_dir = -1; x_dir <= 1; ++x_dir) {
            if (y_dir == 0 && x_dir == 0) continue;
            *xmas_counter += find_xmas_word(search_buf, board, x, y, x_dir, y_dir);
        }
    }
}

int xmas(char** board) {
    int xmas_counter = 0;
    char search_buf[5];

    for (int y = 0; y < NUM_ROWS; ++y)
        for (int x = 0; x < NUM_COLS; ++x)
            search_xmas_in_all_directions(&xmas_counter, search_buf, board, x, y);

    return xmas_counter;
}

int check_mas_pattern(char** board, int x, int y, int pattern[8]) {
    return (board[y+pattern[0]][x+pattern[1]] == 'A' && board[y+pattern[2]][x+pattern[3]] == 'S' && board[y+pattern[4]][x+pattern[5]] == 'M' && board[y+pattern[6]][x+pattern[7]] == 'S');
}

int find_cross_mas(char** board, int y, int x) {
    int mas_counter = 0;

    int mas_patterns[][8] = {
        {-1, -1, -2, -2,  0, -2, -2,  0},
        {-1, -1, -2, -2, -2,  0, -2,  0},
        {-1,  1, -2,  2, -2,  0,  0,  2},
        {-1,  1, -2,  2,  0,  2, -2,  0},
        { 1, -1,  2, -2,  0, -2,  2,  0},
        { 1, -1,  2, -2,  2,  0,  0, -2},
        { 1,  1,  2,  2,  0,  2,  2,  0},
        { 1,  1,  2,  2,  2,  0,  0,  2},
    };

    int row_bounds[] = {-2, -2,  2, 2};
    int col_bounds[] = {-2,  2, -2, 2};

    for (int i = 0; i < 4; ++i) {
        int row_check = y + row_bounds[i];
        int col_check = x + col_bounds[i];

        if (row_check >= 0 && row_check < NUM_ROWS && col_check >= 0 && col_check < NUM_COLS) {
            mas_counter += check_mas_pattern(board, x, y, mas_patterns[i * 2]);
            mas_counter += check_mas_pattern(board, x, y, mas_patterns[i * 2 + 1]);
        }
    }

    return mas_counter;
}

int x_mas(char** board) {
    int mas_counter = 0;

    for (int y = 0; y < NUM_ROWS; ++y) {
        for (int x = 0; x < NUM_COLS; ++x) {
            if (board[y][x] != 'M') continue;

            mas_counter += find_cross_mas(board, y, x);
            board[y][x] = '*';
        }
    }

    return mas_counter;
}

int main(void) {
    INIT_CLOCK();

    char** board = init_board();
    read_into_board(board, WORDSEARCH_PATH);

    printf("xmas ctr: %d\n", xmas(board));
    printf("x_mas ctr: %d\n", x_mas(board));

    free_board(board);

    GET_ELAPSED_TIME();

    return 0;
}
