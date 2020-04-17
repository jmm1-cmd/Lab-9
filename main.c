

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
int main(void)
{
    timer_init(); //setup the timer
    lcd_init(); //setup the lcd screen
    servo_init();// initalize servo
    button_init(); //initalize button
    uart_init(); // configured for baud rate of 115200
    adc_init();
    ping_init();
}

