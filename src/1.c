#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NUMBER_SET_PATH "input/1.txt"

#define MAX_LINE_LENGTH 32
#define NUM_SETS 1000 /* total number of sets of numbers */

int comp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(void) {
    int* nums1 = calloc(NUM_SETS, sizeof(int));
    int* nums2 = calloc(NUM_SETS, sizeof(int));

    char* line = alloca(MAX_LINE_LENGTH);
    assert(line != NULL);

    FILE* fp = fopen(NUMBER_SET_PATH, "r");
    assert(fp != NULL);

    size_t len;
    int num1, num2;
    int cur_num = 0;
    while (getline(&line, &len, fp) != -1) {
        if (sscanf(line, "%d    %d", &num1, &num2) == 2) {
            nums1[cur_num] = num1;
            nums2[cur_num] = num2;
            cur_num++;
        }
    }

    fclose(fp);

    qsort(nums1, NUM_SETS, sizeof(int), comp);
    qsort(nums2, NUM_SETS, sizeof(int), comp);

    int diff_sum = 0;
    for (int i = 0; i < NUM_SETS; ++i) {
        diff_sum += abs(nums1[i] - nums2[i]);
    }

    printf("diffsum: %d\n", diff_sum);

    free(nums1);
    free(nums2);
    return 0;
}
