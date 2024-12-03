#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define MEMORY_PATH "input/3.txt"

int prefix(const char *str, const char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

int parse_mul_arguments(int *num1, int *num2, char* cursor) {
    /* 16 is a bit overkill, considering we should only
     * ever have 3-digit numbers, but if we only had 4,
     * then we might not have been able to see precisely
     * how long our number was, as it might've cut off */
    char* miscbuf = alloca(16 * sizeof(char));

    sscanf(cursor, "%d", num1);
    cursor += snprintf(miscbuf, 16, "%d", *num1);
    if (*cursor++ != ',') return 0; /* wrong format */

    sscanf(cursor, "%d", num2);
    cursor += snprintf(miscbuf, 16, "%d", *num2);
    if (*cursor != ')') return 0; /* wrong format */

    return 1;
}

void part1(int *mult_sum, char* buf, size_t len, FILE* fp) {
    while (fgets(buf, len, fp)) {
        char* cursor = buf;

        while ((cursor = strstr(cursor, "mul(")) != NULL) {
            int num1 = 0, num2 = 0;
            cursor += 4; /* move past `mul(` */

            if (!parse_mul_arguments(&num1, &num2, cursor)) continue;
            *mult_sum += num1 * num2;

            cursor++;
        }
    }
}

void part2(int *mult_sum, char* buf, size_t len, FILE* fp) {
    int mul_enabled = 1;

    while (fgets(buf, len, fp)) {
        char* cursor = buf;

        while ((cursor = strstr(cursor, "mul(")) != NULL) {
            int num1 = 0, num2 = 0;
            cursor += 4; /* move past `mul(` */

            char* tracker = cursor;
            while (tracker != buf) {
                if (prefix(tracker, "don't()")) { mul_enabled = 0; break; }
                else if (prefix(tracker, "do()")) { mul_enabled = 1; break; }
                tracker--;
            }

            if (!parse_mul_arguments(&num1, &num2, cursor)) continue;
            if (mul_enabled) *mult_sum += num1 * num2;

            cursor++;
        }
    }
}

int main(void) {
    INIT_CLOCK();

    FILE* fp = fopen(MEMORY_PATH, "r");

    /* had to allocate a LOT of memory unfortunately
     * with a small buffer the input gets truncated
     * and yields wrong results. there might be a
     * better solution than this, but it's certainly
     * more complicated. */
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int mult_sum = 0;

    char* buf = malloc((len + 1) * sizeof(char));

    part1(&mult_sum, buf, len + 1, fp);
    printf("mult sum: %d\n", mult_sum);

    mult_sum = 0;
    fseek(fp, 0, SEEK_SET);

    part2(&mult_sum, buf, len  + 1, fp);
    printf("mult sum2: %d\n", mult_sum);

    fclose(fp);

    free(buf);

    GET_ELAPSED_TIME();

    return 0;
}
