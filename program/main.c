#include "boardctl.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    struct board b;
    register_board(&b);
    
    turn_on_diode(b.diode1_red);
    while (1)
    {
        if (is_switch_pressed(b.switch1))
            break;
        usleep(10000);
    }
    turn_off_diode(b.diode1_red);

    unregister_board(&b);
    return 0;
}