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
#include <util\delay.h>
#include <stdbool.h>
/*
#define 0 0
#define 1 1
#define 2 2
#define 3 3
#define 4 4
#define 5 5
#define 6 6
#define 7 7
#define 8 8
#define 9 9
#define 10 10
#define 11 11
#define 12 12
#define 13 13

#define 2 2
#define 6 6
#define 10 10
#define 12 12
#define 1 1
#define 5 5
#define 9 9
#define 13 13
#define 0 0
#define 4 4
#define 8 8
#define 14 14
#define 3 3
#define 7 7
#define 11 11
#define 15 15
#define 255 255//utilizada quando não se está precionando qualquer tecla
*/
int8_t estado=0;
int8_t teclado=255;

uint8_t reg_sensores_1s = 0b11111111;

bool senha_aberta[4] = {true,false,false,false};
bool pressionado = false;
bool FlagSenha = false;
bool ativar=false;//flag de ativar/desativar ..sensores..zona.

int senhas[4]={1234,-1,-1,-1};
	
int resposta;

//*********FUNÇÕES***********
void verifica_sensores_ativos();
bool insere_verifica_senhas(bool mestre);
int inserir_senha();


//Interrupção Exerterna INT0
ISR(INT0_vect){
	teclado = ((PIND & 120) >> 3);
	pressionado = true;
	//PORTD |= (1 << DDD4);	
}

int main(void)
{
	ConfigTWI();
	Start();
	AddrEscravo(0x40);
	setLCD();
	Stop();
	
	
    // Confg. Interrupção Externa
    EICRA = 0b00000010;		// seta o modo da interrupção externa no pino INT0 como FALLING
    EIMSK = 0b00000001;		// habilita interrupção externa do pino INT0

    /*//Contador - TIMER0
    TCCR0A = 0b00000000;
    TCCR0B = 0b00000011; //prescaler de 64	
	*/
	
	DDRB = 0b00000000;
	DDRD = 0b10000000;
	
	PORTB = 0b11111111;
	
	writeScreen("Iniciando...");
	_delay_ms(500);
	
	//Interrupção Global
	sei();
	
   while(1)
   {	
	   
        switch (estado){
	        case 0:
			
				FlagSenha = false;
				switch(teclado){
					case 13:
						//position0();
						clearDisplay();
						writeScreen("Des->1!");
						_delay_ms(10);
						estado = 1;
						pressionado = false;
					break;
					case 12:
						position0();
						writeScreen("Des->Programar!");
						_delay_ms(10);
						estado = 2;
						pressionado = false;
					break;
					case 3:
						position0();
						writeScreen("Des->Recuperar!");
						_delay_ms(10);
						estado = 3;
						pressionado = false;
					break;
					case 100:
						//Verificar senha mestre
						insere_verifica_senhas(true);
					break;
					default:
						position0();
						writeScreen("  0!  \nDigite o comando");
						pressionado = false;
					break;
	        }
	        break;
	        case 1:	

				switch(teclado){
					case 14:
						position0();
						writeScreen("Atv-Desativar!");
						estado = 0;
						pressionado = false;
						teclado = 100;
					break;
					case 3:
						position0();
						writeScreen("Atv-Recuperar!");
						estado = 3;
						pressionado = false;		       
					break;
					case 11 :
						position0();
						writeScreen("Atv->4!");
						_delay_ms(500);					
						estado = 4;	
						teclado = 255;
						pressionado = false;	       
					break;
					default:
					if(FlagSenha == false){
						//Verificar senha mestre
						insere_verifica_senhas(true);
					}
					else{
						//Verificar sensores
						verifica_sensores_ativos();
					}
					break;
	        }
	        break;
	        case 2:
				clearDisplay();
				writeScreen("Inicio   \n Prog");
				switch (teclado){
					case 3:
						position0();
						writeScreen("Prog->Desativar!");
						estado = 0;
						teclado = 255;	
						pressionado = false;				
					break;
					case 11:
						position0();
						writeScreen("Prog->4!");
						estado = 4;	
						pressionado = false;				
					break;
					case(13):
						estado=12;
					break;
					case(14):
						estado=13;
					break;
				}
	        break;
	        case 3:
					position0();
					writeScreen("Recuperando...");
					pressionado = false;
	        break;
	        case 4:
				switch (teclado){
					case 11:
						position0();
						writeScreen("Pan->0!");
						PORTD &= ~(1 << DDD7);
						estado = 0;	
						pressionado = false;				
					break;			
					default:
						position0();
						writeScreen("Alarme Acionado!");
						PORTD |= (1 << DDD7);
						pressionado = false;
					break;
				}
	        break;
			case 12:
				clearDisplay();
				writeScreen("Selecione opcao ");
				_delay_ms(100);
				switch (teclado){
					case(6):
						estado=5;
					break;
					case(10):
						estado=6;
						ativar=true;
					break;
					case(1):
						estado=7;
						ativar=true;
					break;
					case(5):
						estado=8;
						ativar=true;
					break;
					case(9):
						estado=9;
					break;
					case(0):
						estado=10;
					break;
					case(4):
						estado=11;
					break;
					/*
					case(3):
					//caso_R();
					break;
					case(11):
					//caso_S();
					break;
					*/
					default:
					break;
					//verificar_timeout();
			}
			case 13:
				switch (teclado){
					case(10):
					estado=6;
					ativar=false;
					break;
					case(1):
					estado=7;
					ativar=false;
					break;
					case(5):
					estado=8;
					ativar=false;
					break;
					case(3):
					//caso_R();
					break;
					case(11):
					//caso_S();
					break;
					default:
					break;
					//verificar_timeout();
			}

			case 5 :
				clearDisplay();
				writeScreen("Alterar_senha:\n");
				_delay_ms(100);
				switch (teclado){
					case(7):
					alterar_senha(0);
					break;
					case(2):
					alterar_senha(1);
					break;
					case(6):
					alterar_senha(2);
					break;
					case(10):
					alterar_senha(3);
					break;
					case(3):
					//caso_R();
					break;
					case(11):
					//caso_S();
					break;
					default:
					break;
			}
			break;
			case 6 :
			switch (teclado){
				case(3):
				//caso_R();
				break;
				case(11):
				//caso_S();
				break;
				default:
				break;
				//verificar_timeout();
			}
			break;
			case 7 :
			break;
			case 8 :
			break;
			case 9 :
			break;
			case 10 :
			break;
			case 11 :
			break;
        }
	   
    }
}

