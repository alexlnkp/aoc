#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define REPORTS_PATH "input/2.txt"

#define NUM_REPORTS 1000

typedef struct report_t {
    int* levels;
    int num_levels;
} report_t;

int count_characters(const char *str, char character) {
    const char *p = str;
    int count = 0;

    do { if (*p == character) count++; } while (*(p++));

    return count;
}

int is_increasing(int *arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] >= arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int is_decreasing(int *arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] <= arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    INIT_CLOCK();

    int num_reports = 0;
    report_t* reports = calloc(NUM_REPORTS, sizeof(report_t));

    FILE* fp = fopen(REPORTS_PATH, "r");

    char* line = alloca(64);
    size_t len;

    while (getline(&line, &len, fp) != EOF) {
        report_t rep = {0};

        /* in a set of numbers separated by spaces,
         * the total amount of numbers is the same
         * as the total amount of spaces + 1 */
        rep.num_levels = count_characters(line, ' ') + 1;
        rep.levels = calloc(rep.num_levels, sizeof(int));

        char *token = strtok(line, " ");
        int size = 0;

        while (token != NULL) {
            rep.levels[size++] = atoi(token);
            token = strtok(NULL, " ");
        }

        reports[num_reports] = rep;
        num_reports++;
    }

    int num_safe_reports = 0;

    for (int i = 0; i < NUM_REPORTS; ++i) {
        int score = 0;

        for (int j = 0; j < reports[i].num_levels - 1; ++j) {
            unsigned diff = abs(reports[i].levels[j + 1] - reports[i].levels[j]);
            score += (diff <= 3 && diff >= 1);
        }

        /* if the numbers in levels are ONLY increasing or ONLY decreasing,
         * and every difference for every pair of numbers is correct per
         * level, then it's a safe report. */
        if (is_decreasing(reports[i].levels, reports[i].num_levels) ||
            is_increasing(reports[i].levels, reports[i].num_levels)) {
            num_safe_reports += (score == reports[i].num_levels - 1);
        }

    }

    printf("num safe reports: %d\n", num_safe_reports);

    fclose(fp);

    GET_ELAPSED_TIME();
    return 0;
}
