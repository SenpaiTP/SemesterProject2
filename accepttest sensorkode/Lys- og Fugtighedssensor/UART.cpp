#include "UART.h"
#define F_CPU 16000000UL // Definer CPU-taktfrekvensen til 16 MHz
#define BAUD 9600 // Definer baudrate til 9600
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

void USART_TransmitSensorData(float temperature, float humidity, float light) {
	char buffer[50];  // Adjust the size based on the maximum expected string length

	// Convert float values to integers (whole numbers)
	int tempInt = (int)temperature;
	int humidityInt = (int)(humidity / 10.0); // Divide humidity by 100
	int lightInt = (int)(light / 10.0);       // Divide light by 100

	// Format the values as strings
	snprintf(buffer, sizeof(buffer), "%d,%d,%d", tempInt, humidityInt, lightInt);

	// Transmit the string
	int len = strlen(buffer);
	for (int i = 0; i < len; ++i) {
		UDR0 = buffer[i];
		// Wait for empty transmit buffer before sending the next character
		while (!(UCSR0A & (1 << UDRE0)));
	}

	// Transmit newline character
	UDR0 = '\n';
	while (!(UCSR0A & (1 << UDRE0)));
}
