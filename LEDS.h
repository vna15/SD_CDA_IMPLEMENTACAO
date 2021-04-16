#ifndef LEDS_H
#define LEDS_H
#include <avr/io.h>
#include "TWI.h"

/*endereço I2C = 0x42
  A0 = HIGH
  A1 e A2 EM LOW
*/
/* LED A -> P7 -> 1
   LED P -> P6 -> 2
   LED Z1 -> P5 -> 3
   LED Z2 -> P4  -> 4
   LED Z3 -> P3 -> 5
   Qualquer outro valor apagará os leds*/

void acenderLed(int num){
	uint8_t led = 0;
	if(num==1){
		led = 128;
	}
	else if(num==2){
		led = 64;
	}
	else if(num==3){
		led = 32;
	}
	else if(num==4){
		led = 16;
	}
	else if(num==5){
		led = 8;
	}
	else{
		led = 0;
	}
	
	Start();
	AddrEscravo(0x42);
	carregaMSG(led);
	Stop();
}


#endif // LEDS_H