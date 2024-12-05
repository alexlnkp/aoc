#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define PAGES_PATH "input/5.txt"

#define NUM_COLS 80
#define NUM_ROWS 1400

#define MAX_UPDATES_PPAGE 24 /* could vary from input to input */

#define EOP -1 /* end of pages */

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

int read_into_board(char** board, const char* path) {
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

    return board_counter;
}

void parse_board(int *num_rules, int *num_pages, char** board, int num_lines, int** rules, int** pages) {
    int part = 0;
    int rule_count = 0;
    int pages_count = 0;

    for (int i = 0; i < num_lines; ++i) {
        if (strlen(board[i]) == 1) { part = 1; continue; }
        if (!part) {
            char *firstPart  = strtok(board[i], "|");
            char *secondPart = strtok(NULL, "\n");
            if (firstPart && secondPart) {
                rules[rule_count][0] = atoi(firstPart);
                rules[rule_count][1] = atoi(secondPart);
                rule_count++;
            }
        } else {
            int column = 0;
            for (char *token = strtok(board[i], ","); token != NULL; token = strtok(NULL, ","))
                pages[pages_count][column++] = atoi(token);

            pages[pages_count][column] = EOP;
            pages_count++;
        }
    }

    *num_rules = rule_count;
    *num_pages = pages_count;
}

int find_num_idx(int number, int *page) {
    int idx = 0;
    while(1) {
        if (page[idx] == -1 && number != -1) return -1;
        if (page[idx] == number) return idx;
        idx++;
    }
}

int sort(int** rules, int* page, int num_rules) {
    int n = find_num_idx(-1, page);
    if (n == -1) return 1; /* consider sorted */

    int is_already_sorted = 1;
    for (int i = 0; i < n - 1; ++i) {
        int is_swapped = 0;
        for (int j = 0; j < n - i - 1; ++j) {
            int need_to_swap = 0;

            for (int k = 0; k < num_rules; k++) {
                if (rules[k][0] == page[j] && rules[k][1] == page[j + 1]) break;
                if (rules[k][1] == page[j] && rules[k][0] == page[j + 1]) { need_to_swap = 1; break; }
            }

            if (need_to_swap) {
                is_already_sorted = 0;
                int temp = page[j];
                page[j] = page[j + 1];
                page[j + 1] = temp;
                is_swapped = 1;
            }
        }

        if (!is_swapped) break;
    }

    return is_already_sorted;
}

void parse_pages(int** pages, int** rules, int num_rules, int num_pages) {
    int sum1 = 0, sum2 = 0;

    for (int i = 0; i < num_pages; i++) {
        int mid_idx = find_num_idx(-1, pages[i]) / 2;
        if (sort(rules, pages[i], num_rules))
                sum1 += pages[i][mid_idx]; /* part1 */
        else    sum2 += pages[i][mid_idx]; /* part2 */

    }
    printf("  sorted sum:\t%d\nunsorted sum:\t%d\n", sum1, sum2);
}

int main(void) {
    char** board = init_board();
    int num_lines = read_into_board(board, PAGES_PATH);

    int** rules = malloc(num_lines * sizeof(int*));
    for (int i = 0; i < num_lines; ++i)
        rules[i] = malloc(2 * sizeof(int)); /* X|Y */

    int** pages = malloc(num_lines * sizeof(int*));
    for (int i = 0; i < num_lines; ++i)
        pages[i] = malloc(MAX_UPDATES_PPAGE * sizeof(int)); /* X,Y,Z,W,U,V,... */

    INIT_CLOCK();

    int num_rules = 0;
    int num_pages = 0;

    parse_board(&num_rules, &num_pages, board, num_lines, rules, pages);
    parse_pages(pages, rules, num_rules, num_pages);

    GET_ELAPSED_TIME();

    free_board(board);

    for (int i = 0; i < num_lines; ++i) {
        free(rules[i]); free(pages[i]);
    }

    free(rules); free(pages);

    return 0;
}
