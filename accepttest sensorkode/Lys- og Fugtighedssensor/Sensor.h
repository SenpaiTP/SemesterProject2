#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


void init_ADC();
float readADC(uint8_t channel);
void i2c_start();
void i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char isLast);
void i2c_stop();
void i2c_init();
float readLM75Temperature(unsigned char SensorAddress);