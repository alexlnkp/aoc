#include <stdio.h>
#include <stdlib.h>

#define DISKMAP_INPUT "input/9.txt"

#define EMPTY_SPACE -1 /* equivalent of '.' */

#define BUFFER_SIZE_MARGIN 1024

char* read_diskmap(const char* path, long* n_chars) {
    FILE* fp = fopen(path, "r");

    fseek(fp, 0, SEEK_END);
    *n_chars = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* full = calloc(*n_chars, sizeof(char));
    fread(full, sizeof(char), *n_chars, fp);
    fclose(fp);

    return full;
}

void append_val_n_times(long *res, long *res_written, long val, int n) {
    for (int j = 0; j < n; j++) res[(*res_written)++] = val;
}

long* parse_diskmap(char* dm, long n_chars, long *res_written) {
    long res_size = BUFFER_SIZE_MARGIN;
    long* res = malloc(res_size * sizeof(long));
    *res_written = 0;

    for (long i = 0; i < n_chars - 1; ++i) {
        int n = dm[i] - '0';

        if ((*res_written + n) >= res_size) {
            res_size += BUFFER_SIZE_MARGIN;
            res = realloc(res, res_size * sizeof(long));
        }

        if ((i + 1) % 2 == 0) {
            append_val_n_times(res, res_written, EMPTY_SPACE, n);
        } else {
            append_val_n_times(res, res_written, (i / 2), n);
        }
    }

    return res;
}

void reorder_diskmap(long* pdm, long n_ids) {
    for (long i = 0; i < n_ids; ++i) {
        if (pdm[i] != -1) continue;
        /* found an empty spot. rearrange
            * everything from end to beginning */

        for (long j = n_ids - 1; j > i; --j) {
            if (pdm[j] != -1) {
                /* "move" number from end to us */
                pdm[i] = pdm[j];
                pdm[j] = -1;
                break;
            }
        }
    }
}

long calculate_checksum(long* pdm, long n_ids) {
    long chsum = 0;

    for (long i = 0; i < n_ids; ++i) {
        if (pdm[i] > 0) chsum += pdm[i] * i;
    }

    return chsum;
}

int main(void) {
    long n_chars = 0;
    char* dm = read_diskmap(DISKMAP_INPUT, &n_chars);

    long n_ids = 0;
    long* pdm = parse_diskmap(dm, n_chars, &n_ids);
    reorder_diskmap(pdm, n_ids);

    printf("checksum: %ld\n", calculate_checksum(pdm, n_ids));

    free(dm);
    free(pdm);

    return 0;
}
