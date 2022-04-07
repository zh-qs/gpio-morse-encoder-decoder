#include "boardctl.h"
#include "time_operations.h"

#ifndef WITH_GPIO
#include <stdio.h>
#endif

void register_board(struct board *b)
{
#ifdef WITH_GPIO
    b->chip = THROW_ON_NULL(gpiod_chip_open_by_name(CHIPNAME));

    b->diode1_red = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINED1));
    b->diode2_yellow = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINED2));
    b->diode3_green = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINED3));
    b->diode4_blue = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINED4));

    THROW_ON_ERROR(gpiod_line_request_output(b->diode1_red, CONSUMER, 0));
    THROW_ON_ERROR(gpiod_line_request_output(b->diode2_yellow, CONSUMER, 0));
    THROW_ON_ERROR(gpiod_line_request_output(b->diode3_green, CONSUMER, 0));
    THROW_ON_ERROR(gpiod_line_request_output(b->diode4_blue, CONSUMER, 0));

    b->switch1 = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINESW1));
    b->switch2 = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINESW2));
    b->switch3 = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINESW3));
    b->switch4 = THROW_ON_NULL(gpiod_chip_get_line(b->chip, LINESW4));

    // czy musimy requestować input, zanim requestujemy eventy?
    //THROW_ON_ERROR(gpiod_line_request_input(b->switch1, CONSUMER));
    //THROW_ON_ERROR(gpiod_line_request_input(b->switch2, CONSUMER));
    //THROW_ON_ERROR(gpiod_line_request_input(b->switch3, CONSUMER));
    // gpiod_line_request_input(b->switch4, CONSUMER);
    THROW_ON_ERROR(gpiod_line_request_both_edges_events(b->switch1, CONSUMER));
    THROW_ON_ERROR(gpiod_line_request_both_edges_events(b->switch2, CONSUMER));
    THROW_ON_ERROR(gpiod_line_request_both_edges_events(b->switch3, CONSUMER));
    //gpiod_line_request_both_edges_events(b->switch4, CONSUMER);

    gpiod_line_bulk_init(&b->switch13);
    gpiod_line_bulk_add(&b->switch13, b->switch1);
    gpiod_line_bulk_add(&b->switch13, b->switch3);

    gpiod_line_bulk_init(&b->switch123);
    gpiod_line_bulk_add(&b->switch123, b->switch1);
    gpiod_line_bulk_add(&b->switch123, b->switch2);
    gpiod_line_bulk_add(&b->switch123, b->switch3);
#else
    b->switch1 = SW1;
    b->switch2 = SW2;
    b->switch3 = SW3;
    b->switch4 = SW4;
#endif
    
}

void turn_on_diode(struct gpiod_line *diode)
{
#ifdef WITH_GPIO
    THROW_ON_ERROR(gpiod_line_set_value(diode, HIGH));
#else
    printf("(");
    fflush(stdout);
#endif
}

void turn_off_diode(struct gpiod_line *diode)
{
#ifdef WITH_GPIO
    THROW_ON_ERROR(gpiod_line_set_value(diode, LOW));
#else
    printf(")");
    fflush(stdout);
#endif
}

int is_switch_pressed(struct gpiod_line *sw)
{
#ifdef WITH_GPIO
    return !THROW_ON_ERROR(gpiod_line_get_value(sw));
#else
    return 0;
#endif
}

void unregister_board(struct board *b)
{
#ifdef WITH_GPIO
    gpiod_line_release(b->diode1_red);
    gpiod_line_release(b->diode2_yellow);
    gpiod_line_release(b->diode3_green);
    gpiod_line_release(b->diode4_blue);

    gpiod_line_release(b->switch1);
    gpiod_line_release(b->switch2);
    gpiod_line_release(b->switch3);
    gpiod_line_release(b->switch4);

    gpiod_chip_close(b->chip);
#endif
}

#ifdef WITH_GPIO
const struct timespec max_bounce_time = {0, 5000000}; // 5ms

int debounce_wait_read(struct gpiod_line *line, const struct timespec *timeout, struct gpiod_line_event *event)
{
    int res = gpiod_line_event_wait(line, timeout);
    printf("po 1 czekaniu\n");
    if (res <= 0) return res;
    if (gpiod_line_event_read(line, event) == -1) return -1;
    while ((res = gpiod_line_event_wait(line, &max_bounce_time)) > 0)
    {
        if (res == -1) return -1;

        printf("a");
        fflush(stdout);

        if (gpiod_line_event_read(line, event) == -1) return -1;
    }
    return 1;
}

