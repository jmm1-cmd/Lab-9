

#include <servo.h>
#include <ping.h>
#include <math.h>
#include <uart.h>
#include <adc.h>
#include <button.h>

void sweep(void){
    int i = 0;
    int j = 0;
    int IR_distance;
    float Ping_distance;
    char buffer[50];
    int IR[90]; // IR sensor values
    float Ping[90]; // PING sensor values
    short object[90]; // if object, 1 if not 0
    float sum[6] = {0}; //sum of max of 6 objects
    int divisor[6] = {0}; //divisor of max of 6 objects
    float width[6] = {0}; //width of max of 6 objects
    short degrees[6] = {0}; //sum of degrees of max of 6 objects
    float object_angle[6]= {0}; // object angles max of 6
    float object_distance[6]= {0}; //object distance max of 6
    long clockCycles;
    //Header for Putty
    char header[] = "\n\rAngle\tPING Distance\tIR Raw Value\n\r"; //initialize graph headings
    uart_sendStr(header); //send the string to putty

    //moves servo 2 degrees at a time and records IR and PING data
    for(i=0; i<180; i = i+2){
        servo_move(i); //move servo
        IR_distance = adc_read(); //IR sensor distance
        ping_trigger(); //read from sensor
        while(update_flag ==0){}; //wait for rising edge
        while(update_flag ==1){}; //wait for falling edge
        if(update_flag == 2){ //falling edge has occured
           clockCycles = ping_getClockCycles();
           Ping_distance = ping_getDistance(); //get distance in centimeters
        }
        sprintf(buffer, "%d\t%f\t%d\n\r", i, Ping_distance, IR_distance); //format data to send to putty
        uart_sendStr(buffer); //send data over putty
        IR[j] = IR_distance; //store IR value into array
        Ping[j] = Ping_distance; //store Ping value into array
        j = j+1; //increment array position
    }

    //detects where objects are
    for(i=0; i<90; i = i+1){
        if(IR[i] <= 80){
            object[i] = 1; //object present
        }
        else{
            object[i] = 0; //object absent
        }
    }

    // Object definition parts based off object detection (sum,amount,degrees)
    j = 0;
    i = 0;
    while(i < 90){
        //If object is detected
        if(object[i]){
            j += 1;
            while(1){
                if(object[i] == 0){
                    break; //break if object is no longer detected
                }
                sum[j] = sum[j] + Ping[i]; //calculate sum of object distances
                divisor[j] = divisor[j] + 1; //calculate how many degrees object is in view
                degrees[j] = degrees[j] + i*2; //calculate sum of object degrees
                i += 1; //add 1 to i
            }
        }
        i += 1; //add 1 to i
    }

    // header for Putty
    char header1[] = "\n\rDistance\tAngle\tRadial Size\tWidth\n\r"; //initialize graph headings
    uart_sendStr(header1); //send the string to putty

    // Defining the objects based off parts
    for (i=0; i< sizeof(sum)/sizeof(sum[0]); i = i+1){
        object_angle[i] = degrees[i]/divisor[i]; //calcualte object angles
        object_distance[i] = (sum[i]/divisor[i]); //calculate object distances
        width[i] = (sum[i]/divisor[i])*(2*divisor[i])*(M_PI/180); // calculate object widths
        sprintf(buffer, "%f\t%f\t%d\t%f\n\r",object_distance[i] , object_angle[i], divisor[i]*2, width[i]); //format data to send to putty
        uart_sendStr(buffer); //send data over putty
    }

}

void move(void){
    unsigned long w;
    int y;
    long clockCycles;
    float Ping_distance;
    char buffer[50];
    char header2[] = "\n\rPing\n\r"; //initialize graph headings
    uart_sendStr(header2); //send the string to putty
    while (1){
        y = button_getButton();
        w = get_angle();
        if (y == 1){ //if button one is pushed
           if ((w + 1) >= 180){ //if incrementing the angle will be larger than 180
               servo_move(180); //move to the max value
           }
           else{
               w += 1;
               servo_move(w); //add one to the angle
           }
       }

        if (y==2){ //if button one is pushed
        if ((w - 1) <= 0){ //if decreasing the angle will be smaller than 0
                    servo_move(0);  //move to minimum value
                }
        else{
            w -= 1;
            servo_move(w); //decrease the angle by one
        }

        ping_trigger(); //read from sensor
        while(update_flag ==0){}; //wait for rising edge
        while(update_flag ==1){}; //wait for falling edge
        if(update_flag == 2){ //falling edge has occured
           clockCycles = ping_getClockCycles();
           Ping_distance = ping_getDistance(); //get distance in centimeters
        }
        sprintf(buffer, "%f\n\r", Ping_distance); //format data to send to putty
        uart_sendStr(buffer); //send data over putty
    }
        timer_waitMillis(100);
    }
}
