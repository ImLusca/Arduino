// Present a "Will be back soon web page", as stand-in webserver.
// 2011-01-30 <jc@wippler.nl>
//
// License: GPLv2

#include <EtherCard.h>

#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,200 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <!-- <link rel=\"stylesheet\" href=\"style.css\"> --> <title>Menu Config</title> <style> body{height: 100vh;justify-content: center;align-items: center;display: flex;font-family: monospace;background:linear-gradient(245deg,#7ac5e3ad,#a960e1); overflow: hidden;user-select: none;} .frame{display: flex;flex-direction: column;justify-content: center;min-width: 20vw;height: 40vh;padding: 2rem;background: linear-gradient(185deg, #e5e5e55c, #f7f7f794);border-radius: 0.4em;backdrop-filter: blur(5px);} .frame input[type=submit]{height: 1.7em;min-width: 50%;margin-left: auto;border: none;border-radius: 6px;background: linear-gradient(243deg, #fffffff5, #f7f7f757);transition: 0.7s;} .principal form{display: none;flex-wrap: wrap;justify-content: space-around;align-items: center; font-size: 16px; position: absolute; transition: 0.5s;} .cards .frame .bloco input[type=text]{border: 0;border-bottom: 1px solid #000;font-size: 1.2rem;background: none;cursor: text;} .opcoes{width: 20%;height: 100%;align-items: start;display: flex;flex-direction: column;justify-content: center;} .cards .frame .bloco{display: flex;flex-direction: column;width: 40%;margin: 10px 0;align-items: start;} .cards .frame{margin: 1vh 1vw;height: 80vh;width: 56vw;padding: 1rem;flex-direction: row;} .frame input{margin-bottom: 3vh;font-size: 1.4rem;cursor: pointer;outline: none;} .principal{width: 78%; display: flex;align-items: center; position: relative;} .opcao p{line-height: 2;transition: 0.7s;user-select: none;cursor: pointer;} .frame p{margin: 0 0 0.4em 0;font-weight: 400;font-size: 1.4rem;} .cards .frame input[type=submit]{height: 1.4em;min-width: 40%;} .frame input[type=submit]:hover{transform: scale(0.95);} .cards h1{margin: auto;font-size: 2.5em;width: 100%;} .bloco input[type=submit]{margin-left: 0;} .opcao p:hover{transform: scale(0.9);} .principal form.active{display: flex;} h1{margin: auto;font-size: 2.5em;} </style></head><body> <div class=\"cards\"> <div class=\"frame\"> <div class=\"opcoes\"> <div class=\"opcao\"> <p data-option=\"red\" onclick=\"trocaMenu(1)\">Rede</p> </div> <div class=\"opcao\"> <p data-option=\"s1\" onclick=\"trocaMenu(2)\">Serial 1</p> </div> <div class=\"opcao\"> <p data-option=\"s2\" onclick=\"trocaMenu(3)\">Serial 2</p> </div> </div> <div class=\"principal\"> <form class=\"rede active\"> <h1>rede</h1> <div class=\"bloco\"> <label for=\"host\">Hostname</label> <input type=\"text\" id=\"host\"> </div> <div class=\"bloco\"> <label for=\"ip\">IP</label> <input type=\"text\" id=\"ip\"> </div> <div class=\"bloco\"> <label for=\"mask\">Máscara</label> <input type=\"text\" id=\"mask\"> </div> <div class=\"bloco\"> <label for=\"gateway\">Gateway</label> <input type=\"text\" id=\"gateway\"> </div> <div class=\"bloco\"> <label for=\"dns1\">Dns 1</label> <input type=\"text\" id=\"dns1\"> </div> <div class=\"bloco\"> <label for=\"dns2\">Dns 2</label> <input type=\"text\" id=\"dns2\"> </div> <div class=\"bloco\"> <label for=\"mac\">Mac </label> <input type=\"text\" id=\"mac\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> <form class=\"rede\"> <h1>Serial 1</h1> <div class=\"bloco\"> <label for=\"velocidade\">Velocidade</label> <input type=\"text\" id=\"velocidade\"> </div> <div class=\"bloco\"> <label for=\"bits\">Bits de daods</label> <input type=\"text\" id=\"bits\"> </div> <div class=\"bloco\"> <label for=\"paridade\">paridade</label> <input type=\"text\" id=\"paridade\"> </div> <div class=\"bloco\"> <label for=\"porta\">porta</label> <input type=\"text\" id=\"porta\"> </div> <div class=\"bloco\"> <label for=\"protocolo\">protocolo</label> <input type=\"text\" id=\"protocolo\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> <form class=\"rede\"> <h1>Serial 2</h1> <div class=\"bloco\"> <label for=\"velocidade\">Velocidade</label> <input type=\"text\" id=\"velocidade\"> </div> <div class=\"bloco\"> <label for=\"bits\">Bits de daods</label> <input type=\"text\" id=\"bits\"> </div> <div class=\"bloco\"> <label for=\"paridade\">paridade</label> <input type=\"text\" id=\"paridade\"> </div> <div class=\"bloco\"> <label for=\"porta\">porta</label> <input type=\"text\" id=\"porta\"> </div> <div class=\"bloco\"> <label for=\"protocolo\">protocolo</label> <input type=\"text\" id=\"protocolo\"> </div> <div class=\"bloco\"> <input type=\"submit\" value=\"Confirmar\"> </div> </form> </div> </div> </div> <p style=\"position: absolute;left: 15px;bottom: 0;\">made by lusc@</p></body><script> function trocaMenu(num){ var elemento = document.getElementsByClassName(\"active\")[0]; elemento.classList.remove(\"active\"); var elemento2 = document.querySelector(\".principal form:nth-child(\"+num+\")\") elemento2.classList.add(\"active\"); }</script></html>"
;

void setup(){
  Serial.begin(57600);
  Serial.println("\n[backSoon]");

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
}

void loop(){
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}
