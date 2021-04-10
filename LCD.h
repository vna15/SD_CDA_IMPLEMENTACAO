/*
 * LCD.h
 *
 * Created: 02/04/21  17:50:05
 * Author : Vinicius
 */ 


#ifndef LCD_H
#define LCD_H	   
#include <avr/io.h>

void dly(){
	for (int i = 0; i<255; i++){
		for (int j = 0; j<2; j++){
			//int a = 0;
			DDRB = 0xFF;
		}
	}
}

void clk(int num){
	dly();
	PORTB = num + 128;
	dly();
	PORTB = num;
	dly();
	
}	

void setLCD(){
	// Função para fazer a configuração inicial do LCD.
	DDRB = 0xFF;
	PORTB = 0x00;
	dly();
	
	// Modo de 4 bits
	PORTB = 0b00001000;
	clk(PORTB);
	
	// Tela de 2 linhas
	PORTB = 0b00001000; //
	clk(PORTB);
	PORTB = 0b00100000;
	clk(PORTB);
	
	// Display On/Off
	PORTB = 0b00000000;
	clk(PORTB);
	PORTB = 0b00111000;
	clk(PORTB);
	
	// Mode Set
	PORTB = 0b00000000;
	clk(PORTB);
	PORTB = 0b00011000;
	clk(PORTB);
}

void secondLine(){
	PORTB = 0b00110000;
	clk(PORTB);
	PORTB = 0b00000000;
	clk(PORTB);
}

void shiftDisplayRight(){
	PORTB = 0b00000100;
	clk(PORTB);
	PORTB = 0b00111000;
	clk(PORTB);
}

void shiftCursorRight(){
	PORTB = 0b00000100; 
	clk(PORTB);
	PORTB = 0b00011000;
	clk(PORTB);
}

void writeCaracter(uint8_t partOne, uint8_t partTwo){
	PORTB = partOne; 
	clk(PORTB);
	PORTB = partTwo;
	clk(PORTB);
}

void writeScreen(char* letters){
	int letterOne = 0, letterTwo = 0; 
	for(int i = 0; i<32; i++){
		if(letters[i] == '\n'){
			secondLine();
			i++;
		}
		if(letters[i] == ' '){
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
