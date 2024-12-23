#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "Sensor.h"

#define LDR_CHANNEL 0 // ADC channel for LDR sensor (ADC0 corresponds to A0)
#define SOIL_MOISTURE_CHANNEL 1 // ADC channel for soil moisture sensor (ADC1 corresponds to A1)

int main(void) {
	init_ADC(); // Initialize ADC
	init_UART(); // Initialize UART
	i2c_init();

	while (1) {
		float ldrValue = readADC(LDR_CHANNEL); // Read ADC value from LDR sensor
		float soilMoisture = readADC(SOIL_MOISTURE_CHANNEL); // Read ADC value from soil moisture sensor
		float temperature = readLM75Temperature(0); // Read temperature from LM75 sensor

		USART_TransmitSensorData(temperature, soilMoisture, ldrValue);

		_delay_ms(10000); // Update every second
	}
}
