#include "RaspberryPiIF.h"
#include "Sensor.h"
#include "Pumpe.h"
#include "ArduinoIF.h"
#include "DataindsamlingCtrl.h"
#include "VandingCtrl.h"

extern char* nData;
int main(void) {
	init_ADC(); // Initialiser ADC
	init_UART(); // Initialiser UART
	i2c_init();
	
	while (1) {
		nData = requestData();
		storeLocally(nData);
		storeOnServer(nData);
		_delay_ms(10000); // Opdater hver sekund
		free(nData);
	}
}
