#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000UL // Definer CPU-taktfrekvensen til 16 MHz
#define BAUD 9600 // Definer baudrate til 9600
#define UBRR_VALUE ((F_CPU / (BAUD * 16UL)) - 1) // Beregn UBRR-værdien for UART-kommunikation

#define LDR_CHANNEL 0 // Definer ADC-kanal til LDR-sensor (ADC0 svarer til A0)
#define SOIL_MOISTURE_CHANNEL 1 // Definer ADC-kanal til jordfugtighedssensor (ADC1 svarer til A1)

void init_ADC() {
	// Sæt reference spænding til AVCC (3.3V)
	ADMUX |= (1 << REFS0);

	// Aktiver ADC og indstil fordelingsfaktoren til 128 for lavere prøvetagningshastighed
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t readADC(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Indstil ADC-kanalen
	ADCSRA |= (1 << ADSC); // Start ADC-konvertering

	while (ADCSRA & (1 << ADSC)); // Vent på at ADC-konvertering er færdig

	return ADC; // Returner ADC-værdien
}

void init_UART() {
	// Indstil baudrate
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)UBRR_VALUE;

	// Aktiver transmitter og receiver for UART-kommunikation
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	// Indstil rammeformat: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void sendSensorData(uint16_t ldrValue, uint16_t soilMoisture) {
	// Beregn procentdelen for begge sensorer
	float ldrPercentage = (ldrValue / 1023.0) * 100.0;
	float soilMoisturePercentage = (soilMoisture / 1023.0) * 100.0;

	// Vent på at transmitteren er klar
	while (!(UCSR0A & (1 << UDRE0)));

	// Send data over UART som en streng "ldr,soilMoisture\n"
	char buffer[25];
	snprintf(buffer, sizeof(buffer), "%d,%d\n", (int)ldrPercentage, (int)soilMoisturePercentage);

	for (int i = 0; buffer[i] != '\0'; i++) {
		UDR0 = buffer[i];
		// Vent på at transmitteren er klar til den næste karakter
		while (!(UCSR0A & (1 << UDRE0)));
	}
}

int main(void) {
	init_ADC(); // Initialiser ADC
	init_UART(); // Initialiser UART

	while (1) {
		uint16_t ldrValue = readADC(LDR_CHANNEL); // Læs ADC-værdi fra LDR-sensoren
		uint16_t soilMoisture = readADC(SOIL_MOISTURE_CHANNEL); // Læs ADC-værdi fra jordfugtighedssensoren

		// Antag at lavere værdier for LDR angiver lysere lys
		ldrValue = 1023 - ldrValue;

		// Antag at højere værdier for jordfugtighed angiver tørre forhold
		soilMoisture = 1023 - soilMoisture;

		// Send LDR- og jordfugtighedsprocent over UART
		sendSensorData(ldrValue, soilMoisture);

		_delay_ms(1000); // Opdater hver sekund
	}
}
