/*
 * servo.c
 *
 *  Created on: Apr 3, 2020
 *      Author: joemm
 */
#include <stdio.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <timer.h>
#include <driverlib/interrupt.h>
#include "lab_parts.h"

void servo_init(){
    SYSCTL_RCGCGPIO_R |=0x02; //enable clock to port B
    while((SYSCTL_PRGPIO_R & 0x02) == 0){}; //wait for port B to be ready
    SYSCTL_RCGCTIMER_R |= 0x02; //enable clock to Timer 1
    while((SYSCTL_PRTIMER_R & 0x02) == 0){}; //wait for Timer 1 to be ready

    GPIO_PORTB_DEN_R |= 0x20; //enable pin 5 on Port B
    GPIO_PORTB_AFSEL_R |= 0x20; //enable alternate function to pin5
    GPIO_PORTB_PCTL_R |=(GPIO_PORTB_PCTL_R & 0xFF0FFFFF) | 0x00700000; //select timer as alternate function

    TIMER1_CTL_R &= 0x2FF; //disable the timer while it is being configured
    TIMER1_CFG_R |= 0x4; //set the time to 16-bit mode
    TIMER1_TBMR_R |= 0xA; //enable PWM mode and periodic mode - PWM enables countdown directly
    TIMER1_TBPR_R = (TIMER1_TBPR_R & 0x00) | 0x4;
    TIMER1_TBILR_R = (TIMER1_TBILR_R & 0x0000) | 0xE200;
    TIMER1_TBPMR_R = (TIMER1_TBPMR_R & 0x00) | 0x4;
    TIMER1_TBMATCHR_R = (TIMER1_TBMATCHR_R & 0x0000) | 0x8440;

    TIMER1_CTL_R |= 0x100;//enable the timer
}

void servo_move(float degrees){
    unsigned long value;
    value = (int)(-156.666*degrees+311000); //obtain match value
    TIMER1_CTL_R &= 0x2FF; //disable the timer while new values are loaded
    TIMER1_TBPMR_R = (TIMER1_TBPMR_R & 0x00) | (value >> 16); //top eight bits are stored in prescaler
    TIMER1_TBMATCHR_R = (TIMER1_TBMATCHR_R & 0x0000) | value; //last 16 bits are stored
    TIMER1_CTL_R |= 0x100;//enable the timer
    timer_waitMillis(30);
}

int get_angle(void){
    unsigned long value;
    value = (TIMER1_TBPMR_R << 16) | TIMER1_TBMATCHR_R; //get curent value stored in registers
    value = -(value-311000)/156.666; //convert to degrees
    return value;
}

void move_formula(int degrees){
    float value = m*degrees + b;
    servo_move(value);
}

