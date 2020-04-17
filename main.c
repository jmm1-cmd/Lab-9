

/**
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include <timer.h>
#include <lcd.h>
#include <servo.h>
#include <button.h>
#include <lab_parts.h>
#include <adc.h>
#include <ping.h>
#include <uart.h>
#include <object_detection.h>

int main(void)
{
    timer_init(); //setup the timer
    lcd_init(); //setup the lcd screen
    uart_init(); // configured for baud rate of 115200
    servo_init();// initalize servo
    button_init(); //initalize button
    adc_init(); //initalize adc
    ping_init(); //initalize  sonar sensor

    move(); //button one should increment counterclockwise, button two should decrement clockwise (output to putty)
}

