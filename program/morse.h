#ifndef MORSE_H
#define MORSE_H

#include "boardctl.h"
#include "time_operations.h"
#include "morse_search_tree.h"

#define DOT_TIME_US 500000
#define N_CALIBRATION 10

enum read_status
{
    NEW_WORD,
    NEW_CHAR,
    STOP
}

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
char morse_getchar(struct board *b, struct morse_getchar_options *options, enum read_status *status);
void calibrate(struct board *b, struct timespec *dot_time);

const char *morse_codes[] = 
{
    "", // (no code)  

    " ", // (space)
    "-.-.--", //!
    ".-..-.", //"
    "", //#
    "", //$
    "", //%
    "", //&
    ".----.", //'
    "-.--.", //(
    "-.--.-", //)
    "", //*
    ".-.-.", //+
    "--..--", //,
    "-....-", //-
    ".-.-.-", //. 
    "-..-.", // /

    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9

    "---...", //:
    "-.-.-.", //;
    "", //<
    "-...-", //=
    "", //>
    "..--..", //?
    ".--.-." //@

    ".-", //A
    "-...", //B
    "-.-.", //C
    "-..", //D
    ".", //E
    "..-.", //F
    "--.", //G
    "....", //H
    "..", //I
    ".---", //J
    "-.-", //K
    ".-..", //L
    "--", //M
    "-.", //N
    "---", //O
    ".--.", //P
    "--.-", //Q
    ".-.", //R
    "...", //S
    "-", //T
    "..-", //U
    "...-", //V
    ".--", //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z

    "..--.-", //_
    
};



#endif
