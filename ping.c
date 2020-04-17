/*
 * ping.c
 *
 *  Created on: Mar 10, 2020
 *      Author: joemm
 */

#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <timer.h>
#include <driverlib/interrupt.h>

volatile unsigned long last_time = 0;
volatile unsigned long current_time = 0;
volatile signed long time_diff = 0;
volatile int update_flag = 0;

void TIMER3B_Handler(void){
    if (TIMER3_MIS_R & 0x400){
        last_time = current_time;
        current_time = TIMER3_TBR_R;
        update_flag = update_flag + 1;
        TIMER3_ICR_R |= 0x400; //Clear capture interrupt flag
    }
}

void ping_init(void){ //initialize the timer

    SYSCTL_RCGCGPIO_R |= 0x02; //enable clock to port B
    while((SYSCTL_RCGCGPIO_R & 0x02) == 0){}; //wait for port B to be ready
    SYSCTL_RCGCTIMER_R |= 0x08; //enable clock to Timer 3
    while((SYSCTL_PRTIMER_R & 0x08) == 0) {}; //wait for Timer 3 to be ready

    GPIO_PORTB_DEN_R |= 0x08; //enable pin 3 on port B
    GPIO_PORTB_AFSEL_R |= 0x08; //enable alternate function to pin 3
    GPIO_PORTB_PCTL_R |= 0x00007000; //set the alternate function of pin 3 to 7

    TIMER3_CTL_R &= 0x2FF; //disable the timer while it is being configured
    TIMER3_CFG_R |= 0x4; // set the time to 16-bit mode
    TIMER3_TBMR_R |= 0x7; //Edge-time mode & capture mode
    TIMER3_TBMR_R &= 0xEF; //count down
    TIMER3_TBILR_R |= 0xFFFF; //sets a 16-bit max value
    TIMER3_TBPR_R |= 0xFF; //sets 8-bit max value for the prescaler

    TIMER3_ICR_R |= 0x400; //Clear capture interrupt flag
    TIMER3_IMR_R |= 0x400; //Enable TB capture interrupt
    NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF0F) | 0x20; //set the priority to T3CCP1
    NVIC_EN1_R |= 0x10; //set bit 4

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable(); //allow CPU to service interrupts

    TIMER3_CTL_R |= 0xD00; //Both edges, enable to timer now that it is configured

}

void ping_trigger(void){ //activate the PING sensor
    TIMER3_CTL_R &= ~0x100; //disable the timer //~0x100;
    //TIMER3_IMR_R &= ~0x400; //Mask timer interrupt
    GPIO_PORTB_AFSEL_R &= ~0x08; //Disable alternate function to PB3
    GPIO_PORTB_DIR_R |= 0x08; //set PB3 as an output
    GPIO_PORTB_DATA_R &= 0xF7; //set PB3 low
    GPIO_PORTB_DATA_R |= 0x08; //set PB3 HigH
    timer_waitMicros(5); //wait 5 microseconds

    GPIO_PORTB_DIR_R &= 0b11110111; //set PB3 as an input
    GPIO_PORTB_DATA_R &= 0xF7; //set PB3 low
    GPIO_PORTB_AFSEL_R |= 0x08; //reenable the alternate function to PB3

    //TIMER3_ICR_R |= 0x400; //Clear capture interrupt flag
    //TIMER3_IMR_R |= 0x400; //Unmask timer intterupt
    TIMER3_CTL_R |= 0x100; //enable the timer

}

long ping_getClockCycles(void){

    long clock_cycles;
    if (last_time < current_time){
        time_diff = 16777216 +last_time - current_time;//overflow has occured
    }
    else{
        time_diff = last_time - current_time; //calculate the time difference  ((unsigned long) overflow << 24)+
    }
    clock_cycles = time_diff;
    //update_flag = 0; //this is only here for PART 2
    return clock_cycles;
}

float ping_getDistance(void){ //calculate distance
    float distance;
    distance = 0.0343*((time_diff/16)/2); //distance in centimeters
    update_flag = 0;
    return distance;
}
