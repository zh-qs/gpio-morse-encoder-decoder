#include "morse.h"
#include "boardctl.h"
#include <stdio.h>
#include <stdlib.h>

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
    send_string(b, str);
}

void decode_mode(struct board *b)
{
    printf("Wybierz tryb wpisywania kodu Morse'a:\n");
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