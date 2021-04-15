/*
 * CentraldeAlarme.c
 *
 * Created: 09/04/2021 17:57:51
 * Author : vnaze
 */ 

#include <avr/io.h>
#include "LCD.h"
#include "RTC.h"
#include "TWI.h"

int hora, min, seg;

int main(void)
{
	//int hora, min, seg;
	char horaC[9] = {'\0'};
	char minC[9] = {'\0'};
	char segC[9] = {'\0'};
	char testeC[9] = {'\0'};
	//itoa(hora,horaC,10);
	//uint8_t teste = 10001000;
	int teste = bcdtodec(153);
	
	ConfigTWI();
	Start();
	AddrEscravo(0x40);
	setLCD();
	//writeScreen("Central\n   de Alarme");
	Stop();
	Start();
	AddrEscravo(0xD0);
	writeRTC(00);
	writeRTC(50);
	writeRTC(9);
	writeRTC(18);
	writeRTC(5);
	writeRTC(15);
	writeRTC(04);
	writeRTC(21);
	Stop();
	Start();
	AddrEscravo(0xD0);
	writeRTC(00);
	Stop();
	/*Start();
	AddrEscravo(0xD1);
	seg = readRTC(1);
	min = readRTC(1);
	hora = readRTC(0);
	Stop();
	itoa(seg,segC,10);
	itoa(min,minC,10);
	itoa(hora,horaC,10);
	itoa(teste,testeC,10);
	Start();
	AddrEscravo(0x40);
	setLCD();
	writeScreen("   ");
	writeScreen(horaC);
	writeScreen(":");
	writeScreen(minC);
	writeScreen(":");
	writeScreen(segC);
	Stop();*/
	
    while (1) 
    {
		Start();
	AddrEscravo(0xD1);
	seg = readRTC(1);
	min = readRTC(1);
	hora = readRTC(0);
	Stop();
	itoa(seg,segC,10);
	itoa(min,minC,10);
	itoa(hora,horaC,10);
	Start();
	AddrEscravo(0x40);
	position0();
	writeScreen("Hora: ");
	writeScreen(horaC);
	writeScreen(":");
	writeScreen(minC);
	writeScreen(":");
	writeScreen(segC);
	Stop();
	Start();
	AddrEscravo(0xD0);
	writeRTC(00);
	Stop();
    }
}

