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
const int8_t desativado=0;
const int8_t ativado=1;
const int8_t programacao=2;
const int8_t recuperacao=3;
const int8_t panico=4;

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

//estados e transições aqui

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

//--------transições dos estados--------

void caso_D(){

}
void caso_A(){
	espera_tempo()//-->para implementar
	estado=ativado;
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

}

//--------tarefas dos estados--------

void desativado(){

}
void ativado(){
	imprime_display("ATIVADO:")//verificar qual a condição de envio de mensagem
	verifica_sensores_ativos()
}
void programacao(){

}
void panico(){

}
void recuperacao(){


}