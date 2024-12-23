#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern char* nData;
void init_UART();
void storeOnServer(char data[]); 
void sendCharUART(char character);