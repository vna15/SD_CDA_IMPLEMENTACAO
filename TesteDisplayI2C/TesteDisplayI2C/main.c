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

#define desativado 0
#define ativado 1
#define programacao 2
#define recuperacao 3
#define panico 4
#define p_alterar_senha 5
#define p_sensor_config 6
#define p_sensor_zona 7
#define p_sel_zona 8
#define p_tempo_ativacao 9
#define p_tempo_timeout 10
#define p_tempo_sirene 11
#define p_ativar 12
#define p_desativar 13

#define b_1 2
#define b_2 6
#define b_3 10
#define b_P 12
#define b_4 1
#define b_5 5
#define b_6 9
#define b_A 13
#define b_7 0
#define b_8 4
#define b_9 8
#define b_D 14
#define b_R 3
#define b_0 7
#define b_S 11
#define b_E 15
#define b_invalida 255//utilizada quando não se está precionando qualquer tecla

int8_t estado=desativado;
int8_t teclado=b_invalida;

uint8_t reg_sensores_ativados = 0b11111111;

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
	        case desativado:
			
				FlagSenha = false;
				switch(teclado){
					case b_A:
						//position0();
						clearDisplay();
						writeScreen("Des->Ativado!");
						_delay_ms(10);
						estado = ativado;
						pressionado = false;
					break;
					case b_P:
						position0();
						writeScreen("Des->Programar!");
						_delay_ms(10);
						estado = programacao;
						pressionado = false;
					break;
					case b_R:
						position0();
						writeScreen("Des->Recuperar!");
						_delay_ms(10);
						estado = recuperacao;
						pressionado = false;
					break;
					case 100:
						//Verificar senha mestre
						insere_verifica_senhas(true);
					break;
					default:
						position0();
						writeScreen("  Desativado!  \nDigite o comando");
						pressionado = false;
					break;
	        }
	        break;
	        case ativado:	

				switch(teclado){
					case b_D:
						position0();
						writeScreen("Atv-Desativar!");
						estado = desativado;
						pressionado = false;
						teclado = 100;
					break;
					case b_R:
						position0();
						writeScreen("Atv-Recuperar!");
						estado = recuperacao;
						pressionado = false;		       
					break;
					case b_S :
						position0();
						writeScreen("Atv->Panico!");
						_delay_ms(500);					
						estado = panico;	
						teclado = b_invalida;
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
	        case programacao:
				clearDisplay();
				writeScreen("Inicio   \n Prog");
				switch (teclado){
					case b_R:
						position0();
						writeScreen("Prog->Desativar!");
						estado = desativado;
						teclado = b_invalida;	
						pressionado = false;				
					break;
					case b_S:
						position0();
						writeScreen("Prog->Panico!");
						estado = panico;	
						pressionado = false;				
					break;
					case(b_A):
						estado=p_ativar;
					break;
					case(b_D):
						estado=p_desativar;
					break;
				}
	        break;
	        case recuperacao:
					position0();
					writeScreen("Recuperando...");
					pressionado = false;
	        break;
	        case panico:
				switch (teclado){
					case b_S:
						position0();
						writeScreen("Pan->Desativado!");
						PORTD &= ~(1 << DDD7);
						estado = desativado;	
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
			case p_ativar:
				clearDisplay();
				writeScreen("Selecione opcao ");
				_delay_ms(100);
				switch (teclado){
					case(b_2):
						estado=p_alterar_senha;
					break;
					case(b_3):
						estado=p_sensor_config;
						ativar=true;
					break;
					case(b_4):
						estado=p_sensor_zona;
						ativar=true;
					break;
					case(b_5):
						estado=p_sel_zona;
						ativar=true;
					break;
					case(b_6):
						estado=p_tempo_ativacao;
					break;
					case(b_7):
						estado=p_tempo_timeout;
					break;
					case(b_8):
						estado=p_tempo_sirene;
					break;
					/*
					case(b_R):
					//caso_R();
					break;
					case(b_S):
					//caso_S();
					break;
					*/
					default:
						//estado = p_ativar;
					break;
					//verificar_timeout();
			}
			case p_desativar:
			switch (teclado){
				case(b_3):
				estado=p_sensor_config;
				ativar=false;
				break;
				case(b_4):
				estado=p_sensor_zona;
				ativar=false;
				break;
				case(b_5):
				estado=p_sel_zona;
				ativar=false;
				break;
				case(b_R):
				//caso_R();
				break;
				case(b_S):
				//caso_S();
				break;
				default:
				break;
				//verificar_timeout();
			}

			case p_alterar_senha :
				clearDisplay();
				writeScreen("Selecione senha:\n");
				_delay_ms(100);
				switch (teclado){
					case(b_0):
					alterar_senha(0);
					break;
					case(b_1):
					alterar_senha(1);
					break;
					case(b_2):
					alterar_senha(2);
					break;
					case(b_3):
					alterar_senha(3);
					break;
					case(b_R):
					//caso_R();
					break;
					case(b_S):
					//caso_S();
					break;
					default:
					estado = p_alterar_senha;
					break;
			}
			break;
			case p_sensor_config :
			switch (teclado){
				case(b_R):
				//caso_R();
				break;
				case(b_S):
				//caso_S();
				break;
				default:
				break;
				//verificar_timeout();
			}
			break;
			case p_sensor_zona :
			break;
			case p_sel_zona :
			break;
			case p_tempo_ativacao :
			break;
			case p_tempo_timeout :
			break;
			case p_tempo_sirene :
			break;
        }
	   
    }
}

