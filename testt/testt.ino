#include <EtherCard.h>
static byte myip[] = { 192,168,15,7 };
static byte gwip[] = { 192,168,15,1 };
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
 
ETHER_28J60 e;

void setup() {
  // put your setup code here, to run once:
Serial.begin(57600);
  Serial.println("\n[Temp]");

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Deu ruim no controlador");
  else{
    Serial.println( "Controlador OK");  
  }
  
   if (!ether.dhcpSetup())
      Serial.println("deu ruim no DHCP");
      
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GATEWAY:  ", ether.gwip);    
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  digitalWrite(6, HIGH);
}

void loop() {
  if (e.serviceRequest())
  {
    e.print("OK");
    e.respond();
  }
  delay(100);

}
