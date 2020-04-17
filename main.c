

/**
 * main.c
 */
#include <inc/tm4c123gh6pm.h>
#include <timer.h>
#include <lcd.h>
#include <servo.h>
#include <button.h>
#include <lab_parts.h>
int main(void)
{
    timer_init(); //setup the timer
    lcd_init(); //setup the lcd screen
    servo_init();// initalize servo
    button_init(); //initalize button


}

