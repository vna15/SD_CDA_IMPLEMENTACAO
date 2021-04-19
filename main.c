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
#include "LEDS.h"
//#include <util/delay.h>
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
#define p_sensor_desativar 14

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

uint8_t reg_sensores_ativados[8] = {0,0,0,0,0,0,0,0};
uint8_t zona1[9] = {0,0,0,0,0,0,0,0,0};
uint8_t zona2[9] = {0,0,0,0,0,0,0,0,0};
uint8_t zona3[9] = {0,0,0,0,0,0,0,0,0};

bool pressionado = false;
bool flagSensorZona = false;
bool flagZona = true;

int senhas[4]={1234,-1,-1,-1};
	
int resposta;

//*********FUNÇÕES***********
void verifica_sensores_ativos();
bool insere_verifica_senhas(bool mestre, bool EorNot);
int inserir_senha(bool EorNot);

//Interrupção Exerterna INT0
ISR(INT0_vect){
	teclado = ((PIND & 120) >> 3);
	pressionado = true;
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
	
	//bool varSenha = false;
	//estado = p_sel_zona;
	
   while(1)
   {	
        switch (estado){
	        case desativado:
				
				switch(teclado){
					case b_A:
						clearDisplay();
						position0();
						writeScreen("Des->Ativado!");
						_delay_ms(500);
						insere_verifica_senhas(false, true);
						pressionado = false;
					break;
					case b_P:
						clearDisplay();
						position0();
						writeScreen("Des->Programar!");
						_delay_ms(500);
						insere_verifica_senhas(true, false);
						pressionado = false;
					break;
					case b_R:
						clearDisplay();
						position0();
						writeScreen("Des->Recuperar!");
						_delay_ms(500);
						estado = recuperacao;
						pressionado = false;
					break;
					case 101:
						estado = programacao;
					break;
					case 102:
						teclado = 255;
						estado = ativado;
					break;
					default:
						position0();
						writeScreen("  Desativado!  \nDigite o comando");
						_delay_ms(5);
						pressionado = false;
					break;
	        }
	        break;
	        case ativado:	
				switch(teclado){
					case b_D:
						clearDisplay();
						insere_verifica_senhas(false, true);
						pressionado = false;
					break;
					case b_R:
						clearDisplay();
						estado = recuperacao;
						pressionado = false;		       
					break;
					case b_S :
						clearDisplay();
						position0();
						writeScreen("Atv->Panico!");
						_delay_ms(500);					
						estado = panico;	
						teclado = b_invalida;
						pressionado = false;	       
					break;
					case 102:
						teclado = 255;
						estado = desativado;
					break;
					default:
						verifica_sensores_ativos();
						_delay_ms(100);
					break;
	        }
	        break;
	        case programacao:
				switch (teclado){
					case b_R:
						clearDisplay();
						writeScreen("Prog->Desativar!");
						estado = desativado;
						teclado = b_invalida;	
						pressionado = false;				
					break;
					case b_S:
						clearDisplay();
						writeScreen("Prog->Panico!");
						estado = panico;	
						pressionado = false;				
					break;
					case(b_A):
						clearDisplay();
						teclado = 255;
						estado = p_ativar;
					break;
					case(b_D):
						clearDisplay();
						teclado = 255;
						estado = p_desativar;
					break;
					default:
					position0();
					writeScreen("Inicio prog\nA->Atv / B->Des");
					_delay_ms(300);
					break;
				}
	        break;
	        case recuperacao:
					clearDisplay();
					writeScreen("Recuperando...");
					pressionado = false;
	        break;
	        case panico:
				switch (teclado){
					case b_S:
						clearDisplay();
						writeScreen("Pan->Desativado!");
						PORTD &= ~(1 << DDD7);
						estado = desativado;	
						pressionado = false;				
					break;			
					default:
						clearDisplay();
						writeScreen("Alarme Acionado!");
						PORTD |= (1 << DDD7);
						pressionado = false;
					break;
				}
	        break;
			case p_ativar:
				switch (teclado){
					case(b_2):
						teclado = 255;
						estado=p_alterar_senha;
						clearDisplay();
					break;
					case(b_3):
						teclado = 255;
						estado=p_sensor_config;
						clearDisplay();
					break;
					case(b_4):
						teclado = 255;
						flagSensorZona = true;
						estado=p_sensor_zona;
						clearDisplay();
					break;
					case(b_5):
						teclado = 255;
						flagZona = true;
						estado=p_sel_zona;
						clearDisplay();
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
					default:
					position0();
					writeScreen("Selecione opcao:\n2/3/4/5 ");
					_delay_ms(100);
					break;
			}
			break;
			case p_desativar:
				switch (teclado){
					case(b_3):
					teclado = 255;
					estado = p_sensor_desativar;
					break;
					case(b_4):
					teclado = 255;
					flagSensorZona = false;
					estado=p_sensor_zona;
					break;
					case(b_5):
					flagZona = false;
					estado=p_sel_zona;
					break;
					default:
					clearDisplay();
					writeScreen("Selecione opcao ");
					_delay_ms(100);
					break;
			}
			break;

			case p_alterar_senha :
				switch (teclado){
					case(b_1):
					clearDisplay();
					teclado = 255;
					alterar_senha(1);
					estado = desativado;
					break;
					case(b_2):
					clearDisplay();
					alterar_senha(2);
					break;
					case(b_3):
					clearDisplay();
					alterar_senha(3);
					default:
					position0();
					writeScreen("Alterar senha:\n1/2/3");
					_delay_ms(100);
					break;
			}
			break;
			case p_sensor_config :
				switch(teclado){
					case b_0:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '0'   ");
					_delay_ms(250);
					reg_sensores_ativados[0] = 1;
					waitE();
					break;
					case b_1:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '1'   ");
					_delay_ms(250);
					reg_sensores_ativados[1] = 1;
					waitE();
					break;
					case b_2:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '2'   ");
					_delay_ms(250);
					reg_sensores_ativados[2] = 1;
					waitE();
					break;
					case b_3:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '3'   ");
					_delay_ms(250);
					reg_sensores_ativados[3] = 1;
					waitE();
					break;
					case b_4:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '4'   ");
					_delay_ms(250);
					reg_sensores_ativados[4] = 1;
					waitE();
					break;
					case b_5:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '5'   ");
					_delay_ms(250);
					_delay_ms(100);
					reg_sensores_ativados[5] = 1;
					waitE();
					break;
					case b_6:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '6'   ");
					_delay_ms(250);
					reg_sensores_ativados[6] = 1;
					waitE();
					break;
					case b_7:
					position0();
					writeScreen("   Habilitado!  \n   Sensor '7'   ");
					_delay_ms(250);
					reg_sensores_ativados[7] = 1;
					waitE();
					break;
					case 103:
						clearDisplay();
						//position0;
						writeScreen("   Sensor  \n  habilitado! ");
						_delay_ms(300);
						clearDisplay();
						teclado = 255;
						estado = desativado;
					break;
					default:
					position0();
					writeScreen("Habilitar Sensor\n0/1/2/3/4/5/6/7");
					_delay_ms(300);
					break;
			}
			break;
			
			case p_sensor_desativar:
			switch(teclado){
				case b_0:
				clearDisplay();
				writeScreen("0");
				_delay_ms(100);
				reg_sensores_ativados[0] = 0;
				waitE();
				break;
				case b_1:
				clearDisplay();
				writeScreen("1");
				_delay_ms(100);
				reg_sensores_ativados[1] = 0;
				waitE();
				break;
				case b_2:
				reg_sensores_ativados[2] = 0;
				waitE();
				break;
				case b_3:
				reg_sensores_ativados[3] = 0;
				waitE();
				break;
				case b_4:
				reg_sensores_ativados[4] = 0;
				waitE();
				break;
				case b_5:
				clearDisplay();
				writeScreen("5");
				_delay_ms(100);
				reg_sensores_ativados[5] = 0;
				waitE();
				case b_6:
				reg_sensores_ativados[6] = 0;
				waitE();
				break;
				case b_7:
				reg_sensores_ativados[7] = 0;
				waitE();
				break;
				case 103:
				clearDisplay();
				writeScreen("Sensor Desabilitado");
				_delay_ms(100);
				teclado = 255;
				estado = desativado;
				break;
				default:
				clearDisplay();
				writeScreen("Desabilitar Sensor");
				_delay_ms(10);
				break;
			}
			break;

			case p_sensor_zona:
				switch(teclado){
					case b_E:
						clearDisplay();
						writeScreen("Sensor na Zona");
						_delay_ms(100);
						estado = desativado;
					break;
					case b_0:
						clearDisplay();
						writeScreen("'0' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '0'->\n-> Zona '1'");
									_delay_ms(200);
									zona1[0] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '0' X\n X Zona '1'");
									_delay_ms(200);
									zona1[0] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '0'->\n-> Zona '2'");
									_delay_ms(200);
									zona2[0] = 1;
									}else{
									clearDisplay();
									writeScreen(" Sensor '0' X\n X Zona '2'");
									_delay_ms(200);
									zona2[0] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '0'->\n-> Zona '3'");
									_delay_ms(200);
									zona3[0] = 1;
									}else{
									clearDisplay();
									writeScreen(" Sensor '0' X\n X Zona '3'");
									_delay_ms(200);
									zona3[0] = 0;
								}
							}
							}while(teclado!=b_E);
					break;
					case b_1:
						clearDisplay();
						writeScreen("'1' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '1'->\n-> Zona '1'");
									_delay_ms(200);									
									zona1[1] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '1' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[1] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '1'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[1] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '1' X\n X Zona '2'");
									_delay_ms(200);
									zona2[1] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '1'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[1] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '1' X\n X Zona '3'");
									_delay_ms(200);									
									zona3[1] = 0;
								}
							}
							}while(teclado!=b_E);
					break;
					case b_2:
						clearDisplay();
						writeScreen("'2' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '2'->\n-> Zona '1'");
									_delay_ms(200);									
									zona1[2] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '2' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[2] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '2'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[2] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '2' X\n X Zona '2'");
									_delay_ms(200);									
									zona2[2] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '2'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[2] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '2' X\n X Zona '2'");
									_delay_ms(200);									
									zona3[2] = 0;
								}
							}
							}while(teclado!=b_E);
					break;
					case b_3:
						clearDisplay();
						writeScreen("'3' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '3'->\n-> Zona '1'");
									_delay_ms(200);
									zona1[3] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '3' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[3] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '3'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[3] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '3' X\n X Zona '2'");
									_delay_ms(200);									
									zona2[3] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '3'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[3] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '3' X\n X Zona '3'");
									_delay_ms(200);									
									zona3[3] = 0;
								}
							}
							}while(teclado!=b_E);
					break;
					case b_4:		
						clearDisplay();
						writeScreen("'4' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '4'->\n-> Zona '1'");
									_delay_ms(200);									
									zona1[4] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '4' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[4] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '4'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[4] = 1;
									}else{
									clearDisplay();
									writeScreen(" Sensor '4' X\n X Zona '2'");
									_delay_ms(200);										
									zona2[4] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '4'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[4] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '4' X\n X Zona '3'");
									_delay_ms(200);									
									zona3[4] = 0;
								}
							}
							}while(teclado!=b_E);
					break;
					
					case b_5:
						clearDisplay();
						writeScreen("'5' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '5'->\n-> Zona '1'");
									_delay_ms(200);									
									zona1[5] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '5' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[5] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '5'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[5] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '5' X\n X Zona '2'");
									_delay_ms(200);									
									zona2[5] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '5'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[5] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '5' X\n X Zona '3'");
									_delay_ms(200);									
									zona3[5] = 0;
								}
							}
							}while(teclado!=b_E);
					break;					
					case b_6:
						clearDisplay();
						writeScreen("'6' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '6'->\n-> Zona '1'");
									_delay_ms(200);									
									zona1[6] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '6' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[6] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '6'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[6] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '6' X\n X Zona '2'");
									_delay_ms(200);									
									zona2[6] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '6'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[6] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '6' X\n X Zona '3'");
									_delay_ms(200);										
									zona3[6] = 0;
								}
							}
							}while(teclado!=b_E);
					break;	
							
					case b_7:
						clearDisplay();
						writeScreen("'7' Selecionado!");
						_delay_ms(200);
						clearDisplay();
						writeScreen("Selecione zona\n1/2/3");
						_delay_ms(200);
						teclado = 255;
						do{
							if(teclado==b_1){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '7'->\n-> Zona '1'");
									_delay_ms(200);									
									zona1[7] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '7' X\n X Zona '1'");
									_delay_ms(200);									
									zona1[7] = 0;
								}
							}
							if(teclado==b_2){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '7'->\n-> Zona '2'");
									_delay_ms(200);									
									zona2[7] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '7' X\n X Zona '2'");
									_delay_ms(200);									
									zona2[7] = 0;
								}
							}
							if(teclado==b_3){
								if(flagSensorZona == true){
									clearDisplay();
									writeScreen(" Sensor '7'->\n-> Zona '3'");
									_delay_ms(200);									
									zona3[7] = 1;
								}else{
									clearDisplay();
									writeScreen(" Sensor '7' X\n X Zona '3'");
									_delay_ms(200);									
									zona3[7] = 0;
								}
							}
							}while(teclado!=b_E);
					break;
						
					default:
						position0();
						writeScreen("Selecione sensor");
						_delay_ms(10);
					break;
			}
			break;
			
			case p_sel_zona :
				switch(teclado){
					case b_1:
						clearDisplay();
						writeScreen("zona 01");
						_delay_ms(100);
						if(flagZona == true){
							zona1[9] = 1;
						}
						else{
							zona1[9] = 0;
						}
						waitE();
					break;
					case b_2:
						clearDisplay();
						writeScreen("zona 02");
						_delay_ms(100);
						if(flagZona == true){
							zona2[9] = 1;
						}
						else{
							zona2[9] = 0;
						}
						waitE();
					break;
					case b_3:
						clearDisplay();
						writeScreen("zona 03");
						_delay_ms(100);
						if(flagZona == true){
							zona3[9] = 1;
						}
						else{
							zona3[9] = 0;
						}
						waitE();						
					break;
					case 103:
						estado = desativado;
					break;
					default:
						clearDisplay();
						writeScreen("Selecione zona");
						_delay_ms(100);
			
		     		break;
				}
			break;
			/*
			case p_tempo_ativacao :
			break;
			case p_tempo_timeout :
			break;
			case p_tempo_sirene :
			break;*/
        }
	   
    }
}

