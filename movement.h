#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <open_interface.h>
//move irobot forward
double move_forward(oi_t *sensor_data, double distance_mm);
void turn_right(oi_t *sensor_data, double degrees);
void turn_left(oi_t *sensor_data, double degrees);
double move_backward(oi_t *sensor_data, double distance_mm);
#endif
