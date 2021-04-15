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
	carregaMSG(0b00111000);
	//TWDR = 0b00111000;
	clk(0b00111000);
	
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

void writeCaracter(uint8_t partOne, uint8_t partTwo){
	carregaMSG(partOne);
	//TWDR = partOne; 
	clk(partOne);
	carregaMSG(partTwo);
	//TWDR = partTwo;
	clk(partTwo);
}

void writeScreen(char* letters){
	int letterOne = 0, letterTwo = 0; 
	int tam = sizeof(letters);
	for(int i = 0; i<tam; i++){
		if(letters[i] == '\n'){
			secondLine();
		}
		else if(letters[i] == ' '){
			shiftCursorRight();
		}
		else{
		letterOne = (letters[i]>>2) | 64;
		letterTwo = ((letters[i]<<2) | 64) & 127;
		writeCaracter(letterOne, letterTwo);
		}
	}
}

#endif // LCD_H