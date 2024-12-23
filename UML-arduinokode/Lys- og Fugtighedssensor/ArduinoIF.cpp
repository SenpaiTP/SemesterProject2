#include "ArduinoIF.h"
#include <avr/eeprom.h>
void storeLocally(char* nData){
	for (int i = 0; i < sizeof(nData); ++i) {
		eeprom_write_byte((uint8_t*)(i), nData[i]);
	}
} 
void deleteLocalData(int dele, int address){
	for (int i = 0; i < dele; ++i) {
		eeprom_write_byte((uint8_t*)(address + i), 0);
	}
}