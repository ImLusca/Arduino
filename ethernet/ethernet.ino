void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

int recebimento = 0;

void loop() {

  while(Serial.available() > 0){
    recebimento = Serial.read();
    digitalWrite(recebimento, HIGH);
    delay(150);
    digitalWrite(recebimento, LOW);
  }

}
