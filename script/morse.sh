#!/bin/sh

DIODE1=27
DIODE2=23
DIODE3=22
DIODE4=24

DIODES=$DIODE1 $DIODE2 $DIODE3 $DIODE4

SW1=18
SW2=17
SW3=10
SW4=25

SWITCHES=$SW1 $SW2 $SW3 $SW4

encode () {
    echo Wpisz ciag znakow, ktory ma byc zakodowany, i nacisnij ENTER:
    read -p "" str

}

registerboard () {
    for i in $DIODES $SWITCHES
        do
            echo $i > /sys/class/gpio/export
        done
    
    for i in $DIODES
        do
            echo low > /sys/class/gpio$i/direction
        done
    
    for i in $SWITCHES
        do
            echo in > /sys/class/gpio$i/direction
            echo both > /sys/class/gpio$i/edge
        done
}

unregisterboard () {
    for i in $DIODES $SWITCHES
        do
            echo i > /sys/class/gpio/unexport
        done
}

mainmenu () {

    echo Wybierz tryb:
    echo  1. Nadawanie
    echo  2. Odbieranie
    echo  3. Wyjscie
    read -n 1 -p "" modeselect
    if [ "$modeselect" = "1" ]; then
        encode
    elif [ "$modeselect" = "2" ]; then
        decode
    elif [ "$modeselect" = "3" ]; then
        exit
    else
        mainmenu
    fi

}

mainmenu