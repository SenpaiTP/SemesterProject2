#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define XTAL 16000000

// Function to initialize UART with 8 data bits, no parity, and one stop bit.
void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int)
{
	if (BaudRate < 300 || BaudRate > 115200) {
		exit(0);
	}
	if (DataBit < 5 || DataBit > 8) {
		exit(0);
	}
	UCSR0C = (DataBit - 5)<<1;
	UCSR0B = 0b00011000;
	if (Rx_Int) {
		UCSR0B = UCSR0B | 0b10000000; //Slår Interrupts til
	}
	UBRR0 = (16000000 / (16 * BaudRate)) - 1;
}

// Function to send a character via UART.
void sendChar(char data) {
	while (UCSR0A & (1 << UDRE0) == 0); // Wait for the data register to be empty
	UDR0 = data; // Transmit the character
}

// Function to receive a character via UART.
char receiveChar() {
	while (UCSR0A & (1 << RXC0) == 0); // Wait for data to be received
	return UDR0; // Return received data
}

int main() {
	InitUART(9600, 8, 1); // Initialize UART

	while (1) {
		char receivedChar = receiveChar(); // Receive a character
		sendChar(receivedChar); // Send the received character back

		// Add your code to process the received character here
	}

	return 0;
}