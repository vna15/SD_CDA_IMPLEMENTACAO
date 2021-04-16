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

-teste10():não implementada
	def:temporizador para verificar se a tecla foi precionada por mais de 10 segundos, retorna true imediatamente após 10 segundos, false tecla fo solta antes.
	obs: pode ser implementada com base em funções mais simples de verificação de teclas ou interrupções.


VARIÁVEIS:
-teclado:char; registrador salvo com o ultimo botão pressionado da interrupção do teclado.
-
-estado:int8_t; registra representação da máquina de estados

*/

const uint8_t in_teclado=0b00001111;//portas de entrada do teclado


const int8_t desativado=0;
const int8_t ativado=1;
const int8_t programacao=2;
const int8_t recuperacao=3;
const int8_t panico=4;

const uint8_t b_1=0;
const uint8_t b_2=1;
const uint8_t b_3=2;
const uint8_t b_P=3;
const uint8_t b_4=4;
const uint8_t b_5=5;
const uint8_t b_6=6;
const uint8_t b_A=7;
const uint8_t b_7=8;
const uint8_t b_8=9;
const uint8_t b_9=10;
const uint8_t b_D=11;
const uint8_t b_R=12;
const uint8_t b_0=13;
const uint8_t b_S=14;
const uint8_t b_E=15;
const uint8_t b_invalida=255;//utilizada quando não se está precionando qualquer tecla

int8_t estado=desativado;
int8_t teclado=desativado;

//definir headers de funções


//estados e transições aqui
void main(){
	while(1){
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
  		if(maior) estado=recuperacao;
  	}
  	else estado=desativado;
}

//--------tarefas dos estados--------

void desativado(){

}
void ativado(){
	imprime_display("ATIVADO:");//verificar qual a condição de envio de mensagem
	verifica_sensores_ativos();
}
void programacao(){
	if (permitido)
	{
		/* code */
	}
	else{
		imprime_display("PROGRAMACAO:");
	}
	
	while(teclado!=b_A|teclado!=b_D){

	}

}
void panico(){

}
void recuperacao(){

}
//--------funções auxiliares--------

nonetype? interrupcao_botoes(){ //configurar interrupção na borda de subida
	if (teclado==(PORTD&in_teclado)){
		if(contador>=1225)//valor numerado de 10 segundos
	}
	PORTD&in_teclado
}

bool insere_verifica_senhas(){
	senha[0]="1234"
	senha[4]=
	carro_senha=1<<carro_senha;
}
