#ifndef MORSE_H
#define MORSE_H

#include "boardctl.h"
#include "time_operations.h"
#include "morse_search_tree.h"

#define DOT_TIME_US 300000
#define N_CALIBRATION 10

enum read_status
{
    NEW_WORD,
    NEW_CHAR,
    STOP
};

enum recognition_method 
{
    DELAY = 1, // new char -> 3 dots delay, new word -> 7 dots delay
    SECOND_SWITCH // new char -> 1, new word -> 2 SW2 clicks
};

struct morse_getchar_options
{
    struct timespec dot_time;
    enum recognition_method method;
};

void send_string(struct board *b, const char *str);
void send_char(struct board *b, char c);
char morse_getchar(struct board *b, enum read_status *status, struct morse_getchar_options *options);
void calibrate(struct board *b, struct timespec *dot_time);

#endif
