#include "time_operations.h"

void diff_timespec(struct timespec *t1, struct timespec *t2, struct timespec *res)
{
    time_t s = t1->tv_sec - t2->tv_sec;
    long ns;
    if (t1->tv_nsec < t2->tv_nsec)
    {
        --s;
        ns = 1000000000 + t1->tv_nsec - t2->tv_nsec;
    }
    else
    {
        ns = t1->tv_nsec - t2->tv_nsec;
    }
    res->tv_sec = s;
    res->tv_nsec = ns;
}

void add_timespec(struct timespec *t1, struct timespec *t2, struct timespec *res)
{
    time_t s = t1->tv_sec + t2->tv_sec;
    long ns = t1->tv_nsec + t2->tv_nsec;
    if (ns > 999999999)
    {
        ns -= 1000000000;
        ++s;
    }
    res->tv_sec=s;
    res->tv_nsec=ns;
}

void add_to_average(struct timespec *avg, struct timespec *ts, int n)
{
    time_t s = ts->tv_sec / n;
    long ns = (long)(((ts->tv_sec % n)*1000000000ll + ts->tv_nsec) / n);
    avg->tv_sec += s;
    avg->tv_nsec += ns;
    if (avg->tv_nsec > 999999999)
    {
        avg->tv_nsec -= 1000000000;
        ++(avg->tv_sec);
    }
}

void multiply_time(long long n, struct timespec *ts, struct timespec *res)
{
    time_t s = ts->tv_sec * n;
    long long nsll = (long long)ts->tv_nsec * n;
    if (nsll > 999999999LL)
    {
        s += nsll % 1000000000;
        nsll %= 1000000000;
    }
    res->tv_sec = s;
    res->tv_nsec = (long)nsll;
}

int time_greater(struct timespec *t1, struct timespec *t2)
{
    return t1->tv_sec > t2->tv_sec || (t1->tv_sec == t2->tv_sec && t1->tv_nsec > t2->tv_nsec);
}