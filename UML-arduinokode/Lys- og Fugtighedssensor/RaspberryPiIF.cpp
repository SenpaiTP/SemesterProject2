#include "RaspberryPiIF.h"
#include "Sensor.h"
#define F_CPU 16000000UL // Definer CPU-taktfrekvensen til 16 MHz
#define BAUD 57600 // Definer baudrate til 9600
#define UBRR_VALUE ((F_CPU / (BAUD * 16UL)) - 1) // Beregn UBRR-værdien for UART-kommunikation

void init_UART() {
	// Indstil baudrate
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)UBRR_VALUE;

	// Aktiver transmitter og receiver for UART-kommunikation
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

	// Indstil rammeformat: 8 data bits, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void storeOnServer(char data[]) {
	// Transmit the string
	int len = strlen(data);
	for (int i = 0; i < len; ++i) {
		UDR0 = data[i];
		// Wait for empty transmit buffer before sending the next character
		while (!(UCSR0A & (1 << UDRE0)));
	}
	// Transmit newline character
	UDR0 = '\n';
	while (!(UCSR0A & (1 << UDRE0)));
	
}