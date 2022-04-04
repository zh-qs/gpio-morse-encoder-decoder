#include "boardctl.h"
#include "time_operations.h"

void register_board(struct board *b)
{
    b->chip = gpiod_chip_open_by_name(CHIPNAME);

    b->diode1_red = gpiod_chip_get_line(b->chip, LINED1);
    b->diode2_yellow = gpiod_chip_get_line(b->chip, LINED2);
    b->diode3_green = gpiod_chip_get_line(b->chip, LINED3);
    b->diode4_blue = gpiod_chip_get_line(b->chip, LINED4);

    gpiod_line_request_output(b->diode1_red, CONSUMER, 0);
    gpiod_line_request_output(b->diode2_yellow, CONSUMER, 0);
    gpiod_line_request_output(b->diode3_green, CONSUMER, 0);
    gpiod_line_request_output(b->diode4_blue, CONSUMER, 0);

    b->switch1 = gpiod_chip_get_line(b->chip, LINESW1);
    b->switch2 = gpiod_chip_get_line(b->chip, LINESW2);
    b->switch3 = gpiod_chip_get_line(b->chip, LINESW3);
    b->switch4 = gpiod_chip_get_line(b->chip, LINESW4);

    // gpiod_line_request_input(b->switch1, CONSUMER);
    // gpiod_line_request_input(b->switch2, CONSUMER);
    // gpiod_line_request_input(b->switch3, CONSUMER);
    // gpiod_line_request_input(b->switch4, CONSUMER);
    gpiod_line_request_both_edges_events(b->switch1, CONSUMER);
    gpiod_line_request_both_edges_events(b->switch2, CONSUMER);
    gpiod_line_request_both_edges_events(b->switch3, CONSUMER);
    //gpiod_line_request_both_edges_events(b->switch4, CONSUMER);

    b->switch13 = gpiod_line_bulk_new(2);
    gpiod_line_bulk_add_line(b->switch13, b->switch1);
    gpiod_line_bulk_add_line(b->switch13, b->switch3);

    b->switch123 = gpiod_line_bulk_new(3);
    gpiod_line_bulk_add_line(b->switch13, b->switch1);
    gpiod_line_bulk_add_line(b->switch13, b->switch2);
    gpiod_line_bulk_add_line(b->switch13, b->switch3);

    // request events!
}

void turn_on_diode(struct gpiod_line *diode)
{
    gpiod_line_set_value(diode, HIGH);
}

void turn_off_diode(struct gpiod_line *diode)
{
    gpiod_line_set_value(diode, LOW);
}

int is_switch_pressed(struct gpiod_line *sw)
{
    return !gpiod_line_get_value(sw);
}

void unregister_board(struct board *b)
{
    gpiod_line_bulk_free(b->switch13);
    gpiod_line_bulk_free(b->switch123);

    gpiod_line_release(b->diode1_red);
    gpiod_line_release(b->diode2_yellow);
    gpiod_line_release(b->diode3_green);
    gpiod_line_release(b->diode4_blue);

    gpiod_line_release(b->switch1);
    gpiod_line_release(b->switch2);
    gpiod_line_release(b->switch3);
    gpiod_line_release(b->switch4);

    gpiod_chip_close(b->chip);
}

int wait_for_switch_then_get_time_pressed(struct gpiod_line *line, struct timespec *time)
{
    struct gpiod_line_event event;
    struct timespec old_time;
    gpiod_line_event_read(line, &event);
    while (event->event_type != SWITCH_PRESSED)
    {
        gpiod_line_event_read(line, &event);
    }
    old_time = event.ts;
    
    gpiod_line_event_read(line, &event);
    if (event->event_type != SWITCH_RELEASED)
    {
        return -1; // pressed too fast to register
    }
    diff_timespec(&(event->ts), &old_time, time);
    return 1;
}

void clear_line_buffer(struct gpiod_line *line)
{
    struct timespec timeout = {0,0};
    struct gpiod_line_event e;
    while (gpiod_line_event_wait(line, &timeout))
    {
        gpiod_line_event_read(line, &e);
    }
}

void clear_line_buffer_func(struct gpiod_line *line, void*)
{
    clear_line_buffer(line);
}

void clear_line_buffer_bulk(struct gpiod_line_bulk *line_bulk)
{
    gpiod_line_bulk_foreach_line(line_bulk, clear_line_buffer_func, NULL);
}

int wait_for_switches_then_get_line_and_time_pressed(struct gpiod_line_bulk *line_bulk, struct timespec *time, struct gpiod_line **pline, struct timespec *timeout)
{
    struct gpiod_line_bulk *event_bulk = gpiod_line_bulk_new(3);
    struct gpiod_line_event event;
    struct gpiod_line *event_line;
    struct timespec old_time;
    int ret = gpiod_line_event_wait_bulk(line_bulk, timeout, event_bulk);
    if (ret == 0) return 0;
    event_line = gpiod_line_bulk_get_line(event_bulk, 0);
    gpiod_line_event_read(event_line, &event);
    while (event->event_type != SWITCH_PRESSED)
    {
        gpiod_line_event_wait_bulk(line_bulk, timeout, event_bulk);
        event_line = gpiod_line_bulk_get_line(event_bulk, 0);
        gpiod_line_event_read(event_line, &event);
    }
    old_time = event.ts;

    gpiod_line_event_read(event_line, &event);
    if (event->event_type != SWITCH_RELEASED)
    {
        return -1; // pressed too fast to register
    }
    diff_timespec(&(event->ts), &old_time, time);

    *pline = event_line;

    gpiod_line_bulk_free(event_bulk);
    return 1;
}

