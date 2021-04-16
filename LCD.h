/*
 * LCD.h
 *
 * Created: 02/04/21  17:50:05
 * Author : Vinicius
 */ 


#ifndef LCD_H
#define LCD_H	   
#include <avr/io.h>
#include "TWI.h"
#include "RTC.h"

void dly(){
	for (int i = 0; i<10000; i++){
		//for (int j = 0; j<2; j++){
			//int a = 0;
			DDRB = 0xFF;
		//}
	}
}

void clk(uint8_t num){
	//dly();
	carregaMSG(num+128);
	//PORTB = num + 128;
	//dly();
	//carregaMSG(num);
	//PORTB = num;
	//dly();
	
}

void setLCD(){
	// Função para fazer a configura??o inicial do LCD.
	//DDRB = 0xFF;
	//PORTB = 0x00;
	//dly();
	
	// Modo de 4 bits
	carregaMSG(0b00001000);
	//TWDR = 0b00001000;
	clk(0b00001000);
	
	// Tela de 2 linhas
	carregaMSG(0b00001000);
	//TWDR = 0b00001000; //
	clk(0b00001000);
	carregaMSG(0b00100000);
	//TWDR = 0b00100000;
	clk(0b00100000);
	
	// Display On/Off
	carregaMSG(0b00000000);
	//TWDR = 0b00000000;
	clk(0b00000000);
	carregaMSG(0b00110000);
	//TWDR = 0b00111000;
	clk(0b00110000);
	
	// Mode Set
	carregaMSG(0b00000000);
	//TWDR = 0b00000000;
	clk(0b00000000);
	carregaMSG(0b00011000);
	//TWDR = 0b00011000;
	clk(0b00011000);
}

void secondLine(){
	carregaMSG(0b00110000);
	//TWDR = 0b00110000;
	clk(0b00110000);
	carregaMSG(0b00000000);
	//TWDR = 0b00000000;
	clk(0b00000000);
}

void position0(){
	carregaMSG(0b00100000);
	//TWDR = 0b00110000;
	clk(0b00110000);
	carregaMSG(0b00000000);
	//TWDR = 0b00000000;
	clk(0b00000000);
}

void shiftDisplayRight(){
	carregaMSG(0b00000100);
	//TWDR = 0b00000100;
	clk(0b00000100);
	carregaMSG(0b00111000);
	//TWDR = 0b00111000;
	clk(0b00111000);
}

void shiftCursorRight(){
	carregaMSG(0b00000100);
	//TWDR = 0b00000100; 
	clk(0b00000100);
	carregaMSG(0b00011000);
	//TWDR = 0b00011000;
	clk(0b00011000);
}

void blank(){
	carregaMSG(0b01001000);
	clk(0b01001000);
	carregaMSG(0b01000000);
	clk(0b01000000);	
}

void clearDisplay(){
	carregaMSG(0b00000000);
	clk(0b00000000);
	carregaMSG(0b00000100);
	clk(0b00000100);
}

void writeCaracter(uint8_t partOne, uint8_t partTwo){
	carregaMSG(partOne);
	//TWDR = partOne; 
	clk(partOne);
	carregaMSG(partTwo);
	//TWDR = partTwo;
	clk(partTwo);
}

void writeScreen(char* letters){
	int letterOne = 0, letterTwo = 0, tam = 0, x = 0; 
	for(int j =0; j<32; j++){
		if(letters[j] == '\0'){
			break;
		}
		tam++;
	}
	for(int i = 0; i<tam; i++){
		if(letters[i] == '\n'){
			secondLine();
		}
		else if(letters[i] == ' '){
			blank();
			//shiftCursorRight();
		}
		else{
		letterOne = (letters[i]>>2) | 64;
		letterTwo = ((letters[i]<<2) | 64) & 127;
		writeCaracter(letterOne, letterTwo);
		}
	}
}

void ZeroinLeft(int number){
	if(number<10){
		writeScreen("0");
	}
}

void dataHourScreen(){
	int hora, min, seg, diaWeek, dia, mes, ano;
	char horaC[9] = {'\0'};
	char minC[9] = {'\0'};
	char segC[9] = {'\0'};
	char diaWeekC[9] = {'\0'};
	char diaC[9] = {'\0'};
	char mesC[9] = {'\0'};
	char anoC[9] = {'\0'};
	Start();
	AddrEscravo(0xD1);
	seg = readRTC(1);
	min = readRTC(1);
	hora = readRTC(1);
	diaWeek = readRTC(1);
	dia = readRTC(1);
	mes = readRTC(1);
	ano = readRTC(0);
	Stop();
	itoa(seg,segC,10);
	itoa(min,minC,10);
	itoa(hora,horaC,10);
	itoa(dia,diaC,10);
	itoa(mes,mesC,10);
	itoa(ano,anoC,10);
	Start();
	AddrEscravo(0x40);
	position0();
	writeScreen("Hora: ");
	ZeroinLeft(hora);
	writeScreen(horaC);
	writeScreen(":");
	ZeroinLeft(min);
	writeScreen(minC);
	writeScreen(":");
	ZeroinLeft(seg);
	writeScreen(segC);
	secondLine();
	writeScreen("Data: ");
	ZeroinLeft(dia);
	writeScreen(diaC);
	writeScreen("/");
	ZeroinLeft(mes);
	writeScreen(mesC);
	writeScreen("/");
	ZeroinLeft(ano);
	writeScreen(anoC);
	Stop();
	Start();
	AddrEscravo(0xD0);
	writeRTC(00);
	Stop();
}

#endif // LCD_H