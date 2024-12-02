#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

int is_safe_report(report_t* report) {
    int score = 0;

    for (int j = 0; j < report->num_levels - 1; ++j) {
        unsigned diff = abs(report->levels[j + 1] - report->levels[j]);
        score += (diff <= 3 && diff >= 1);
    }

    /* if the numbers in levels are ONLY increasing or ONLY decreasing,
     * and every difference for every pair of numbers is correct per
     * level, then it's a safe report. */
    return (score == report->num_levels - 1) &&
           (is_decreasing(report->levels, report->num_levels) ||
            is_increasing(report->levels, report->num_levels));
}

int get_num_safe_reports(report_t* reports) {
    int num_safe_reports = 0;

    for (int i = 0; i < NUM_REPORTS; ++i) {
        if (is_safe_report(&reports[i])) num_safe_reports++;
    }

    return num_safe_reports;
}

int get_num_safe_reports_damp(report_t* reports) {
    int num_safe_reports = 0;

    report_t modified_report;
    /* i'm not proud of this solution, but it's better than having
     * alloca, malloc or calloc within a loop itself. the 16 is
     * just a magic number, and it should always be the most amount
     * of elements a `levels` array can hold, or more. my reports
     * had at most 8 of numbers per levels array, but it may vary */
    modified_report.levels = alloca(16 * sizeof(int));

    for (int i = 0; i < NUM_REPORTS; ++i) {
        modified_report.num_levels = reports[i].num_levels - 1;

        for (int j = -1; j < reports[i].num_levels; ++j) {
            for (int k = 0, l = 0; k < reports[i].num_levels; ++k) {
                /* if `j` (the ignore index) is -1, `k` will never be equal
                 * to it, which effectively just copies the array. */
                if (k == j) continue;
                modified_report.levels[l++] = reports[i].levels[k];
            }

            if (is_safe_report(&modified_report)) {
                num_safe_reports++; break;
            }
        }
    }

    return num_safe_reports;
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

    printf("num safe reports (no dampening): %d\n", get_num_safe_reports(reports));
    printf("num safe reports (dampening): %d\n", get_num_safe_reports_damp(reports));

    fclose(fp);

    GET_ELAPSED_TIME();
    return 0;
}
