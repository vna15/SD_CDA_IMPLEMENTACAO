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

void setHour(int seg, int min, int hora, int diaWeek, int dia, int mes, int ano){
	Start();
	AddrEscravo(0xD0);
	writeRTC(00);
	writeRTC(seg);
	writeRTC(min);
	writeRTC(hora);
	writeRTC(diaWeek);
	writeRTC(dia);
	writeRTC(mes);
	writeRTC(ano);
	Stop();
	Start();
	AddrEscravo(0xD0);
	writeRTC(00);
	Stop();
}


#endif // LCD_H