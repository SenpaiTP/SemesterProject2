#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void init_UART();
void USART_TransmitSensorData(float temperature, float humidity, float light); 
void sendCharOverUART(char character);