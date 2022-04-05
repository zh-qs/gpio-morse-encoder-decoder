#ifndef TIME_OPERATIONS_H
#define TIME_OPERATIONS_H

#include <time.h>

#define TRUE 1
#define FALSE 0

// res = t1 - t2
void diff_timespec(struct timespec *t1, struct timespec *t2, struct timespec *res);
// res = t1 + t2
void add_timespec(struct timespec *t1, struct timespec *t2, struct timespec *res);
// avg = ts/n
void add_to_average(struct timespec *avg, struct timespec *ts, int n);
// res = ts * n
void multiply_time(long long n, struct timespec *ts, struct timespec *res);
// t1 > t2
int time_greater(struct timespec *t1, struct timespec *t2);

#endif