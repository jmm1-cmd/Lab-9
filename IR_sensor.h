/*
 * IR_sensor.h
 *
 *  Created on: Feb 28, 2020
 *      Author: jmm1
 */

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

#include "adc.h"
#include "open_interface.h"
#include "movement.h"

void inital_IR_distances(int * IR_val, int * IR_dist, oi_t * sensor_data);
int IR_read(int * IR_val, int * IR_dist);

#endif /* IR_SENSOR_H_ */
