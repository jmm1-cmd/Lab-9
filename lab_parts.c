/*
 * lab_parts.c
 *
 *  Created on: Apr 8, 2020
 *      Author: joemm
 */
#include <stdint.h>
#include <servo.h>
#include <timer.h>
#include <button.h>
#include <lcd.h>

volatile float m;
volatile int b;

void part1 (void){
    servo_move(90); //move the servo to angle 90
    timer_waitMillis(300);//wait for a moment
    servo_move(30); //move the servo to angle 30
    timer_waitMillis(300);//wait for a moment
    servo_move(150); //move the servo to angle 150
    timer_waitMillis(300);//wait for a moment
    servo_move(90); //move the servo to angle 90
}


void part2 (void){
    int x = 0;
    while (1){
    unsigned long w = get_angle();
    uint8_t y = button_getButton(); //check button status
    if (y == 3){ //if button three is pushed..
        x++; //increment x
    }
    if (x % 2 != 0){ //if x is odd rotate counter clock wise
        lcd_printf("Current angle: %d\n Counter Clockwise", w); //print angle and direction

        if (y == 1){ //if button one is pushed
            if ((w + 1) >= 180){ //if incrementing the angle will be larger than 180
                servo_move(180); //move to the max value
            }
            else{
                w += 1;
                servo_move(w); //add one to the angle
            }
        }

        if (y == 2){ //if button two is pushed
            if ((w +5) >= 180){ //if incrementing the angle will be larger than 180
                servo_move(180); // move to max value
            }
            else {
                w += 5;
                servo_move(w); // add five to the angle
            }
        }

        if (y == 4){ //if button four is pushed
            servo_move(5); //move to 5 degrees
        }
    }
    if (x % 2 == 0){ //if x is an even number
        lcd_printf("Current angle: %d\n Clockwise", w); //display angle and direction
        if (y==1){ //if button one is pushed
            if ((w - 1) <= 0){ //if decreasing the angle will be smaller than 0
                        servo_move(0);  //move to minimum value
                    }
            else{
                w -= 1;
                servo_move(w); //decrease the angle by one
            }
        }
        if (y == 2){ //if button two is pushed
            if ((w -5) <= 0){ //if decreasing the angle will be smaller than 0
                        servo_move(0);// move to minimum value
                    }
            else{
                w -= 5;
                servo_move(w); //decrease the angle by five
            }
        }
        if (y == 4){ //if button four is pushed
            servo_move(175); //move to angle 175
        }
    }
  }
}

void part3(void){
    int x = 0;
    while(1){
    int i = 0;
        int w = get_angle();
        uint8_t y = button_getButton(); //check button status
        while (!y){
            y = button_getButton();
        }
        if (y == 3){ //if button three is pushed..
            x++; //increment x
            timer_waitMillis(100);
        }
        if (y == 4){ //if button four is pushed
            servo_move(90); //move to 90 degrees
            timer_waitMillis(100);
        }
        if (x % 2 != 0){ //if x is odd rotate counter clock wise
            lcd_printf("Counter Clockwise"); //print direction
            if (y == 1){ //if button one is pushed
                for(i = 0; i<=180; i++){
                    w = get_angle();
                    if(w<=180){
                        servo_move(i); // move servo 180 degrees incrimenting by 1
                    } else{
                        servo_move(180);
                    }
                    timer_waitMillis(100);
                }
            }
            if (y == 2){ //if button two is pushed
                for(i = 0; i<=180; i= i+5){
                    w = get_angle();
                    if(w<=180){
                        servo_move(i); // move servo 180 degrees incrimenting by 5
                    } else{
                        servo_move(180);
                    }
                }
                timer_waitMillis(100);
            }
        }
        if (x % 2 == 0){ //if x is an even number
            lcd_printf("Clockwise"); //display direction
            if (y == 1){ //if button one is pushed
                for(i = 180; i>=0; i--){
                    w = get_angle();
                    if(w>=0){
                        servo_move(i); // move servo 180 degrees decrement by 1
                    } else{
                        servo_move(0);
                    }
                }
                timer_waitMillis(100);
            }
            if (y == 2){ //if button two is pushed
                for(i = 180; i>=0; i=i-5){
                    w = get_angle();
                    if(w>=0){
                        servo_move(i); // move servo 180 degrees decrement by 5
                    } else{
                        servo_move(0);
                    }
                }

            }
            timer_waitMillis(100);
        }
        timer_waitMillis(100);
    }
}

void part4(){

    int degrees[19] = {0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180};
    int values[19];
    int slopes[18];
    servo_move(0);
    int i = 0;
    int j = 0;
    float slope;
    int sum = 0;

    for(i=0; i <= sizeof(degrees)/sizeof(degrees[0]);i++){

        while(1){
            lcd_printf("Move to Degree: %d", degrees[i]);
            uint8_t y = button_getButton();
            unsigned long w = get_angle();

            if (y == 1){ //if button one is pushed
                w += 1;
                servo_move(w); //add one to the angle
                timer_waitMillis(100);
            }
            if (y == 2){ //if button one is pushed
                w -= 1;
                servo_move(w); //subtracts one to the angle
                timer_waitMillis(100);
            }
            if(y == 4){
                values[i] = w;
                timer_waitMillis(100);
                break;
            }
            timer_waitMillis(100);
         }
    }

    i = 0;
    for (j = 0; j < sizeof(slopes)/sizeof(slopes[0]); j++){
        while(1){
            slopes[j] = (values[i+1] - values[i]) / (degrees[i+1]-degrees[i]);
            break;
        }
        i++;
    }

    for(i = 0; i < sizeof(slopes)/sizeof(slopes[0]); i++){
        sum = sum + slopes[i];
    }

    slope = sum / (sizeof(slopes)/sizeof(slopes[0]));
    m = slope;
    b = values[0];
}

void part4_check (void){
        move_formula(0); //move the servo to angle 0
        timer_waitMillis(300);//wait for a moment
        move_formula(45); //move the servo to angle 45
        timer_waitMillis(300);//wait for a moment
        move_formula(90); //move the servo to angle 90
        timer_waitMillis(300);//wait for a moment
        move_formula(135); //move the servo to angle 135
        timer_waitMillis(300);//wait for a moment
        move_formula(180); //move the servo to angle 180
}
