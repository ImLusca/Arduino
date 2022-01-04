#include <EtherCard.h>
#include "SdFat.h"
#include "sdios.h"

#define TCP_FLAGS_FIN_V 1 
#define TCP_FLAGS_ACK_V 0x10 
#define SH_CP 3
#define ST_CP 4
#define DS 5

#define CS_ENC28J60 10

#define FILTRO 10


//Sensores e relés =======================================

int Sensor0 = 0, Sensor1 = 0, Sensor2 = 0, Sensor3 = 0;
int rele = -1;

int vetMedia0[FILTRO], vetMedia1[FILTRO];
int vetMedia2[FILTRO], vetMedia3[FILTRO];


void LeituraSensores();  
long MediaMovel(int val,int vetMedia[]);
void MudaReles();

byte shiftRegister = 0b10000000;

//Sensores e relés =======================================


//Enc28j60 ===============================================
static byte myip[] = { 192,168,15,7 };
static byte gwip[] = { 192,168,15,1 };
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
byte Ethernet::buffer[750]; // buffer tcp ip 

void olha_tcp();

const char END[] PROGMEM = "Fim da conexao\n";

const char site0[] PROGMEM = "HTTP/1.1 200 OK\r\n Content-Type: text/html\r\n\r\n<!DOCTYPE html><html lang=\"pt-br\"><head> <meta charset=\"UTF-8\"> <title>Menu Config</title> <style>body{height:100vh;justify-content:center;align-items:center;display:flex;font-family:monospace;ba";
const char site1[] PROGMEM = "ckground:linear-gradient(245deg,#7ac5e3ad,#a960e1);overflow:hidden;user-select:none}.frame{display:flex;flex-direction:column;justify-content:center;min-width:20vw;height:40vh;padding:2rem;background:linear-gradient(185deg,#e5e5e55c,#f7f7f794);border";
const char site2[] PROGMEM = "-radius:.4em;backdrop-filter:blur(5px)}.frame input[type=submit]{height:1.7em;min-width:50%;margin-left:auto;border:none;border-radius:6px;background:linear-gradient(243deg,#fffffff5,#f7f7f757);transition:.7s}.principal form{display:none;flex-wrap:wr";
const char site3[] PROGMEM = "ap;justify-content:space-around;align-items:center;font-size:16px;position:absolute;transition:.5s}.cards .frame .bloco input[type=text]{border:0;border-bottom:1px solid #000;font-size:1.2rem;background:none;cursor:text}.opcoes{width:20%;height:100%;";
const char site4[] PROGMEM = "align-items:start;display:flex;flex-direction:column;justify-content:center}.cards .frame .bloco{display:flex;flex-direction:column;width:40%;margin:10px 0;align-items:start}.cards .frame{margin:1vh 1vw;height:80vh;width:56vw;padding:1rem;flex-direct";
const char site5[] PROGMEM = "ion:row}.frame input{margin-bottom:3vh;font-size:1.4rem;cursor:pointer;outline:none}.principal{width:78%;display:flex;align-items:center;position:relative}.opcao p{line-height:2;transition:.7s;user-select:none;cursor:pointer}.frame p{margin:0 0 .4em;";
const char site6[] PROGMEM = "font-weight:400;font-size:1.4rem}.cards .frame input[type=submit]{height:1.4em;min-width:40%}.frame input[type=submit]:hover{transform:scale(0.95)}.cards h1{margin:auto;font-size:2.5em;width:100%}.bloco input[type=submit]{margin-left:0}.opcao p:hover";
const char site7[] PROGMEM = "{transform:scale(0.9)}.principal form.active{display:flex}h1{margin:auto;font-size:2.5em}</style></head><body> <div class=\"cards\"> <div class=\"frame\"> <div class=\"opcoes\"> <div class=\"opcao\"> <p data-option=\"red\" onclick=\"trocaMenu(1)\">Re";
const char site8[] PROGMEM = "de</p> </div> <div class=\"opcao\"> <p data-option=\"s1\" onclick=\"trocaMenu(2)\">Serial 1</p> </div> <div class=\"opcao\"> <p data-option=\"s2\" onclick=\"trocaMenu(3)\">Serial 2</p> </div> </div> <div class=\"principal\"> <form class=\"rede active";
const char site9[] PROGMEM = "\"> <h1>rede</h1> <div class=\"bloco\"> <label for=\"host\">Hostname</label> <input type=\"text\" id=\"host\"> </div> <div class=\"bloco\"> <label for=\"ip\">IP</label> <input type=\"text\" id=\"ip\"> </div> <div class=\"bloco\"> <label for=\"mask\">";
const char site10[] PROGMEM = " Máscara</label> <input type=\"text\" id=\"mask\"> </div> <div class=\"bloco\"> <label for=\"gateway\">Gateway</label> <input type=\"text\" id=\"gateway\"> </div> <div class=\"bloco\"> <label for=\"dns1\">Dns 1</label> <input type=\"text\" id=\"dns1\"";
const char site11[] PROGMEM = "> </div> <div class=\"bloco\"> <label for=\"dns2\">Dns 2</label> <input type=\"text\" id=\"dns2\"> </div> <div class=\"bloco\"> <label for=\"mac\">Mac </label> <input type=\"text\" id=\"mac\"> </div> <div class=\"bloco\"> <input type=\"submit\" value";
const char site12[] PROGMEM = "=\"Confirmar\"> </div> </form> <form class=\"rede\"> <h1>Serial 1</h1> <div class=\"bloco\"> <label for=\"velocidade\">Velocidade</label> <input type=\"text\" id=\"velocidade\"> </div> <div class=\"bloco\"> <label for=\"bits\">Bits de daods</label> <";
const char site13[] PROGMEM = "input type=\"text\" id=\"bits\"> </div> <div class=\"bloco\"> <label for=\"paridade\">paridade</label> <input type=\"text\" id=\"paridade\"> </div> <div class=\"bloco\"> <label for=\"porta\">porta</label> <input type=\"text\" id=\"porta\"> </div> <di";
const char site14[] PROGMEM = "v class=\"bloco\"> <label for=\"protocolo\">protocolo</label> <input type=\"text\" id=\"protocolo\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> <form class=\"rede\"> <h1>Serial 2</h1> <div class=\"bloco\">";
const char site15[] PROGMEM = "<label for=\"velocidade\">Velocidade</label> <input type=\"text\" id=\"velocidade\"> </div> <div class=\"bloco\"> <label for=\"bits\">Bits de daods</label> <input type=\"text\" id=\"bits\"> </div> <div class=\"bloco\"> <label for=\"paridade\">parida";
const char site16[] PROGMEM = "de</label> <input type=\"text\" id=\"paridade\"> </div> <div class=\"bloco\"> <label for=\"porta\">porta</label> <input type=\"text\" id=\"porta\"> </div> <div class=\"bloco\"> <label for=\"protocolo\">protocolo</label> <input type=\"text\" id=\"prot";
const char site17[] PROGMEM = "ocolo\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> </div> </div> </div> <p style=\"position: absolute;left: 15px;bottom: 0;\">made by lusc@</p></body><script> function trocaMenu(num){ var elemento = docum";
const char site18[] PROGMEM = "ent.getElementsByClassName(\"active\")[0]; elemento.classList.remove(\"active\"); var elemento2 = document.querySelector(\".principal form:nth-child(\"+num+\")\"); elemento2.classList.add(\"active\"); }</script></html>";
const char site19[] PROGMEM = "444555";

