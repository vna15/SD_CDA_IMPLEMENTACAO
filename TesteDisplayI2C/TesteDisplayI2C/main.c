/*
 * main.c
 *
 * Created: 4/9/2021 3:00:45 PM
 *  Author: Pedro
 */ 

#define F_CPU 16000000
#include <xc.h>
#include <util/delay.h>

void ConfigTWI();
void Start();
void AddrEscravo(int Addr);
void EnableLCD();
void Stop();
void carregaMSG();
void ConfigLCD();
void EscreveTraco();

int main(void) {
	//Cnfiguração do I2C
	ConfigTWI();
	//Configuração do LCD16x2
	ConfigLCD();
	
	//----------------
	Start(); 
	AddrEscravo(0x40); //Endereço do escravo
	
	EscreveTraco();
	EscreveTraco();
	EscreveTraco();
	
	Stop();
	//----------------
		
	while (1) {
		
	}
}

void ConfigTWI(){
	//Configuração do módulo TWI
	DDRC = (1 << 4) | (1 << 5);
	TWSR |= 0b00000001; //Prescaler = 4
	//Fator de divisão para geração de 100khz
	TWBR= 18;
	TWCR= 0b00000100; //Habilita o módulo TWI (TWEN = 1)
	//------------------------------------------------------
}

void Start(){
	//Envia a condição de START
	TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
	while (!(TWCR & (1<<TWINT)));
}

void AddrEscravo(int Addr){
	//Envia o endereço e o bit de R/W
	TWDR = Addr ; //0b0100 000 0
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));	
}

void EnableLCD(){
	_delay_ms(10);
	//Envia o dado
	TWDR |= (1<<TWD3) ; //enable HIGH
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
	_delay_ms(10);
	//Envia o dado
	TWDR &= ~(1<<TWD3) ; //enable low
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
	_delay_ms(10);
}

void carregaMSG(){
	_delay_ms(10);
	//Envia o dado
	TWDR |= (1<<TWD1) ; //RS HIGH
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
	_delay_ms(10);

	EnableLCD();
}

void Stop(){
	//Envia uma condição de STOP
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	//--------------------------------------------------------
	_delay_ms(100);
}

void EscreveTraco(){
	//Envia Caracter - " - " => (0010-1101)
	TWDR |= (1<<TWD5); // (0010-****)
	TWDR &= ~(1<<TWD7)&~(1<<TWD6)&~(1<<TWD4);
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
	carregaMSG();
	//Envia o dado
	TWDR |= (1<<TWD7)|(1<<TWD6)|(1<<TWD4); // (1101-****)
	TWDR &= ~(1<<TWD5);
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
	carregaMSG();	
}

void ConfigLCD(){
	
Start();
//---------------------------------------------------
AddrEscravo(0x40);
//----------------------------------------------------
//-----------SetUp Display--------Modo 4 bits--------(0010-1000)-------------------------
//Envia o dado
TWDR |= (1<<TWD5) ; //modo 4 bits do display (0010-0000)
TWDR &= ~(1<<TWD7)&~(1<<TWD6)&~(1<<TWD4);
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//Envia o dado
TWDR |= (1<<TWD4) ; //Configura segunda linha (****-0001)
TWDR &= ~(1<<TWD7)&~(1<<TWD6)&~(1<<TWD5);
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();

//-----------------Limpas Display => (0000-0001)-------------------
//Envia o dado
TWDR &= ~(1<<TWD7)&~(1<<TWD6)&~(1<<TWD5)&~(1<<TWD4);	//modo 4 bits do display (0000-0000)
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//Envia o dado
TWDR |= (1<<TWD4) ; //Configura segunda linha (0001-0000)
TWDR &= ~(1<<TWD7)&~(1<<TWD6)&(1<<TWD5);
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//----------------------------------------------------------------------

//-----------Liga Display / Liga Cursor / Liga Blinck do cursor => (0000-1111)------------
TWDR &= ~(1<<TWD7)&~(1<<TWD6)&~(1<<TWD5)&~(1<<TWD4);	// (0000-0000)
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//Envia o dado
TWDR |= (1<<TWD7)|(1<<TWD6)|(1<<TWD5)|(1<<TWD4) ; // (1111-****)
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//----------------------------------------------------------------------

//----------------Entry Mode Set => (0000-0110)------------------------
TWDR &= ~(1<<TWD7)&~(1<<TWD6)&~(1<<TWD5)&~(1<<TWD4);	// (0000-0000)
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//Envia o dado
TWDR |= (1<<TWD6)|(1<<TWD5); // (0110-****)
TWDR &= ~(1<<TWD7)&~(1<<TWD4);
TWCR = ((1<< TWINT) | (1<<TWEN));
while (!(TWCR & (1 <<TWINT)));
EnableLCD();
//----------------------------------------------------

Stop();	
}