#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"

#define EQUATIONS_PATH "input/7.txt"

typedef struct equation_t {
    /* i can't believe i wasted 2 hours because this one was an int */
    long expected_res;

    long* parts;
    int num_parts;
} equation_t;

typedef struct equation_list_t {
    equation_t* eqs; /* equations */
    int num_eqs; /* num_equations */
} equation_list_t;

typedef struct stack_frame_t {
    long value;
    int index;
    int num_parts;
} stack_frame_t;

equation_list_t read_equations(const char* path) {
    FILE* fp = fopen(path, "r");

    char* buf = NULL;
    size_t len = 0;
    equation_list_t eql = {0};

    while (getline(&buf, &len, fp) != -1) {
        eql.eqs = realloc(eql.eqs, (eql.num_eqs + 1) * sizeof(equation_t));
        equation_t* current_eq = &eql.eqs[eql.num_eqs];
        current_eq->parts = NULL;
        current_eq->num_parts = 0;

        sscanf(buf, "%ld:", &current_eq->expected_res);

        char* cursor = buf;
        while (*cursor != ':') cursor++;
        cursor++; /* move past colon */

        while (*cursor == ' ') cursor++;

        while (*cursor != '\n' && *cursor != '\0') {
            int part;
            if (sscanf(cursor, "%d", &part) == 1) {
                current_eq->parts = realloc(current_eq->parts, (current_eq->num_parts + 1) * sizeof(long));
                current_eq->parts[current_eq->num_parts] = part;
                current_eq->num_parts++;
            }

            while (*cursor != ' ' && *cursor != '\n' && *cursor != '\0') cursor++;
            while (*cursor == ' ') cursor++;
        }

        eql.num_eqs++;
    }

    free(buf);
    fclose(fp);

    return eql;
}

void free_equation_list(equation_list_t* eql) {
    if (eql == NULL) return;

    for (int i = 0; i < eql->num_eqs; i++)
        free(eql->eqs[i].parts);

    free(eql->eqs);
    eql->eqs = NULL;
    eql->num_eqs = 0;
}

int fits_expected_res(equation_t* const eq) {
    /* 2^(eq->num_parts) is only a theoretical limit.
    * in practice we never even exceed 64. */
    stack_frame_t* stack = malloc((1 << eq->num_parts) * sizeof(stack_frame_t));
    int top = -1;

    stack[++top] = (stack_frame_t){.value = eq->parts[0], .index = 1, .num_parts = eq->num_parts - 1};

    while (top >= 0) {
        stack_frame_t frame = stack[top--];

        if (frame.num_parts == 0) {
            if (frame.value == eq->expected_res) return 1;
            continue;
        }

        long part = eq->parts[frame.index];

        stack[++top] = (stack_frame_t){.value = frame.value + part, .index = frame.index + 1, .num_parts = frame.num_parts - 1};
        stack[++top] = (stack_frame_t){.value = frame.value * part, .index = frame.index + 1, .num_parts = frame.num_parts - 1};
    }

    free(stack);

    return 0;
}

int main(void) {
    INIT_CLOCK();

    equation_list_t eql = read_equations(EQUATIONS_PATH);

    long long total_sum = 0;

    for (int i = 0; i < eql.num_eqs; ++i)
        total_sum += eql.eqs[i].expected_res * fits_expected_res(&eql.eqs[i]);

    printf("total sum: %lld\n", total_sum);

    free_equation_list(&eql);
    GET_ELAPSED_TIME();
    return 0;
}