void enviarArquivo();

// Enc28j60 ==============================================

void setup(){
  pinMode(CS_ENC28J60, OUTPUT);

  
  
  Serial.begin(57600);
  Serial.println("\n[Teste]");

  //site = F("HTTP/1.1 200 OK\r\n Content-Type: text/html\r\n\r\n<!DOCTYPE html><html lang=\"pt-br\"><head> <meta charset=\"UTF-8\"> <title>Menu Config</title> <style>body{height:100vh;justify-content:center;align-items:center;display:flex;font-family:monospace;background:linear-gradient(245deg,#7ac5e3ad,#a960e1);overflow:hidden;user-select:none}.frame{display:flex;flex-direction:column;justify-content:center;min-width:20vw;height:40vh;padding:2rem;background:linear-gradient(185deg,#e5e5e55c,#f7f7f794);border-radius:.4em;backdrop-filter:blur(5px)}.frame input[type=submit]{height:1.7em;min-width:50%;margin-left:auto;border:none;border-radius:6px;background:linear-gradient(243deg,#fffffff5,#f7f7f757);transition:.7s}.principal form{display:none;flex-wrap:wrap;justify-content:space-around;align-items:center;font-size:16px;position:absolute;transition:.5s}.cards .frame .bloco input[type=text]{border:0;border-bottom:1px solid #000;font-size:1.2rem;background:none;cursor:text}.opcoes{width:20%;height:100%;align-items:start;display:flex;flex-direction:column;justify-content:center}.cards .frame .bloco{display:flex;flex-direction:column;width:40%;margin:10px 0;align-items:start}.cards .frame{margin:1vh 1vw;height:80vh;width:56vw;padding:1rem;flex-direction:row}.frame input{margin-bottom:3vh;font-size:1.4rem;cursor:pointer;outline:none}.principal{width:78%;display:flex;align-items:center;position:relative}.opcao p{line-height:2;transition:.7s;user-select:none;cursor:pointer}.frame p{margin:0 0 .4em;font-weight:400;font-size:1.4rem}.cards .frame input[type=submit]{height:1.4em;min-width:40%}.frame input[type=submit]:hover{transform:scale(0.95)}.cards h1{margin:auto;font-size:2.5em;width:100%}.bloco input[type=submit]{margin-left:0}.opcao p:hover{transform:scale(0.9)}.principal form.active{display:flex}h1{margin:auto;font-size:2.5em}</style></head><body> <div class=\"cards\"> <div class=\"frame\"> <div class=\"opcoes\"> <div class=\"opcao\"> <p data-option=\"red\" onclick=\"trocaMenu(1)\">Rede</p> </div> <div class=\"opcao\"> <p data-option=\"s1\" onclick=\"trocaMenu(2)\">Serial 1</p> </div> <div class=\"opcao\"> <p data-option=\"s2\" onclick=\"trocaMenu(3)\">Serial 2</p> </div> </div> <div class=\"principal\"> <form class=\"rede active\"> <h1>rede</h1> <div class=\"bloco\"> <label for=\"host\">Hostname</label> <input type=\"text\" id=\"host\"> </div> <div class=\"bloco\"> <label for=\"ip\">IP</label> <input type=\"text\" id=\"ip\"> </div> <div class=\"bloco\"> <label for=\"mask\">Máscara</label> <input type=\"text\" id=\"mask\"> </div> <div class=\"bloco\"> <label for=\"gateway\">Gateway</label> <input type=\"text\" id=\"gateway\"> </div> <div class=\"bloco\"> <label for=\"dns1\">Dns 1</label> <input type=\"text\" id=\"dns1\"> </div> <div class=\"bloco\"> <label for=\"dns2\">Dns 2</label> <input type=\"text\" id=\"dns2\"> </div> <div class=\"bloco\"> <label for=\"mac\">Mac </label> <input type=\"text\" id=\"mac\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> <form class=\"rede\"> <h1>Serial 1</h1> <div class=\"bloco\"> <label for=\"velocidade\">Velocidade</label> <input type=\"text\" id=\"velocidade\"> </div> <div class=\"bloco\"> <label for=\"bits\">Bits de daods</label> <input type=\"text\" id=\"bits\"> </div> <div class=\"bloco\"> <label for=\"paridade\">paridade</label> <input type=\"text\" id=\"paridade\"> </div> <div class=\"bloco\"> <label for=\"porta\">porta</label> <input type=\"text\" id=\"porta\"> </div> <div class=\"bloco\"> <label for=\"protocolo\">protocolo</label> <input type=\"text\" id=\"protocolo\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> <form class=\"rede\"> <h1>Serial 2</h1> <div class=\"bloco\"> <label for=\"velocidade\">Velocidade</label> <input type=\"text\" id=\"velocidade\"> </div> <div class=\"bloco\"> <label for=\"bits\">Bits de daods</label> <input type=\"text\" id=\"bits\"> </div> <div class=\"bloco\"> <label for=\"paridade\">paridade</label> <input type=\"text\" id=\"paridade\"> </div> <div class=\"bloco\"> <label for=\"porta\">porta</label> <input type=\"text\" id=\"porta\"> </div> <div class=\"bloco\"> <label for=\"protocolo\">protocolo</label> <input type=\"text\" id=\"protocolo\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> </div> </div> </div> <p style=\"position: absolute;left: 15px;bottom: 0;\">made by lusc@</p></body><script> function trocaMenu(num){ var elemento = document.getElementsByClassName(\"active\")[0]; elemento.classList.remove(\"active\"); var elemento2 = document.querySelector(\".principal form:nth-child(\"+num+\")\") elemento2.classList.add(\"active\"); }</script></html>");

  //ENC28J60 Setup =======================================
  
  
  if (ether.begin(sizeof Ethernet::buffer, mymac, CS_ENC28J60)){
      Serial.println(F("Controlador OK"));
  }else{
      Serial.println(F("Deu ruim no controlador")); 
  }    
    
  if (ether.dhcpSetup())
    Serial.println(F("DHCP OK"));  
  else
    Serial.println(F("deu ruim no DHCP"));
      
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GATEWAY:  ", ether.gwip);    
  ether.hisport = 4000;
  
 
  //ENC28J60 Setup =======================================


  
  //Pinos analógicos sensores ============
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  //Pinos analógicos sensores =============
  
  //Shift Register ========================
  pinMode(SH_CP, OUTPUT);
  pinMode(ST_CP, OUTPUT);
  pinMode(DS, OUTPUT);
  
  shiftOut(DS, SH_CP, LSBFIRST, 0x01);
  digitalWrite(ST_CP, LOW);
  digitalWrite(ST_CP, HIGH);
  digitalWrite(ST_CP, LOW);
  //Shift Register =======================

  

}

