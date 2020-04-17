#include "open_interface.h"
#include "movement.h"
#include "lcd.h"


double move_forward(oi_t *sensor_data, double distance_mm){
	sensor_data->distance = 0;
    double sum = 0;
	
	while (sum < distance_mm) {
	    oi_setWheels(50,50); //move forward at full speed
		oi_update(sensor_data); //update the sensor data
		sum += sensor_data->distance; //derefrence the pointer and increase the value of sum
		//lcd_printf("%lf", sum);
	}
	
	oi_setWheels(0,0); //stop
	return sum;
	
}

double move_backward(oi_t *sensor_data, double distance_mm){
    sensor_data->distance = 0;
    double sum = 0;

    while (sum > distance_mm) {
        oi_setWheels(-250,-250); //move backward at full speed
        oi_update(sensor_data); //update the sensor data
        sum += sensor_data->distance; //derefrence the pointer and increase the value of sum
        //lcd_printf("%lf", sum);
    }

    oi_setWheels(0,0); //stop
    return sum;

}

void turn_right(oi_t *sensor_data, double degrees){
	sensor_data->angle = 0;
	double sum = 0;
	
	while(sum > degrees){
	    oi_setWheels(-250,250);
		oi_update(sensor_data);
		sum += sensor_data->angle;
		//lcd_printf("%lf", sum);
	}
	oi_setWheels (0,0);
}

void turn_left(oi_t *sensor_data, double degrees){
    sensor_data->angle = 0;
    double sum = 0;

	while(sum < degrees){
	    oi_setWheels(250,-250);
		oi_update(sensor_data);
		sum += sensor_data->angle;
		//lcd_printf("%lf", sum);
	}
	oi_setWheels (0,0);
}