void verifica_sensores_ativos(){
	//reg_sensores_ativados:uint8_t;contendo bit a bit entre os 8 sensores estão ligados
	
		clearDisplay();
		writeScreen("Verificando \nSensores...");
		_delay_ms(1000);
	
	if (PINB != 0b11111111)//se alguma das portas dos sensores foram ativadas
	{
		if ((reg_sensores_ativados & PINB) == (PINB & 0b11111110)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 01");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
			};
		if ((reg_sensores_ativados & PINB) == (PINB & 0b11111101)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 02");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_ativados & PINB) == (PINB & 0b11111011)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 03");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_ativados & PINB) == (PINB & 0b11110111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 04");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_ativados & PINB) == (PINB & 0b11101111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 05");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_ativados & PINB) == (PINB & 0b11011111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 06");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_ativados & PINB) == (PINB & 0b10111111)){
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 07");
			PORTD |= (1 << DDD7);
			_delay_ms(1000);
		}
		if ((reg_sensores_ativados & PINB) == (PINB & 0b01111111)){
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
				case(b_0):
				ver_senha[index]=0;
				writeScreen("0");
				break;
				case(b_1):
				ver_senha[index]=1;
				writeScreen("1");
				break;
				case(b_2):
				ver_senha[index]=2;
				writeScreen("2");
				break;
				case(b_3):
				ver_senha[index]=3;
				writeScreen("3");
				break;
				case(b_4):
				ver_senha[index]=4;
				writeScreen("4");
				break;
				case(b_5):
				ver_senha[index]=5;
				writeScreen("5");
				break;
				case(b_6):
				ver_senha[index]=6;
				writeScreen("6");
				break;
				case(b_7):
				ver_senha[index]=7;
				writeScreen("7");
				break;
				case(b_8):
				ver_senha[index]=8;
				writeScreen("8");
				break;
				case(b_9):
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
	while(teclado != b_E){
		int MCDU = 1;
		resposta=0;
		int i = 0;
				for (i=3; i >=0; --i){
					resposta=resposta+ver_senha[i]*MCDU;
					MCDU=MCDU*10;
				}
				
		/*
		switch (teclado){
			case b_E:
				for (i=3; i >=0; --i){
					resposta=resposta+ver_senha[i]*MCDU;
					MCDU=MCDU*10;
				}
				return resposta;
			break;
			case(b_R):
				//caso_R();
				return -1;
				break;
			case(b_S):
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
			case(b_E):
			senhas[index]=resposta;
			break;
			case(b_R):
			//caso_R();
			break;
			case(b_S):
			//caso_S();
			break;
			default:
			//delay(1);//apenas convencionado
			break;
		}
	}
	senha_aberta[index]=true;
}