void loop(){
    
    //LEMBRAR DE ABRIR PROTOCOLO DE PING NA PORTA 2000 RESPONDENDO OK

    //Olhando buffer da porta 80 pro webservice
    webApp();

    //Olhando buffer da porta 4000 pros controles de relês
    //olha_tcp();
    
    //filtro para limitar o número de leituras e cálculos e não
    //Sobrecarregar o sistema    
    if(millis() % 10 == 0){
        LeituraSensores();    
   }
    
}



//Funções enc28j60 ===========================================


void webApp(){
  ether.hisport = 80;  

    word pos = ether.packetLoop(ether.packetReceive());//Convertendo pacote do buffer    
    if (pos) {
        ether.httpServerReplyAck();                    // Retorna ACK para confirmar conexão
                
        char* data = (char *) Ethernet::buffer + pos;  // Montando pacote de dados 

        if(!strncmp("GET / ", data, 6)){
            enviarArquivo();
        }else if(!strncmp("GET /dados", data, 10)){
          memcpy_P(ether.tcpOffset(),site19, sizeof(site19));
          ether.httpServerReply_with_flags(sizeof site19 - 1, TCP_FLAGS_ACK_V| TCP_FLAGS_FIN_V);    
        }
    }
    
}


void olha_tcp(){
      
  ether.hisport = 4000;
    word pos = ether.packetLoop(ether.packetReceive());//Convertendo pacote do buffer    
    if (pos) {
        ether.httpServerReplyAck();                   // Retorna ACK para confirmar conexão
        
        char* data = (char *) Ethernet::buffer + pos; // Montando pacote de dados 

        if(strstr(data, "RESET") != 0){               // Interpreta se é abertura ou fechamento de relés
            int rele = data[6];                       // Pega a casa do relé
            rele -= 48;                               // Passa de ascii para decimal
            if(rele < 1 || rele > 4) {return;}        // Verifica se é um número válido
                        
            MudaReles(rele, 1);                       // muda o Relé
            
        }else if(strstr(data, "SET") != 0){           // Mesma coisa
            int rele = data[4];     
            rele -= 48;            
            if(rele < 1 || rele > 4 ) {return;}
            MudaReles(rele, 0);             
            
        }else if (!strncmp("CLOSE", data, 5)){           // se solicitado, finaliza conexão
            memcpy_P(ether.tcpOffset(), END, sizeof END);// Envia Pacote de finalização com ACK e a flag de FIN
            ether.httpServerReply_with_flags(sizeof END - 1,TCP_FLAGS_ACK_V|TCP_FLAGS_FIN_V);
        }
    }  
}

