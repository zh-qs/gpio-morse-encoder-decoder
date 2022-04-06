#ifndef BOARDCTL_H
#define BOARDCTL_H

#include <stdio.h>
#include <stdlib.h>

#define STR(s) #s
#define XSTR(s) STR(s)

#define THROW_ON_ERROR(f) ({ int v=(f); if (-1==v) { perror(__FILE__ ":" XSTR(__LINE__)); exit(EXIT_FAILURE); } v; })

//#define WITH_GPIO

#ifdef WITH_GPIO

#include <gpiod.h>

#else

#include <time.h>

struct dummy
{
    int dummy_value;
};
#define gpiod_chip dummy
#define gpiod_line dummy
#define gpiod_line_bulk dummy

#define SW1 (void*)1
#define SW2 (void*)2
#define SW3 (void*)3
#define SW4 (void*)4

/**
 * @brief Event types.
 */
enum {
	GPIOD_LINE_EVENT_RISING_EDGE = 1,
	/**< Rising edge event. */
	GPIOD_LINE_EVENT_FALLING_EDGE,
	/**< Falling edge event. */
};

/**
 * @brief Structure holding event info.
 */
struct gpiod_line_event {
	struct timespec ts;
	/**< Best estimate of time of event occurrence. */
	int event_type;
	/**< Type of the event that occurred. */
	int offset;
	/**< Offset of line on which the event occurred. */
};

#endif

#include <time.h>

#define CHIPNAME "gpiochip0"
#define CONSUMER "morse"

#define LINED1 27
#define LINED2 23
#define LINED3 22
#define LINED4 24

#define LINESW1 18
#define LINESW2 17
#define LINESW3 10
#define LINESW4 25

#define LOW 0
#define HIGH 1

#define SWITCH_PRESSED GPIOD_LINE_EVENT_FALLING_EDGE
#define SWITCH_RELEASED GPIOD_LINE_EVENT_RISING_EDGE

struct board 
{
    struct gpiod_chip *chip;
    struct gpiod_line *diode1_red,
                      *diode2_yellow,
                      *diode3_green,
                      *diode4_blue;
    struct gpiod_line *switch1,
                      *switch2,
                      *switch3,
                      *switch4;
    struct gpiod_line_bulk *switch13,
                           *switch123;

};

void register_board(struct board *b);
void turn_on_diode(struct gpiod_line *diode);
void turn_off_diode(struct gpiod_line *diode);
int is_switch_pressed(struct gpiod_line *sw);
void unregister_board(struct board *b);
int wait_for_switch_then_get_time_pressed(struct gpiod_line *line, struct timespec *time);
void clear_line_buffer(struct gpiod_line *line);
void clear_line_buffer_bulk(struct gpiod_line_bulk *line_bulk);
int wait_for_switches_then_get_line_and_time_pressed(struct gpiod_line_bulk *line_bulk, struct timespec *time, struct gpiod_line **pline, struct timespec *timeout);

#endif
