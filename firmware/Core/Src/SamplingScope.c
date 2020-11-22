/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"


#define MEAS_BUFFER_SIZE 500
#define FREQ_BUFFER_SIZE 10

uint8_t value_adc[MEAS_BUFFER_SIZE];
uint32_t frequency[FREQ_BUFFER_SIZE];
int newPeriod = 1000;
int newGen = 63;
int measure = 0;
int recieve = 0;
int fr = 0;
uint32_t signalFreq = 0;
int signalRepetVal = 64000;
uint8_t foo = 0;
uint8_t rx_buff[50];
char settingsStringArray[11][6];

uint16_t samplingDivider = 511;
uint16_t generatorDivider = 63;
uint16_t comparatorVoltage = 2048;
uint16_t comparatorHysteresis = 1;
bool channel1On = 1;
bool channel2On = 0;
bool channel3On = 0;
bool channel4On = 0;
uint16_t memDepth = 16384;
uint8_t bits = 8;
bool channel4Vref = 0;



void parseSerial() {
	if (rx_buff[0] == ':') {
		int bufferPointer = 1;
		for (int settingsPointer = 0; settingsPointer < 11; settingsPointer++) {
			int i = 0;
			while (rx_buff[bufferPointer] != '|' && rx_buff[bufferPointer] != ':' && bufferPointer < 50) {
				settingsStringArray[settingsPointer][i] = rx_buff[bufferPointer];
				i++;
				bufferPointer++;
			}
			bufferPointer++;
			settingsStringArray[settingsPointer][5] = 0;
		}
	}
	samplingDivider = atoi(settingsStringArray[0]);
	generatorDivider = atoi(settingsStringArray[1]);
	comparatorVoltage = atoi(settingsStringArray[2]);
	comparatorHysteresis = atoi(settingsStringArray[3]);
	channel1On = atoi(settingsStringArray[4]);
	channel2On = atoi(settingsStringArray[5]);
	channel3On = atoi(settingsStringArray[6]);
	channel4On = atoi(settingsStringArray[7]);
	memDepth = atoi(settingsStringArray[8]);
	bits = atoi(settingsStringArray[9]);
	channel4Vref = atoi(settingsStringArray[10]);

}

void sendSerial(){
	printf(":%u|", __HAL_TIM_GET_AUTORELOAD(&htim1));
	printf("%u|", __HAL_TIM_GET_AUTORELOAD(&htim8));
	printf("%u|", comparatorVoltage);
	printf("%u|", comparatorHysteresis);
	printf("%i|%i|%i|%i|", channel1On, channel2On, channel3On, channel4On);
	printf("%u|", memDepth);
	printf("%i|", channel4Vref);
	printf("%u:", signalFreq);
	for (int i = 0; i < memDepth; i++) {
		printf("%i,", value_adc[i]);
	}
	//printf("%i|\n\r", value_adc[MEAS_BUFFER_SIZE-1]);
	printf("!\n\r");


}



*/




