void enviarArquivo(){    
 
    memcpy_P(ether.tcpOffset(),site0 , sizeof(site0));
    ether.httpServerReply_with_flags(sizeof site0 - 1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site1 , sizeof(site1));
    
    ether.httpServerReply_with_flags(sizeof site1 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site2 , sizeof(site2));
    
    ether.httpServerReply_with_flags(sizeof site2 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site3 , sizeof(site3));
    
    ether.httpServerReply_with_flags(sizeof site3 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site4 , sizeof(site4));
    
    ether.httpServerReply_with_flags(sizeof site4 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site5 , sizeof(site5));
    
    ether.httpServerReply_with_flags(sizeof site5 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site6 , sizeof(site6));
    
    ether.httpServerReply_with_flags(sizeof site6 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site7 , sizeof(site7));
    
    ether.httpServerReply_with_flags(sizeof site7 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site8 , sizeof(site8));
    
    ether.httpServerReply_with_flags(sizeof site8 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site9 , sizeof(site9));
    
    ether.httpServerReply_with_flags(sizeof site9 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site10 , sizeof(site10));
    
    ether.httpServerReply_with_flags(sizeof site10 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site11 , sizeof(site11));
    
    ether.httpServerReply_with_flags(sizeof site11 -1, TCP_FLAGS_ACK_V);
    memcpy_P(ether.tcpOffset(),site12 , sizeof(site12));
    
    ether.httpServerReply_with_flags(sizeof site12 -1, TCP_FLAGS_ACK_V);
    memcpy_P(ether.tcpOffset(),site13 , sizeof(site13));
    
    ether.httpServerReply_with_flags(sizeof site13 -1, TCP_FLAGS_ACK_V);    
    memcpy_P(ether.tcpOffset(),site14 , sizeof(site14));
    
    ether.httpServerReply_with_flags(sizeof site14 -1, TCP_FLAGS_ACK_V);
    memcpy_P(ether.tcpOffset(),site15 , sizeof(site15));
    
    ether.httpServerReply_with_flags(sizeof site15 -1, TCP_FLAGS_ACK_V);
    memcpy_P(ether.tcpOffset(),site16 , sizeof(site16));
    
    ether.httpServerReply_with_flags(sizeof site16 -1, TCP_FLAGS_ACK_V);
    memcpy_P(ether.tcpOffset(),site17 , sizeof(site17));
    
    ether.httpServerReply_with_flags(sizeof site17 -1, TCP_FLAGS_ACK_V);
    memcpy_P(ether.tcpOffset(),site18 , sizeof(site18));
    
    ether.httpServerReply_with_flags(sizeof site18 -1, TCP_FLAGS_ACK_V|TCP_FLAGS_FIN_V );
    
    Serial.println("Arquivo Enviado");
}


