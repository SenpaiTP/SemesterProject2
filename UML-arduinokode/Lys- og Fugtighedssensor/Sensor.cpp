#include "Sensor.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#define LM75_BASE_ADDRESS 0b01001000
#define READ_MASK         0b00000001


///ADC ting
void init_ADC() {
	// Sæt reference spænding til AVCC (3.3V)
	ADMUX |= (1 << REFS0);

	// Aktiver ADC og indstil fordelingsfaktoren til 128 for lavere prøvetagningshastighed
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}



void i2c_init()
{
	// TWI prescaler = 1 (same as default)
	TWSR = 0;
	// LM75 clock has be to be lower than 400 kHz (according to LM75 data sheet)
	// We choose 2-wire clock ~ 216 kHz (when fCPU = 16 MHz)
	// The TWBR must be at least 10 in master mode (AVR data book)
	// SCL frequency = fCPU / (16 + 2*TWBR*1), when prescaler = 1
	TWBR = 29;
}

void i2c_start()
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0)
	{}
}

void i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0)
	{}
}

unsigned char i2c_read(unsigned char isLast)
{
	// If this is not the last byte to read
	if (isLast == 0)
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	// If this is the last byte to read
	else
	TWCR = (1<<TWINT) | (1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0)
	{}
	return TWDR;
}

void i2c_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}


char* requestData(){
	char* data = (char*)malloc(50);
	char tempStr[10], moistureStr[10], lightStr[10];
	
	
	//Temperatur
	int temperature1, temperature2;
	
	i2c_start();
	i2c_write(((LM75_BASE_ADDRESS + 0) << 1) | READ_MASK);
	
	temperature1 = i2c_read(1);  // Read high byte with ACK
	temperature2 = i2c_read(0);  // Read low byte with NACK
	
	i2c_stop();
	// Combine high and low bytes
	int temperatureValue = (temperature1 << 8) | temperature2;

	// Check if the temperature is negative
	if (temperatureValue & 0x8000) {
		// Perform sign extension for negative values
		temperatureValue |= 0xFFFF0000;
	}

	// Right shift to adjust for the 0.5°C resolution
	temperatureValue >>= 7;

	// Convert to float with 0.5°C resolution
	double temperature = (double)temperatureValue * 0.5;
	
	//Moisture
	double moisture;
	ADMUX = (ADMUX & 0xF0) | (0 & 0x0F); // Indstil ADC-kanalen
	ADCSRA |= (1 << ADSC); // Start ADC-konvertering

	while (ADCSRA & (1 << ADSC)); // Vent på at ADC-konvertering er færdig

	moisture = (double)(1023-ADC); // Returner ADC-værdien
	
	//Light
	double light;
	ADMUX = (ADMUX & 0xF0) | (1 & 0x0F); // Indstil ADC-kanalen
	ADCSRA |= (1 << ADSC); // Start ADC-konvertering

	while (ADCSRA & (1 << ADSC)); // Vent på at ADC-konvertering er færdig

	light = (double)(1023-ADC); // Returner ADC-værdien
	
	//Store data in stirng
	dtostrf(temperature, 4, 2, tempStr);
	dtostrf(moisture, 4, 2, moistureStr);
	dtostrf(light, 4, 2, lightStr);

	
	// Build the formatted string
	snprintf(data, 50, "%s,%s,%s", tempStr, moistureStr, lightStr);

	
	return data;
}