void verifica_sensores_ativos(){
	//reg_sensores_ativados:uint8_t;contendo bit a bit entre os 8 sensores estão ligados
	int zonaLed = 0;
	
	if (PINB != 0b11111111)//se alguma das portas dos sensores foram ativadas
	{
		if ((reg_sensores_ativados[0] == 1) & (PINB == (PINB & 0b11111110))){
			zonaLed = whichZone(0);
			acenderLed(zonaLed);
			clearDisplay();
			writeScreen("Alarme 01");
			//PORTD |= (1 << DDD7);
			_delay_ms(100);
			};
		if ((reg_sensores_ativados[1] == 1) & (PINB == (PINB & 0b11111101))){
			zonaLed = whichZone(1);
			acenderLed(zonaLed);
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 02");
			//PORTD |= (1 << DDD7);
			_delay_ms(100);
		}
		if ((reg_sensores_ativados[2] == 1) & (PINB == (PINB & 0b11111011))){
			zonaLed = whichZone(2);
			acenderLed(zonaLed);
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 03");
			//PORTD |= (1 << DDD7);
			_delay_ms(100);
		}
		if ((reg_sensores_ativados[3] == 1) & (PINB == (PINB & 0b11110111))){
			zonaLed = whichZone(3);
			acenderLed(zonaLed);
			clearDisplay();
			writeScreen("Alarme Acionado!\n Setor: 04");
			//PORTD |= (1 << DDD7);
			_delay_ms(100);
		}
		if ((reg_sensores_ativados[4] == 1) & (PINB == (PINB & 0b11101111))){
			zonaLed = whichZone(4);
			acenderLed(zonaLed);
			//clearDisplay();
			//writeScreen("Alarme Acionado!\n Setor: 05");
			//PORTD |= (1 << DDD7);
			_delay_ms(10);
		}
		if ((reg_sensores_ativados[5] == 1) & (PINB == (PINB & 0b11011111))){
			zonaLed = whichZone(5);
			acenderLed(zonaLed);
			//clearDisplay();
			//writeScreen("Alarme Acionado!\n Setor: 06");
			//PORTD |= (1 << DDD7);
			_delay_ms(10);
		}
		if ((reg_sensores_ativados[6] == 1) & (PINB == (PINB & 0b10111111))){
			zonaLed = whichZone(6);
			acenderLed(zonaLed);
			//clearDisplay();
			//writeScreen("Alarme Acionado!\n Setor: 07");
			//PORTD |= (1 << DDD7);
			_delay_ms(100);
		}
		if ((reg_sensores_ativados[7] == 1) & (PINB == (PINB & 0b01111111))){
			zonaLed = whichZone(7);
			acenderLed(zonaLed);
			//clearDisplay();
			//writeScreen("Alarme Acionado!\n Setor: 08");
			//PORTD |= (1 << DDD7);
			_delay_ms(10);
		}
	}
		else{
			position0();
			writeScreen("Verificando \nSensores...");
			_delay_ms(100);
		}
	
}

