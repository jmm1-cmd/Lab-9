

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "string.h"

//initialize the converter
void adc_init();

//functions for the adc
int adc_read(void);
void adc_print(int data);



#endif /* ADC_H_ */
