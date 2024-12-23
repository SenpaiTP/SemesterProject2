#include "Pumpe.h"
#include <avr/io.h>
#include <util/delay.h>
int dosis = 10;
void startPump() {
	int time = 0;
	DDRB |= 0b00100000; // Sætter det relevante bit i DDRB-registeret for at sætte pumpPin som en OUTPUT
	PORTB |= 0b00100000; // Tænder pumpen ved at sætte det relevante bit i PORTB-registeret
	while(time < dosis){
		_delay_ms(10);
		time++;
	}
	PORTB &= 0b11011111;
}

void storeWaterDosage(int nDosis){
	dosis = nDosis;
}


