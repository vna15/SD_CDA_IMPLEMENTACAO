/*
 * main.c
 *
 * Created: 4/16/2021 5:20:47 PM
 *  Author: Pedro
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "TWI.h"



//Interrupção Exerterna INT0
ISR(INT0_vect){
	
	
}

#define desativado 0
#define ativado 1
#define programacao 2
#define recuperacao 3
#define panico 4

#define b_1 0
#define b_2 1
#define b_3 2
#define b_P 3
#define b_4 4
#define b_5 5
#define b_6 6
#define b_A 7
#define b_7 8
#define b_8 9
#define b_9 10
#define b_D 11
#define b_R 12
#define b_0 13
#define b_S 14
#define b_E 15
#define b_invalida 255//utilizada quando não se está precionando qualquer tecla

int8_t estado=desativado;
int8_t teclado=b_invalida;

int main(void)
{
	ConfigTWI();
	setLCD();
	
    // Confg. Interrupção Externa
    EICRA = 0b00000010;		// seta o modo da interrupção externa no pino INT0 como FALLING
    EIMSK = 0b00000001;		// habilita interrupção externa do pino INT0

    /*//Contador - TIMER0
    TCCR0A = 0b00000000;
    TCCR0B = 0b00000011; //prescaler de 64	
	*/

	
    while(1)
    {
        switch (estado){
	        case desativado:
			
				switch(teclado){
					case b_A:
						writeScreen("a!");
						estado = ativado;
					break;
					case b_P:
						writeScreen("Des-Programar!");
						estado = programacao;
					break;
					case b_R:
						writeScreen("Des-Recuperar!");
						estado = recuperacao;
					break;
	        }
	        break;
	        case ativado:	
				switch(teclado){
					case b_D:
						writeScreen("Atv-Desativar!");
						estado = desativado;
					break;
					case b_R:
						writeScreen("Atv-Recuperar!");
						estado = recuperacao;		       
					break;
					case b_S :
						writeScreen("Atv-Panico!");
						estado = panico;		       
					break;
	        }
	        break;
	        case programacao:
				switch (teclado){
					case b_R:
						writeScreen("Prog->Recuperar!");
						estado = programacao;					
					break;
					case b_S:
						writeScreen("Prog->Panico!");
						estado = panico;					
					break;
				}
	        break;
	        case recuperacao:
					writeScreen("Recuperando...");
	        break;
	        case panico:
				switch (teclado){
					case b_S:
						writeScreen("Pan->Desativado!");
						estado = desativado;					
					break;					
					default:
					
					break;
				}
	        break;
        }
    }
}