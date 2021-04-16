/*
Criador:Hilo de Oliveira Goes
Contribuidores:
Data: 12/04/21
Descrição:
  "Código para simulação de maquina de estados e execução de rotina de execução
  do projeto da semana 11 de Sistemas digitais"

OBSERVAÇÕES:
@@ btn S deve ser botão seguro, pois é o unico caso em que pode haver loop indefinido

*/

const uint8_t in_teclado=0b00001111;//portas de entrada do teclado


const int8_t desativado=0;
const int8_t ativado=1;
const int8_t programacao=2;
const int8_t recuperacao=3;
const int8_t panico=4;

<<<<<<< Updated upstream
const int8_t b_1=0;
const int8_t b_2=1;
const int8_t b_3=2;
const int8_t b_P=3;
const int8_t b_4=4;
const int8_t b_5=5;
const int8_t b_6=6;
const int8_t b_A=7;
const int8_t b_7=8;
const int8_t b_8=9;
const int8_t b_9=10;
const int8_t b_D=11;
const int8_t b_R=12;
const int8_t b_0=13;
const int8_t b_S=14;
const int8_t b_E=15;

void inicializacao(){
  estado=desativado;
}
=======
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
>>>>>>> Stashed changes

//definir headers de funções


//estados e transições aqui

switch (estado){
  case(desativado):
    switch(teclado){
      case(b_A):
        case_A();
      break;
      case(b_P):
        case_P();
      break;
      case(b_R):
        case_R();
      break;
      default:
        desativado();
    }
  break;
  case(ativado):
    switch(teclado){
      case(b_D):
        case_D();
      break;
      case(b_R):
        case_R();
      break;
      case(b_S):
        case_S();
      break;
      default:
        ativado();
      }
  break;
  case(programacao):
    switch(teclado){
      case(b_R):
        case_R();
      break;
      case(b_S):
        case_S();
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

//transições dos estados

void caso_D(){
<<<<<<< Updated upstream

}
void caso_A(){

}
void caso_P(){

}
void caso_S(){

}
void caso_R(){
 
  if(teste10()){
    estado=recuperacao;
  }
  else{
    estado=desativado;
  }

=======
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
  		if(teste10()) estado=recuperacao;
  	}
  	else estado=desativado;
>>>>>>> Stashed changes
}

//tarefas dos estados

void desativado(){

}
void ativado(){

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

nonetype? interrupção_botoes(){
	if (teclado==(PORTD&in_teclado)){
		if(contador>=1225)//valor numerado de 10 segundos
	}
	PORTD&in_teclado

	
}

bool insere_verifica_senhas(){

	senha[4]=
	carro_senha=1<<carro_senha;
}