//Funções enc28j60 ===========================================

//Funções Sensores e relés ===================================

void LeituraSensores(){  
    //Notificar quando sensor for modificado
    
    int leitura, res;
  
    leitura = analogRead(A0);
    res = (MediaMovel(res, vetMedia0) > 700);
    if(Sensor0 != res){
       Sensor0 = res;
       //Enviar Sinal para ip da máquina ======================
    }
        
    leitura = analogRead(A1);
    res = (MediaMovel(res, vetMedia1) > 700);  
    if(Sensor1 != res){
       Sensor1 = res;
       //Enviar Sinal para ip da máquina ======================
    }
    
    leitura = analogRead(A2);
    res = (MediaMovel(res, vetMedia2) > 700);
    if(Sensor2 != res){
       Sensor1 = res;
       //Enviar Sinal para ip da máquina ======================
    }
    
    leitura = analogRead(A3);
    res = (MediaMovel(res, vetMedia3) > 700);     
    if(Sensor3 != res){
        Sensor1 = res;
       //Enviar Sinal para ip da máquina ======================
    } 
    
}

void MudaReles(int Rele, int sinal){  
    Serial.println("muda rele");  
    Serial.println(shiftRegister);
  
    char Resposta[8] = {50,53,48,48,48,48,48, 0};
    
    if(sinal){                              //Se for abertura, modifica o bit do byte para positivo
      Resposta[6] = 49;                     //Monta resposta para enviar à máquina
      shiftRegister |= 1UL << Rele ;        //Modifica o bit d aposição solicitada para positivo
    }else{                                  //Mesma coisa
      Resposta[6] = 48;
      shiftRegister &= ~(1UL << Rele );
    }
    
    Resposta[3] = Rele + 48;                //O terceiro char do vetor indica qual é o relé que foi modificado
    

      memcpy_P(ether.tcpOffset(), Resposta, 9);  

    
    //Trocar para Ip da máquina
    ether.httpServerReply_with_flags(7 ,TCP_FLAGS_ACK_V | TCP_FLAGS_FIN_V); 
   
    shiftOut(DS, SH_CP, LSBFIRST, shiftRegister); //Escrever dados no shift Register
    
    digitalWrite(ST_CP, LOW);                     
    digitalWrite(ST_CP, HIGH);                    //Dando um sinal de clock para ele atualizar
    digitalWrite(ST_CP, LOW);    
}

long MediaMovel(int val,int vetMedia[]){
  
  for(int i = 0; i < FILTRO-1; i++){
      vetMedia[i] = vetMedia[i+1];      //shiftando o vetor
  }  
  
  vetMedia[FILTRO-1] = val;             //adicionando valor ao final do vetor
  
  int acc =0;  
  for(int i =0; i < FILTRO; i++){
      acc += vetMedia[i];               //Somando valores
  }
    
  return acc / FILTRO;                  //calculando a média
  
}

//Funções Sensores e relés ===================================
