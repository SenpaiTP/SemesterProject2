/*
 * PlanteUSART.c
 *
 * Created: 06-11-2023 12:59:32
 * Author : thoma
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>



#define F_CPU 16000000UL
#define BAUD 38400
#define UBBR_VALUE ((F_CPU / (16UL * BAUD)) - 1)



void USART_TransmitChar(uint8_t data) {
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));
	
	// Put data into buffer, sends the data
	UDR0 = data;
}
void USART_TransmitString(const char* str) {
	for (uint8_t i = 0; str[i] != '\0'; i++) {
		USART_TransmitChar(str[i]);
	}
}

void USART_Init(uint16_t ubrr) {
	// Set baud rate
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Set frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (3 << UCSZ00);
	
	// Enable USART Receive Complete (USART_RXC) interrupt
	UCSR0B |= (1 << RXCIE0);
	USART_TransmitChar('H');
	USART_TransmitChar('i');
	USART_TransmitChar('\n');
}

ISR(USART0_RX_vect) {
	unsigned char receivedData = UDR0;
	
	USART_TransmitChar(receivedData); // Echo received character
	if(receivedData == 'k'){
		;//Send data fra sensor...Sikkert en funktion
	}
	
	
}

int main(void) {
	USART_Init(UBBR_VALUE);
	sei(); // Enable global interrupts
	
	while (1) {
	}
	
	return 0;
}

