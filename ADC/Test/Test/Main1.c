/*
 * ATmega16_soil_moisture.c
 * http://www.electronicwings.com
 */ 
 
#include <avr/io.h>
#include "LCD16x2_4bit.h"
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

void ADC_Init()
{
	DDRA=0x0;		/*  Make ADC port as input  */
	ADCSRA = 0x87;		/*  Enable ADC, fr/128  */	
}

int ADC_Read()
{
	ADMUX = 0x40;		/* Vref: Avcc, ADC channel: 0  */
	ADCSRA |= (1<<ADSC);	/* start conversion  */
	while ((ADCSRA &(1<<ADIF))==0);	/* monitor end of conversion interrupt flag */
	ADCSRA |=(1<<ADIF);	/* set the ADIF bit of ADCSRA register */
	return(ADCW);		/* return the ADCW */
}

int main(void)
{
	lcdinit();		/* initialize the 16x2 LCD */
	lcd_clear();		/* clear the LCD */
	ADC_Init();		/* initialize the ADC */
	char array[10];
	int adc_value;
	float moisture;

    while(1)
	{
		adc_value = ADC_Read();	/* Copy the ADC value */
		moisture = 100-(adc_value*100.00)/1023.00; /* Calculate moisture in % */
		lcd_gotoxy(0,0);
		lcd_print("Moisture: ");
		dtostrf(moisture,3,2,array);
		strcat(array,"%   ");	/* Concatenate unit of % */
		lcd_gotoxy(0,1);	/* set column and row */
		lcd_print(array);	/* Print moisture on second row */
		memset(array,0,10);
		_delay_ms(500);		
	}
}