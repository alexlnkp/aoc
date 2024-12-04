#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define WORDSEARCH_PATH "input/4.txt"

#define NUM_ROWS 140
#define NUM_COLS 142

int find_word(char search_buf[5], char** board, int x, int y, int dir_x, int dir_y) {
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

void search_in_all_directions(int *xmas_counter, char search_buf[5], char** board, int x, int y) {
    if (board[y][x] != 'X') return;
    for (int y_dir = -1; y_dir <= 1; ++y_dir) {
        for (int x_dir = -1; x_dir <= 1; ++x_dir) {
            if (y_dir == 0 && x_dir == 0) continue;
            *xmas_counter += find_word(search_buf, board, x, y, x_dir, y_dir);
        }
    }
}

int main(void) {
    INIT_CLOCK();

    FILE* fp = fopen(WORDSEARCH_PATH, "r");

    char** board = malloc(NUM_ROWS * sizeof(char*));
    for (int i = 0; i < NUM_ROWS; ++i)
        board[i] = malloc(NUM_COLS * sizeof(char));

{
    char* buf = NULL;
    size_t len = 0;
    int board_counter = 0;

    while (getline(&buf, &len, fp) != -1 && board_counter < NUM_ROWS) {
        strncpy(board[board_counter], buf, NUM_COLS);
        board[board_counter][NUM_COLS - 1] = '\0';
        board_counter++;
    }
    free(buf);
}
    int xmas_counter = 0;
    char search_buf[5];

    for (int y = 0; y < NUM_ROWS; ++y)
        for (int x = 0; x < NUM_COLS; ++x)
            search_in_all_directions(&xmas_counter, search_buf, board, x, y);

    printf("xmas ctr: %d\n", xmas_counter);

    for (int i = 0; i < NUM_ROWS; ++i) free(board[i]);
    free(board);
    fclose(fp);

    GET_ELAPSED_TIME();

    return 0;
}