bool insere_verifica_senhas(bool mestre, bool EorNot){
	clearDisplay();
	position0();
	writeScreen("INSIRA SENHA:\n");
	_delay_ms(150);
	resposta=inserir_senha(EorNot);
	if(resposta>0){
		if(!mestre){
			for (uint8_t i = 1; i < 4; ++i){
					if(resposta==senhas[i]){
						position0();
						writeScreen("\nSENHA CORRETA");
						_delay_ms(500);
						clearDisplay();
						teclado = 102;
						return true;
					}
					else{
						position0();
						writeScreen("\nSENHA INCORRETA");
						_delay_ms(500);
						clearDisplay();
						return false;
				 }
			}
		}
		else{
			if(senhas[0]==resposta){
				clearDisplay();
				position0();
				writeScreen("SENHA MESTRE\n CORRETA!");
				_delay_ms(500);
				clearDisplay();
				teclado = 101;
				return true;
				//criar_log_UART("usuario mestre")
			}
			else{
				clearDisplay();
				position0();
				writeScreen("SENHA MESTRE\n INCORRETA!");
				_delay_ms(500);
				clearDisplay();
				return false;
			}
		}
	}
}

int inserir_senha(bool EorNot){
	uint8_t ver_senha[4]; //para conversão da mensagem do teclado para numeral
	uint8_t index=0;
	while(index!=4){
		if(pressionado){
			//contador=0;
			pressionado=false;
			switch(teclado){
				case(b_0):
				ver_senha[index]=0;
				break;
				case(b_1):
				ver_senha[index]=1;
				break;
				case(b_2):
				ver_senha[index]=2;
				break;
				case(b_3):
				ver_senha[index]=3;
				break;
				case(b_4):
				ver_senha[index]=4;
				break;
				case(b_5):
				ver_senha[index]=5;
				break;
				case(b_6):
				ver_senha[index]=6;
				break;
				case(b_7):
				ver_senha[index]=7;
				break;
				case(b_8):
				ver_senha[index]=8;
				break;
				case(b_9):
				ver_senha[index]=9;
				break;
				default:
				return -1;
			}
			index++;
			writeScreen("*");
		}
	}
	if(EorNot == true){
	while(teclado != b_E){
		int MCDU = 1;
		resposta=0;
		int i = 0;
				for (i=3; i >=0; --i){
					resposta=resposta+ver_senha[i]*MCDU;
					MCDU=MCDU*10;
				}
	}
	}
	else{
		int MCDU = 1;
		resposta=0;
		int i = 0;
		for (i=3; i >=0; --i){
			resposta=resposta+ver_senha[i]*MCDU;
			MCDU=MCDU*10;
		}
	}
	return resposta;
}

