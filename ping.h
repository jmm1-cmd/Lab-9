/*
 * ping.h
 *
 *  Created on: Mar 10, 2020
 *      Author: joemm
 */

#ifndef PING_H_
#define PING_H_

extern volatile unsigned long last_time;
extern volatile unsigned long current_time;
extern volatile signed long time_diff;
extern volatile int update_flag;

void ping_init(void);
void ping_trigger(void);
void enableTimer(void);
float ping_getClockCycles(void);
float ping_getDistance(void);
void TIMER3B_Handler(void);

#endif /* PING_H_ */
