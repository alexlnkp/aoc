#ifndef   __BENCHMARK_H__
#define   __BENCHMARK_H__

#ifdef    BENCHMARK
  #include <time.h>
  #define INIT_CLOCK() clock_t start_time = clock()
  #define GET_ELAPSED_TIME() \
    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC; \
    printf("Done in %f seconds\n", elapsed_time)
#else
  #define INIT_CLOCK() (void)0
  #define GET_ELAPSED_TIME() (void)0
#endif /* BENCHMARK */

#endif /* __BENCHMARK_H__ */
