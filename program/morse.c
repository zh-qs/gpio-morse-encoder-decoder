#include "morse.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>

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

int get_morse_table_index(char c)
{
    if (c >= ' ' && c <= 'Z') return c-' ';
    if (c == '_') return 59;
    if (c >= 'a' && c <= 'z') return 33+c-'a';

    return 0;
}

void send_morse_code(struct board *b, const char *code)
{
    int i=0;
    while (code[i]!='\0')
    {
        if (code[i]==' ') usleep(DOT_TIME_US);
        else
        {
            turn_on_diode(b->diode4_blue);
#ifndef WITH_GPIO
            printf("%c",code[i]);
            fflush(stdout);
#endif
            if (code[i]=='.') usleep(DOT_TIME_US);
            else if (code[i]=='-') usleep(3*DOT_TIME_US);
            turn_off_diode(b->diode4_blue);
            usleep(DOT_TIME_US);
        }
        ++i;
    }
#ifndef WITH_GPIO
    printf(" ");
    fflush(stdout);
#endif
    usleep(3*DOT_TIME_US);
}

void send_char(struct board *b, char c)
{
    send_morse_code(b, morse_codes[get_morse_table_index(c)]);
} 

void send_string(struct board *b, const char *str)
{
    int i=0;
    turn_on_diode(b->diode1_red);
    while (str[i]!='\0')
    {
        send_char(b, get_morse_table_index(str[i++]));
    }
    turn_off_diode(b->diode1_red);
}



// {dot_time (nie NULL)/NULL}
// w trybie nowe słowo czekamy bez końca na SW1 lub STOP
// w trybie nowy znak czekamy 4 kropki/bez końca na SW1 lub STOP, jeśli nie/SW2, to nowe słowo
// po wyklikaniu kodu czekamy 3 kropki/na SW2 i wtedy zwracamy znaleziony znak
char morse_getchar_base(struct board *b, enum read_status *status, struct timespec *dot_time, struct gpiod_line_bulk *switches, int is_delay)
{
    struct timespec three_dot_time, four_dot_time, switch_time;
    struct timespec *pthree_dot_time, *pfour_dot_time;
    if (is_delay)
    {   
        multiply_time(3, dot_time, pthree_dot_time = &three_dot_time);
        multiply_time(4, dot_time, pfour_dot_time = &four_dot_time);
    }
    else
    {
        pthree_dot_time = pfour_dot_time = NULL;
        multiply_time(3, dot_time, &three_dot_time);
    }
    struct gpiod_line *line;
    struct morse_search_tree *node = get_morse_tree();
    int res;
    switch (*status)
    {
        case NEW_CHAR:
            turn_on_diode(b->diode3_green);
            // czekaj 4 kropki na SW1 lub STOP
            res = wait_for_switches_then_get_line_and_time_pressed(switches, &switch_time, &line, pfour_dot_time);
            if (res == 0 || line == b->switch2) // timeout or SW2
            {
                *status = NEW_WORD;
                return ' ';
            }
            // tu trzeba przejść za pierwsze czekanie w NEW WORD
            turn_off_diode(b->diode3_green);
            break;
        case NEW_WORD:
            turn_on_diode(b->diode3_green);
            turn_on_diode(b->diode2_yellow);
            // czekaj bez końca na SW1 lub STOP
            res = wait_for_switches_then_get_line_and_time_pressed(switches, &switch_time, &line, NULL);
            turn_off_diode(b->diode3_green);
            turn_off_diode(b->diode2_yellow);
            break;
        default:
            return '\0';
    }

    while (TRUE)
    {
        if (res == 0 || line == b->switch2) // timeout or SW2
        {
            *status = NEW_CHAR;
            return node->found_char;
        }
        if (line == b->switch3) // STOP
        {
            *status = STOP;
            return node->found_char;
        }
        if (line == b->switch1)
        {
            if (time_greater(&switch_time, &three_dot_time)) // dash
            {
                node = node->dash;
            }
            else // dot
            {
                node = node->dot;
            }
        }

        // czekaj 3 kropki na SW1 lub STOP
        res = wait_for_switches_then_get_line_and_time_pressed(switches, &switch_time, &line, pthree_dot_time);
    }
}

// w trybie nowe słowo czekamy bez końca na SW1 lub STOP
// w trybie nowy znak czekamy 4 kropki na SW1 lub STOP, jeśli nie, to nowe słowo
// po wyklikaniu kodu czekamy 3 kropki i wtedy zwracamy znaleziony znak
char morse_getchar_delay(struct board *b, enum read_status *status, struct timespec *dot_time)
{
    return morse_getchar_base(b, status, dot_time, b->switch13, TRUE);
}

// w trybie nowe słowo czekamy bez końca na SW1 lub STOP
// w trybie nowy znak czekamy na SW1 lub STOP, jeśli SW2, to nowe słowo
// po wyklikaniu kodu czekamy na SW2 i wtedy zwracamy znaleziony znak
char morse_getchar_second_switch(struct board *b, enum read_status *status, struct timespec *dot_time)
{
    return morse_getchar_base(b, status, dot_time, b->switch123, FALSE);
}

char morse_getchar(struct board *b, enum read_status *status, struct morse_getchar_options *options)
{
    if (options->method == DELAY) return morse_getchar_delay(b, status, &(options->dot_time));
    else return morse_getchar_second_switch(b, status, &(options->dot_time));
}


void calibrate(struct board *b, struct timespec *dot_time)
{
    dot_time->tv_nsec = 0;
    dot_time->tv_sec = 0;
    printf("Wcisnij przycisk SW1 %d razy, aby ustalic czas trwania pojedynczej kropki:\n", N_CALIBRATION);
    struct timespec ts;
    //clear_line_buffer(b->switch1);
    for (int i=0;i<N_CALIBRATION;)
    {
        if (wait_for_switch_then_get_time_pressed(b->switch1, &ts) == 1)
        {
            add_to_average(dot_time,&ts,N_CALIBRATION);
            printf("%d...\n",++i);
        }
    }
    printf("Skalibrowano!\n");
}
