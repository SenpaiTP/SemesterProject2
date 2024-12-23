/*
 * ADC.c
 *
 * Created: 09-11-2023 12:26:10
 * Author : benja
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_int.h"
#include "ADC.h"
#include "led.h"
#include "switch.h"

/*
ISR(USART0_RX_vect) {
	SendChar(ADCW);
}
*/

ISR(TIMER1_OVF_vect)
{
	//Reload Timer 1: 0,25 seconds to next overflow
	SendChar('d');
	TCNT1 = 65536-62500;
	toggleLED(1);
}


ISR(ADC_vect)
{
	// Send ADC result to terminal
	SendString ("ADC0 = ");
	SendInteger(ADCW);
	SendChar('\r');
}



int main(void)
{
	InitUART(9600, 8, 1);
	Init_ADC();
	Init_Timer1();
	initLEDport();
    /* Replace with your application code */
    
	while (1) 
    {
		SendChar(' ');
		char input = ReadChar();
		if (input) {
			SendInteger(ADCW);
		}
    }
	
	
}

