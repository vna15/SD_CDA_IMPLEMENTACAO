#ifndef TWI_H
#define TWI_H
#define F_CPU 16000000
#include <avr\io.h>
#include <util\delay.h>

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

void carregaMSG(uint8_t dado){
	//_delay_ms(0.06);
	//Envia o dado
	TWDR = dado;
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
	_delay_ms(0.06);
}

void Stop(){
	//Envia uma condição de STOP
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	//--------------------------------------------------------
	_delay_ms(0.4);
}

int read_twi(uint8_t ack_dado) {
	_delay_ms(0.1);
	TWCR = ((1<< TWINT) | (1<<TWEN) | (ack_dado<<TWEA));
	while ( !(TWCR & (1 <<TWINT)));
	return TWDR;
	_delay_ms(0.1);
}

#endif // TWI_H
