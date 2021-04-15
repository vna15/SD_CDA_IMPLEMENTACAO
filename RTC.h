#ifndef RTC_H
#define RTC_H
#include <avr/io.h>
#include "TWI.h"

uint8_t dectobcd(int value){
	return ((value/10*16) + (value%10));
}

int bcdtodec(int value){
	return ((value/16*10) + (value%16));
}

void writeRTC(int number){
	int bcd = 0;
	bcd = dectobcd(number);
	carregaMSG(bcd);
}

int readRTC(int num){
	int dec;
	dec = bcdtodec(read_twi(num));
	return dec;
}


#endif // LCD_H