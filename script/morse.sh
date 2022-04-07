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

CODE_a=.-
CODE_b=-...
CODE_c=-.-.
CODE_d=-..
CODE_e=.
CODE_f=..-.
CODE_g=--.
CODE_h=....
CODE_i=..
CODE_j=.---
CODE_k=-.-
CODE_l=.-..
CODE_m=--
CODE_n=-.
CODE_o=---
CODE_p=.--
CODE_q=--.-
CODE_r=.-.
CODE_s=...
CODE_t=-
CODE_u=..-
CODE_v=...-
CODE_w=.--
CODE_x=-..-
CODE_y=-.--
CODE_z=--..

N_CALIBRATION=10

# GPIO number = XXX (from /sys/class/gpio/gpiochipXXX) + number of pin 

GPIONUM=`ls /sys/class/gpio | grep gpiochip | tr -d gpiochip`

DIODE1=$(( GPIONUM + 27 ))
DIODE2=$(( GPIONUM + 23 ))
DIODE3=$(( GPIONUM + 22 ))
DIODE4=$(( GPIONUM + 24 ))

DIODES="$DIODE1 $DIODE2 $DIODE3 $DIODE4"

SW1=$(( GPIONUM + 18 ))
SW2=$(( GPIONUM + 17 ))
SW3=$(( GPIONUM + 10 ))
SW4=$(( GPIONUM + 25 ))

SWITCHES="$SW1 $SW2 $SW3 $SW4"

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

	echo Nadawanie...
    echo 1 > /sys/class/gpio/gpio$DIODE1/value
    i=0
    while [ "$i" -le "${#str}" ]; do
        if [ "${str:$i:1}" = " " ]; then
            sleep .3
        else
            eval "CODE=\${CODE_${str:$i:1}}"
            j=0
            while [ "$j" -le "${#CODE}" ]; do
                echo 1 > /sys/class/gpio/gpio$DIODE4/value
                if [ "${CODE:$j:1}" = "." ]; then
                    sleep .3
                elif [ "${CODE:$j:1}" = "-" ]; then
                    sleep .9
                fi
                echo 0 > /sys/class/gpio/gpio$DIODE4/value
                sleep .3
				j=$(( j + 1 ))
            done
        fi
        sleep .9
        i=$(( i + 1 ))
    done
    echo 0 > /sys/class/gpio/gpio$DIODE1/value
	echo OK
}

#waitforswitch_andgettime () {
#    
#}

calibrate () {
    echo Teraz skalibrujemy tempo nadawania pojedynczej kropki.
    echo Wcisnij przycisk SW1 $N_CALIBRATION razy, aby ustalic czas trwania pojedynczej kropki:
    # TODO
}

decode () {
	echo Na razie nie dziala!
    # calibrate
    # TODO
}

unregisterboard () {
    for i in $DIODES $SWITCHES
        do
            echo $i > /sys/class/gpio/unexport
        done
}

mainmenu () {

    echo Wybierz tryb:
    echo  1. Nadawanie
    echo  2. Odbieranie
    echo  3. Wyjscie
    read -p "" modeselect
    if [ "$modeselect" = "1" ]; then
        encode
    elif [ "$modeselect" = "2" ]; then
        decode
    elif [ "$modeselect" = "3" ]; then
        unregisterboard
        exit
    fi
    mainmenu
}

registerboard
mainmenu
