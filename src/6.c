#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_INPUT "input/6.txt"

#define NUM_COLS 131
#define NUM_ROWS 130

#define MAX_MOVES_ALLOWED 5000

typedef struct vec2 {
    int x, y;
} vec2;

typedef struct guard_t {
    vec2 position;
    vec2 direction; /* normalized direction vector */
} guard_t;

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

void find_guard(guard_t* guard, char** board) {
    for (int y = 0; y < NUM_ROWS; ++y) {
        for (int x = 0; x < NUM_COLS; ++x) {
            if (board[y][x] == '^') {
                guard->position = (vec2){ .x=x, .y=y };
                guard->direction = (vec2){ .x=0, .y=-1 }; /* -1 for up */
                goto found;
            }
        }
    }
found:
    return;
}

void rotate_guard_clockwise(guard_t* guard) {
    int temp = guard->direction.x;
    guard->direction.x = -guard->direction.y;
    guard->direction.y = temp;
}


void simulate_guard(guard_t* guard, char** board) {
    while (1) {
        vec2 next_pos = {
            .x = guard->position.x + guard->direction.x,
            .y = guard->position.y + guard->direction.y
        };

        /* leave the breadcrumbs trail of Xs */
        board[guard->position.y][guard->position.x] = 'X';

        if (next_pos.y >= NUM_ROWS || next_pos.y < 0 ||
            next_pos.x >= NUM_COLS || next_pos.x < 0) break;

        if (board[next_pos.y][next_pos.x] == '#') {
            /* bonk and start over */
            rotate_guard_clockwise(guard); continue;
        }

        guard->position = next_pos;
    }
}

int count_char_in_board(char** board, char target) {
    /* count the breadcrumbs */
    int count = 0;
    for (int y = 0; y < NUM_ROWS; ++y) {
        for (int x = 0; x < NUM_COLS; ++x) {
            if (board[y][x] == target) count++;
        }
    }
    return count;
}

int find_a_loop(guard_t* guard, char** board) {
    int num_inf_loops = 0;
    int turn_counter = 0;

    guard_t original_guard = *guard;

    for (int y = 0; y < NUM_ROWS; ++y) {
        for (int x = 0; x < NUM_COLS; ++x) {
            if (board[y][x] != '.') continue;
            board[y][x] = '#';

            while (1) {
                vec2 next_pos = {
                    .x = guard->position.x + guard->direction.x,
                    .y = guard->position.y + guard->direction.y
                };

                if (next_pos.y >= NUM_ROWS || next_pos.y < 0 ||
                    next_pos.x >= NUM_COLS || next_pos.x < 0) break;

                if (board[next_pos.y][next_pos.x] == '#') {
                    /* bonk and turn */

                    turn_counter++;
                    rotate_guard_clockwise(guard); continue;
                }

                /* i wish i was smart enough to find a better way. */
                if (turn_counter >= MAX_MOVES_ALLOWED) {
                    num_inf_loops++; break;
                }

                guard->position = next_pos;
            }

            board[y][x] = '.';

            *guard = original_guard;
            turn_counter = 0;
        }
    }

    return num_inf_loops;
}

int main(void) {
    char** board = init_board();
    read_into_board(board, MAP_INPUT);
{
    guard_t guard = {0};
    find_guard(&guard, board);
    simulate_guard(&guard, board);
}
    printf("distinct positions: %d\n", count_char_in_board(board, 'X'));

    read_into_board(board, MAP_INPUT);

    guard_t guard = {0};
    find_guard(&guard, board);
    printf("num inf loops: %d\n", find_a_loop(&guard, board));

    free_board(board);

    return 0;
}
