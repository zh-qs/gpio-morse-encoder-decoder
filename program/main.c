#include "morse.h"
#include "boardctl.h"
#include <stdio.h>

#define STR(s) #s
#define XSTR(s) STR(s)

#define MAX_STRING 1000
#define SCANF_PATTERN "%" XSTR(MAX_STRING) "d"

enum mode
{
    ENCODE = 1, DECODE, EXIT
};

enum mode get_mode()
{
    int m = 0;
    while (m < 1 || m > 3)
    {
        printf("Wybierz tryb:\n 1. Nadawanie\n 2. Odbieranie\n 3. Wyjscie\n");
        scanf("%d",&m);
    }
    return m;
}

void encode_mode(struct board *b)
{
    char str[MAX_STRING];
    printf("Wpisz tutaj ciag znakow, ktory ma byc zakodowany, i nacisnij ENTER:\n");
    scanf(SCANF_PATTERN, str);
    printf("Nadawanie...");
    fflush(stdout);
    send_string(b, str);
    printf("OK\n");
}

void decode_mode(struct board *b)
{
    int m=0;
    while (m < 1 || m > 2)
    {
        printf("Wybierz tryb dzielenia znaków i wyrazów w kodzie Morse'a:\n 1. 3- i 7-kropkowa przerwa\n 2. 1 i 2 nacisniecia SW2\n");
        scanf("%d", &m);
    }
    struct morse_getchar_options options;
    options.method = m;
    printf("Teraz skalibrujemy tempo nadawania pojedynczej kropki.\n");
    calibrate(b, &(options.dot_time));
    printf("Nadawanie kodu: SW1, zakonczenie nadawania: SW3\n\n");
    enum read_status status = NEW_WORD;
    while (status != STOP)
    {
        printf("%c", morse_getchar(b, &status, &options));
        fflush(stdout);
    }
    printf("\nZakonczono odbieranie kodu.\n");
        
}

int main() 
{
    struct board b;
    register_board(&b);
    
    while (true)
    {
        enum mode m = get_mode();
        if (m == EXIT) break;

        if (m == ENCODE) encode_mode(&b);
        else decode_mode(&b);
    }

    unregister_board(&b);
    return 0;
}
