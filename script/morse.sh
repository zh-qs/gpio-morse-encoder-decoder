#!/bin/sh

CODE_A=.-
CODE_B=-...
CODE_C=-.-.
CODE_D=-..
CODE_E=.
CODE_F=..-.
CODE_G=--.
CODE_H=....
CODE_I=..
CODE_J=.---
CODE_K=-.-
CODE_L=.-..
CODE_M=--
CODE_N=-.
CODE_O=---
CODE_P=.--
CODE_Q=--.-
CODE_R=.-.
CODE_S=...
CODE_T=-
CODE_U=..-
CODE_V=...-
CODE_W=.--
CODE_X=-..-
CODE_Y=-.--
CODE_Z=--..

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

registerboard () {
    for i in $DIODES $SWITCHES
        do
            echo $i > /sys/class/gpio/export
        done
    
    for i in $DIODES
        do
            echo low > /sys/class/gpio/gpio$i/direction
        done
    
    for i in $SWITCHES
        do
            echo in > /sys/class/gpio/gpio$i/direction
            echo both > /sys/class/gpio/gpio$i/edge
        done
}

encode () {
    echo Wpisz ciag znakow, ktory ma byc zakodowany, i nacisnij ENTER:
    read -p "" str

    echo 1 > /sys/class/gpio/gpio$DIODE1/value
    for (( i=0; i<${#str}; i++ )); do
        if [ ${str:$i:1} = " " ] then
            sleep .3
        else
            CODE=CODE_${str:$i:1}
            for (( j=0; j<${#CODE}; j++ )) do
                echo 1 > /sys/class/gpio/gpio$DIODE4/value
                if [ ${CODE:$j:1} = "." ] then
                    sleep .3
                elif [ ${CODE:$j:1} = "-" ] then
                    sleep .9
                fi
                echo 0 > /sys/class/gpio/gpio$DIODE4/value
                sleep .3
            done
        fi
        sleep .9
    done
    echo 0 > /sys/class/gpio/gpio$DIODE1/value
}

calibrate () {

}

decode () {
    calibrate
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
    fi
    mainmenu
}

mainmenu