#include "adc.h"
#include "open_interface.h"
#include "movement.h"

void inital_IR_distances(int * IR_val, int * IR_dist, oi_t * sensor_data){
    int move_dist = 50;
    int init_dist = 800;
    int i;
    //initial collection. put at 50cm and let it run to a wall till 10
    for(i=0; i<15; i++){ //fixed array of 16
        IR_val[i] = adc_read();
        IR_dist[i] = init_dist;
        move_forward(sensor_data, move_dist);
        init_dist -= move_dist;
    }
}

int IR_read(int * IR_val, int * IR_dist){
    int data = adc_read();
    int i = 0;
    while(abs(data - IR_val[i]) > abs(data-IR_val[i+1])){
        i++;
    }
    return IR_dist[i]/10; //returns distance in cm
}