void alterar_senha(uint8_t index){
	position0();
	writeScreen("Digite a senha:\n");
	_delay_ms(100);
	int senha_aux=-1;
	while(senha_aux<0){
		senha_aux=inserir_senha(true);
	}
	senhas[index]=senha_aux;
	clearDisplay();
}

void waitE(){
	switch(teclado){
		case b_E:
			teclado = 103;
		break;
	}
}

int whichZone(int pose){
		if(zona1[pose]==1){
			if(zona1[9] == 1){
				clearDisplay();
				position0();
				writeScreen("Alarme Acionado!\n   Zona 1  ");
				_delay_ms(1000);
				PORTD |= (1 << DDD7);
				return 3;
			}

		}
		else if(zona2[pose]==1){
			if(zona2[9] == 1){
				clearDisplay();
				position0();
				writeScreen("Alarme Acionado!\n   Zona 2  ");
				_delay_ms(1000);
				PORTD |= (1 << DDD7);
				return 4;
			}
		}
		else if(zona3[pose]==1){
			if(zona3[9] == 1){
				clearDisplay();
				position0();
				writeScreen("Alarme Acionado!\n   Zona 2  ");
				_delay_ms(1000);
				PORTD |= (1 << DDD7);
				return 5;
			}
		}
		else{
			return 1;
		}
}

void selZonaSensor0(){
	teclado = 255;
	switch(teclado){
		case b_1:
		teclado = 255;
		clearDisplay();
		writeScreen("1");
		_delay_ms(100);
		//if(flagSensorZona == true){
			zona1[0] = 1;
			//}else{
			//zona1[0] = 0;
		//}
		waitE();
		break;
		case b_2:
		if(flagSensorZona == true){
			zona2[0] = 1;
			}else{
			zona2[0] = 0;
		}
		waitE();
		break;
		case b_3:
		if(flagSensorZona == true){
			zona3[0] = 1;
			}else{
			zona3[0] = 0;
		}
		waitE();
		break;
	}
}
