#include "DataindsamlingCtrl.h"
#include "ArduinoIF.h"
#include "Pumpe.h"
#include "Sensor.h"
#include <ctype.h>
volatile int receivedNumber = 0;
volatile bool isNumberComplete = false;

char* nData;

ISR(USART0_RX_vect){
	char incomingChar = UDR0;
	if(incomingChar == 'S'){	
		startPump();
		isNumberComplete = true;
	}
	else if (isdigit(incomingChar)) {
		// If the received character is a digit, update the receivedNumber
		receivedNumber = receivedNumber * 10 + (incomingChar - '0');
	}
	else if (incomingChar == '\n' || incomingChar == 'S') {
		// If a newline character is received, consider the number complete
		isNumberComplete = true;
		storeWaterDosage(receivedNumber);
	}
	if(incomingChar == 't'){
		deleteLocalData(sizeof(nData),0);
	}
	
}