void verifica_sensores_ativos(){
	//reg_sensores_1s:uint8_t;contendo bit a bit entre os 8 sensores estão ligados
	
		clearDisplay();
		writeScreen("Verificando \nSensores...");
		_delay_ms(1000);
	
	if (PINB != 0b11111111)//se alguma das portas dos sensores foram ativadas
	{
		if ((reg_sensores_1s & PINB) == (PINB & 0b11111110)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 01");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
			};
		if ((reg_sensores_1s & PINB) == (PINB & 0b11111101)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 02");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_1s & PINB) == (PINB & 0b11111011)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 03");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_1s & PINB) == (PINB & 0b11110111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 04");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_1s & PINB) == (PINB & 0b11101111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 05");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_1s & PINB) == (PINB & 0b11011111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 06");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_1s & PINB) == (PINB & 0b10111111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 07");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_1s & PINB) == (PINB & 0b01111111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 08");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
	}
	
}

bool insere_verifica_senhas(bool mestre){
	clearDisplay();
	writeScreen("INSIRA SENHA:\n");
	_delay_ms(10);
	//enable_contador()
	resposta=inserir_senha();
	char vet[20] = {'\0'};
	itoa(resposta, vet,10);
	clearDisplay();
	writeScreen(vet);
	_delay_ms(1000);
	if(resposta>0){
		if(!mestre){
			for (uint8_t i = 1; i < 4; ++i){
				if (senha_aberta[i]){
					if(resposta==senhas[i]){
						//criar_log_UART("usuario x")
						return true;
					}
					else{
						clearDisplay();
						writeScreen("\nSENHA INCORRETA");
						_delay_ms(1000);
						FlagSenha = false;
						return false;
				 }
				 //aqui
				}
			}
		}
		else{
			if(senhas[0]==resposta){
				clearDisplay();
				writeScreen("SENHA MESTRE\n CORRETA!");
				_delay_ms(1000);
				FlagSenha = true;
				//criar_log_UART("usuario mestre")
			}
			else{
				clearDisplay();
				writeScreen("SENHA MESTRE\n INCORRETA!");
				_delay_ms(1000);
				FlagSenha = false;
				return false;
			}
		}
	}
	//disable_contador();
	return true;
	//tem mais//
}

int inserir_senha(){
	uint8_t ver_senha[4]; //para conversão da mensagem do teclado para numeral
	uint8_t index=0;
	while(index!=4){
		if(pressionado){
			//contador=0;
			pressionado=false;
			switch(teclado){
				case(7):
				ver_senha[index]=0;
				writeScreen("0");
				break;
				case(2):
				ver_senha[index]=1;
				writeScreen("1");
				break;
				case(6):
				ver_senha[index]=2;
				writeScreen("2");
				break;
				case(10):
				ver_senha[index]=3;
				writeScreen("3");
				break;
				case(1):
				ver_senha[index]=4;
				writeScreen("4");
				break;
				case(5):
				ver_senha[index]=5;
				writeScreen("5");
				break;
				case(9):
				ver_senha[index]=6;
				writeScreen("6");
				break;
				case(0):
				ver_senha[index]=7;
				writeScreen("7");
				break;
				case(4):
				ver_senha[index]=8;
				writeScreen("8");
				break;
				case(8):
				ver_senha[index]=9;
				writeScreen("9");
				break;
				default:
				return -1;
			}
			index++;
			//writeScreen("*");
		}
	}
	while(teclado != 15){
		int MCDU = 1;
		resposta=0;
		int i = 0;
				for (i=3; i >=0; --i){
					resposta=resposta+ver_senha[i]*MCDU;
					MCDU=MCDU*10;
				}
				
		/*
		switch (teclado){
			case 15:
				for (i=3; i >=0; --i){
					resposta=resposta+ver_senha[i]*MCDU;
					MCDU=MCDU*10;
				}
				return resposta;
			break;
			case(3):
				//caso_R();
				return -1;
				break;
			case(11):
				//caso_S();
				return -1;
			break;
			default:
			break;
		}
	*/	
	}
	return resposta;
}

void alterar_senha(uint8_t index){
	clearDisplay();
	writeScreen("Digite a senha:");
	_delay_ms(100);
	int senha_aux=-1;
	while(senha_aux<0){
		senha_aux=inserir_senha();
	}
	while(!pressionado){
		switch (teclado){
			case(15):
			senhas[index]=resposta;
			break;
			case(3):
			//caso_R();
			break;
			case(11):
			//caso_S();
			break;
			default:
			//delay(1);//apenas convencionado
			break;
		}
	}
	senha_aberta[index]=true;
}
