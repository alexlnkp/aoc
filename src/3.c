#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define MEMORY_PATH "input/3.txt"

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
    char* miscbuf = malloc(16 * sizeof(char));

    while (fgets(buf, len + 1, fp)) {
        char* cursor = buf;

        while ((cursor = strstr(cursor, "mul(")) != NULL) {
            int num1 = 0, num2 = 0;
            cursor += 4; /* move past `mul(` */

            sscanf(cursor, "%d", &num1);
            cursor += snprintf(miscbuf, 16, "%d", num1);
            if (*cursor++ != ',') continue; /* wrong format */

            sscanf(cursor, "%d", &num2);
            cursor += snprintf(miscbuf, 16, "%d", num2);
            if (*cursor != ')') continue; /* wrong format */

            mult_sum += num1 * num2;

            cursor++;
        }
    }

    printf("mult sum: %d\n", mult_sum);

    fclose(fp);

    free(buf);
    free(miscbuf);

    GET_ELAPSED_TIME();

    return 0;
}