int debounce_wait_read_bulk(struct gpiod_line_bulk *lines, const struct timespec *timeout, struct gpiod_line** line_read, struct gpiod_line_event *event)
{
    struct gpiod_line_bulk event_bulk = GPIOD_LINE_BULK_INITIALIZER;
    int res = gpiod_line_event_wait_bulk(lines, timeout, &event_bulk);
    if (res <= 0) return res;
    *line_read = THROW_ON_NULL(gpiod_line_bulk_get_line(lines, 0));
    if (gpiod_line_event_read(*line_read, event) == -1) return -1;
    while ((res = gpiod_line_event_wait_bulk(lines, &max_bounce_time, &event_bulk)) > 0)
    {
        if (res == -1) return -1;
        *line_read = THROW_ON_NULL(gpiod_line_bulk_get_line(lines, 0));
        if (gpiod_line_event_read(*line_read, event) == -1) return -1;
    }
    return 1;
}
#endif

int wait_for_switch_then_get_time_pressed(struct gpiod_line *line, struct timespec *time)
{
#ifdef WITH_GPIO
    struct gpiod_line_event event;
    struct timespec old_time;
    THROW_ON_ERROR(debounce_wait_read(line, NULL, &event));
    //THROW_ON_ERROR(gpiod_line_event_read(line, &event));
    while (event.event_type != SWITCH_PRESSED)
    {
        THROW_ON_ERROR(debounce_wait_read(line, NULL, &event));
        //THROW_ON_ERROR(gpiod_line_event_read(line, &event));
    }
    old_time = event.ts;

    THROW_ON_ERROR(debounce_wait_read(line, NULL, &event));
    //THROW_ON_ERROR(gpiod_line_event_read(line, &event));
    if (event.event_type != SWITCH_RELEASED)
    {
        return -1; // pressed too fast to register
    }
    diff_timespec(&(event.ts), &old_time, time);
    return 1;
#else
    char c;
    scanf(" %c",&c);
    time->tv_sec=1;
    time->tv_nsec=0;
    return 1;
#endif
}

void clear_line_buffer(struct gpiod_line *line)
{
#ifdef WITH_GPIO
    struct timespec timeout = {0,0};
    struct gpiod_line_event e;
    while (THROW_ON_ERROR(gpiod_line_event_wait(line, &timeout)))
    {
        THROW_ON_ERROR(gpiod_line_event_read(line, &e));
    }
#endif
}

void clear_line_buffer_bulk(struct gpiod_line_bulk *line_bulk)
{
#ifdef WITH_GPIO
    struct gpiod_line *line, **pline;
    gpiod_line_bulk_foreach_line(line_bulk, line, pline) clear_line_buffer(line);
#endif
}

int wait_for_switches_then_get_line_and_time_pressed(struct gpiod_line_bulk *line_bulk, struct timespec *time, struct gpiod_line **pline, struct timespec *timeout)
{
#ifdef WITH_GPIO
    struct gpiod_line_event event;
    struct timespec old_time;
    int ret = THROW_ON_ERROR(debounce_wait_read_bulk(line_bulk, timeout, pline, &event));
    if (ret == 0) return 0;
    while (event.event_type != SWITCH_PRESSED)
    {
        THROW_ON_ERROR(debounce_wait_read_bulk(line_bulk, timeout, pline, &event));
    }
    old_time = event.ts;

    //THROW_ON_ERROR(debounce_wait(event_line, NULL));
    //THROW_ON_ERROR(gpiod_line_event_read(event_line, &event));
    THROW_ON_ERROR(debounce_wait_read(*pline, NULL, &event));
    if (event.event_type != SWITCH_RELEASED)
    {
        return -1; // pressed too fast to register
    }
    diff_timespec(&(event.ts), &old_time, time);

    return 1;
#else
    char c;
    scanf(" %c",&c);
    time->tv_nsec=0;
    time->tv_sec=0;
    switch (c)
    {
    case '1':
        *pline = SW1;
        break;
    case '2':
        *pline = SW2;
        break;
    case '3':
        *pline = SW3;
        break;
    case '4':
        *pline = SW4;
        break;
    case '.':
        *pline = SW1;
        break;
    case '-':
        time->tv_sec=4;
        *pline = SW1;
        break;
    default:
        return 0;
    }
    return 1;
#endif
}

