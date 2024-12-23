/*
 * CFile1.c
 *
 * Created: 13-11-2023 12:25:03
 *  Author: benja
 */ 
#include <avr/io.h>
#include <stdlib.h>
#include "ADC.h"

// Target CPU frequency
#define XTAL 16000000

void Init_ADC()
{
	PORTF = 0b11111111;
	ADCSRA = 0b10101111; // Så skal prescalar sættes
	ADCSRB = 0b00000110;
	ADMUX = 0b01000001;
}

void Init_Timer1()
{
	// 16000000 Hz /64 = 250 kHz
	// Therefore we have 250000 "steps" per second
	// - and want 0,25 seconds until next overflow
	TCNT1 = 65536-62500;
	// Timer 1 in Normal Mode and PS = 64
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000011;
	// Enable Timer 1 overflow interrupts
	TIMSK1 |= 0b00000001;
}

int readADC() {
	ADCSRA |= 0b01000000;
	while(ADCSRA & 0b01000000) {
	}
	return ADCW;
}