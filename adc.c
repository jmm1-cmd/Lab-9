
#include "adc.h"
/*
  Initialize PORTB to be ADC 
 */
void adc_init() {
	SYSCTL_RCGCGPIO_R |= 0b000010; //initalize clock to Port B
	while ((SYSCTL_PRGPIO_R & 0b000010) == 0) {}; //wait until Port B is ready to access
	GPIO_PORTB_DIR_R &= 0b11101111; // make PB4 an input on the microcontroller
	GPIO_PORTB_AFSEL_R |= 0b00010000; //enable alternate function on PB4
	GPIO_PORTB_DEN_R &= 0b11101111; //disable digital I/O on PB4
	GPIO_PORTB_AMSEL_R |= 0b00010000; //enable analog function on PB4
	SYSCTL_RCGCADC_R |= 0b01; //enable the clocks to ADC module 0
	while ((SYSCTL_PRADC_R & 0b01) == 0) {}; //wait until ADC module is ready to access
	ADC0_ACTSS_R &= 0b0111; //disable sequencer 3
	ADC0_EMUX_R &= ~0xF000; //sequencer 3 is a software trigger
	ADC0_SSMUX3_R &= 0x0; //clear SS3 field
	ADC0_SSMUX3_R += 10; // set channel Ain10 (PB4)
	ADC0_SSCTL3_R = 0x0006; //no TS0 D0, yes IE0 END0
	ADC0_SAC_R = 0x4; //setup to do 16x hardware oversampling
	ADC0_ACTSS_R |= 0b1000; //enable sample sequencer 3
}
/*
Attain a value out of the analog to digital converter
*/
int adc_read(void) {
	int result;
	ADC0_PSSI_R = 0b1000; //initatie sample sequencer 3
	while ((ADC0_RIS_R & 0b1000) == 0) {}; //wait until a sample has completed conversion
	result = ADC0_SSFIFO3_R & 0xFFF; //store value in a variable
	ADC0_ISC_R = 0b1000; //clear the flag saying conversion is done
	return result; //output the value of the function
}

/*
Print value to LCD panel
*/
void adc_print(int data) {
	char buffer[50]; //create a buffer for a string
	sprintf(buffer, "Value is %d", data); //populate the buffer with values
	lcd_printf("%s", buffer); //print the string to the LCD panel
}





