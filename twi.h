#define F_CPU 16000000
#include <avr\io.h>

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
	delay(10);
	//Envia o dado
    	TWDR = dado;
   	TWCR = ((1<< TWINT) | (1<<TWEN));
   	while (!(TWCR & (1 <<TWINT)));
	delay(10);
}

void Stop(){
	//Envia uma condição de STOP
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	//--------------------------------------------------------
	delay(100);
}

uint8_t read_twi(uint8_t ack_dado) {
	TWCR = ((1<< TWINT) | (1<<TWEN) | (ack_dado<<TWEA));
	while ( !(TWCR & (1 <<TWINT)));
	return TWDR;
}
