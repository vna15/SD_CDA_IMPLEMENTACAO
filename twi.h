#include <avr\io.h>

void config_twi();
void iniciar_twi();
void parar_twi(void);
void escrita_twi(uint8_t data);
uint8_t leitura_twi(uint8_t ack_dado);

void config_twi () {
	DDRC = (1 << 4) | (1 << 5);
	TWSR=0x01;
	TWBR=18;
	TWCR=0x04;
}
void iniciar_twi() {
	TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
	while (!(TWCR & (1<<TWINT)))
}
void parar_twi() {
	TWCR = ((1<< TWINT) | (1<<TWEN) | (1<<TWSTO));
	delay(100) ;
}

void escrita_twi(uint8_t dado) {
	TWDR = dado;
	TWCR = ((1<< TWINT) | (1<<TWEN));
	while (!(TWCR & (1 <<TWINT)));
}
uint8_t leitura_twi(uint8_t ack_dado) {
	TWCR = ((1<< TWINT) | (1<<TWEN) | (v_ackOption_u8<<TWEA));
	while ( !(TWCR & (1 <<TWINT)));
	return TWDR;
}


