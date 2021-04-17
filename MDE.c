/*
Criador:Hilo de Oliveira Goes
Contribuidores:
Data: 12/04/21
Descrição:
  "Código para simulação de maquina de estados e execução de rotina de execução
  do projeto da semana 11 de Sistemas digitais"

OBSERVAÇÕES:
@@ btn S deve ser botão seguro, pois é o unico caso em que pode haver loop indefinido

FUNÇÕES:
-espera_tempo():não implementada: 
	def:função para estabelecer um contador que consumirá em segundos a quantidade de tempo **configurada pelo usuário**
	obs:pode ser implementada com parte do código que irá passar o tempo, levando como parâmetro o uint8_t em segundos.
	obs2:talvez desnecessário, verificar necessidade, ou composição desta lógica em outras funções mais complexas.

-insere_verifica_senhas();insere_verifica_senha_mestre(): não implementados
	def:função responsável por esperar a inserção de senha até que haja timeout, caso a senha seja incorreta mostrar em display, dá tempo para usuário visualizar e retorna false, se a senha corresponde as senhas configuradas retorna true.

-verifica timeout(); inicia timers e verifica se ja houve estouro do tempo especificado; 

-verificar_sensores_ativos():não implementada
	def:checa portB se houve alteração, direciona acionamento de leds

VARIÁVEIS:
-teclado:uint9_t; 	registrador salvo com o ultimo botão pressionado da interrupção do teclado.
-contador:uint9_t; 	armazena os valores de vezes que o contador passou em modo de estouro (normal).
-estado:int8_t; 	registra representação da máquina de estados.

-pressionado:bool; 	indica que um botão foi pressionado.
-flag10:bool;		indica se o botão foi pressionado por mais de 10 segundos.

CONCEITOS:
-portDA;	pino do portD que está ligado a interrupção do MM74C922.
-b_*;		flags de botões.

TIMERS:
TIMER_1:16 BITS - contador de timeout
TIMER_0:8 BITS  - contador normal


*/

const uint8_t in_teclado=0b00111100;//PORT D(2-5)


const uint8_t desativado=0;
const uint8_t ativado=1;
const uint8_t programacao=2;
const uint8_t recuperacao=3;
const uint8_t panico=4;

const uint8_t b_1=0;
const uint8_t b_4=1;
const uint8_t b_7=2;
const uint8_t b_R=3;
const uint8_t b_2=4;
const uint8_t b_5=5;
const uint8_t b_8=6;
const uint8_t b_0=7;
const uint8_t b_3=8;
const uint8_t b_6=9;
const uint8_t b_9=10;
const uint8_t b_S=11;
const uint8_t b_P=12;
const uint8_t b_A=13;
const uint8_t b_D=14;
const uint8_t b_E=15;
const uint8_t b_invalida=255;//utilizada quando não se está precionando qualquer tecla

uint8_t estado=desativado;
uint8_t teclado=b_invalida;
uint8_t contador=0;//timer estouro conta ++1 (interrupção)
uint8_t contador_timeout=0

//definir headers de funções


//estados e transições aqui
void main(){
	while(true){
		contador=0;
		switch (estado){
		  case(desativado):
		    switch(teclado){
		      case(b_A):
		        caso_A();
		      break;
		      case(b_P):
		        caso_P();
		      break;
		      case(b_R):
		        caso_R();
		      break;
		      default:
		        desativado();
		    }
		  break;
		  case(ativado):
		    switch(teclado){
		      case(b_D):
		        caso_D();
		      break;
		      case(b_R):
		        caso_R();
		      break;
		      case(b_S):
		        caso_S();
		      break;
		      default:
		        ativado();
		      }
		  break;
		  case(programacao):
		    switch(teclado){
		      case(b_R):
		        caso_R();
		      break;
		      case(b_S):
		        caso_S();
		      break;
		      default:
		        programacao();
		      }
		  break;
		  case(recuperacao):
		    recuperacao();
		    estado=desativado;
		  break;
		  case(panico):
		    switch (teclado){
		      case(b_S):
		        estado=desativado;
		      default:
		          panico();
		      }
		  break;
		}
	}
}
//--------transições dos estados--------

void caso_D(){
	teclado=b_invalida;
	if(insere_verifica_senhas()){
		estado=desativado;
	}
}
void caso_A(){
	teclado=b_invalida;
	if(insere_verifica_senhas()){
		espera_tempo(/*ativação*/);
		estado=ativado;		
	}
}
void caso_P(){
	teclado=b_invalida;
	if(insere_verifica_senha_mestre()){
		estado=programacao;
	}

}
void caso_S(){
	teclado=b_invalida;
	espera_tempo(/*ativação*/);
	estado=panico;
}
void caso_R(){
	teclado=b_invalida;
	if (estado!=recuperacao){
  		if(flag10) estado=recuperacao;
  	}
  	else estado=desativado;
}

//--------tarefas dos estados--------

void desativado(){

}
void ativado(){
	writeScreen("ATIVADO:");//verificar qual a condição de envio de mensagem
	verifica_sensores_ativos();
}


int programacao(){
	writeScreen("PROGRAMACAO:");
	enable_contador_timeout();
	int contador_timeout=0;
	while(true){
		switch(teclado){
			case b_R:
				return 1;
			break;
			case b_S:
				return 1;
			break;
		}
		if(contador_timeout>/*timeout*/){//colocar timer1 para configuração de 1s para setar mais facil a configuração
			return 1;
		}
	}
	disable_contador_timeout();
	estado=desativado;
}
void panico(){

}
void recuperacao(){

}
//--------funções auxiliares--------

void interrupcao_botoes(){ //configurar interrupção na borda de subida
	teclado=PORTD&in_teclado;//flag de qual botão pressionado
	pressionado=true;
	if(PORTD&in_teclado==b_R){
		enable_contador();
		while(portDA==1){
			if(contador>=1225)//valor numerado de 10 segundos
				flag10=true;
		}
		disable_contador();	
	}
}
interr_contador1(){ //contador16bit(timer_1) normal (estouro->interrupção) contador==2^16/(clk*preset)
	//configurar para interrupção a cada segundo
	contador_timeout=contador_timeout+1;
}
interr_contador2(){ //contador8bit(timer_2) normal (estouro->interrupção) contador==2^8/(clk*preset)
	contador=contador+1;
}

bool insere_verifica_senhas(){
	uint8_t ver_senha[4]; //para conversão da mensagem do teclado para numeral
	imprime_display("INSIRA SENHA MESTRE");
	uint8_t i=0;
	enable_contador()
	while(i!=4){
		if(pressionado){
			contador=0;
			pressionado=false;
			switch(teclado){
				case(b_0):
					ver_senha[i]=0;
				break;
				case(b_1):
					ver_senha[i]=1;
				break;
				case(b_2):
					ver_senha[i]=2;
				break;
				case(b_3):
					ver_senha[i]=3;
				break;
				case(b_4):
					ver_senha[i]=4;
				break;
				case(b_5):
					ver_senha[i]=5;
				break;
				case(b_6):
					ver_senha[i]=6;
				break;
				case(b_7):
					ver_senha[i]=7;
				break;
				case(b_8):
					ver_senha[i]=8;
				break;
				case(b_9):
					ver_senha[i]=9;
				break;
				default:
					return false;
			}
		i++;
		writeScreen("*");
		}
	}
	disable_contador();
}
