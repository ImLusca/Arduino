
  #define SH_CP  3
  #define ST_CP 4
  #define DS 5

  #define FILTRO 10

  int leitorSerial0 = 0;
  int leitorSerial1 = 0;
  int leitorSerial2 = 0;
  int leitorSerial3 = 0;
  
  int vetMedia0[FILTRO];
  int vetMedia1[FILTRO];
  int vetMedia2[FILTRO];
  int vetMedia3[FILTRO];



  long int contador = 0;  
  
  void LeituraSensores();  
  long MediaMovel(int val,int vetMedia[]);
  void MudaReles();

void setup() {
  Serial.begin(9600);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
  pinMode(SH_CP, OUTPUT);
  pinMode(ST_CP, OUTPUT);
  pinMode(DS, OUTPUT);
  
  shiftOut(DS, SH_CP, LSBFIRST, 0x00);
  digitalWrite(ST_CP, LOW);
  digitalWrite(ST_CP, HIGH);
  digitalWrite(ST_CP, LOW);

}

void loop() {

  if(millis() % 50 == 0){
    Serial.println("Muda Rele");    
      MudaReles();
  }
  if(millis() % 5 == 0){
      LeituraSensores();    
  }  
}


void LeituraSensores(){
  
    int res;
  
    res = analogRead(A0);
    leitorSerial0 = MediaMovel(res, vetMedia0);
    res = analogRead(A1);
    leitorSerial1 = MediaMovel(res, vetMedia1);
    res = analogRead(A2);
    leitorSerial2 = MediaMovel(res, vetMedia2);
    res = analogRead(A3);
    leitorSerial3 = MediaMovel(res, vetMedia3);  

    Serial.print(leitorSerial0);
    Serial.print(" ");
    Serial.print(leitorSerial1);
    Serial.print(" ");
    Serial.print(leitorSerial2);
    Serial.print(" ");
    Serial.println(leitorSerial3);
                
}

void MudaReles(){
    
  
    byte num = 0b10000000;
    
    if(leitorSerial0 > 700){
        num ^= 0b00000100;  
    }
    if(leitorSerial1 > 700){
        num ^= 0b00100000;  
    }
    if(leitorSerial2 > 700){
        num ^= 0b00010000;  
    }
    if(leitorSerial3 > 700){
        num ^= 0b00001000;  
    }
    shiftOut(DS, SH_CP, LSBFIRST, num);
    digitalWrite(ST_CP, LOW);
    digitalWrite(ST_CP, HIGH);
    digitalWrite(ST_CP, LOW);
    
}

long MediaMovel(int val,int vetMedia[]){

  //shiftando o vetor
  for(int i = 0; i < FILTRO-1; i++){
      vetMedia[i] = vetMedia[i+1];
  }  
  vetMedia[FILTRO-1] = val;
  
  int acc =0;
  //Somando valores
  for(int i =0; i < FILTRO; i++){
      acc += vetMedia[i];
  }
  
  //Tirando a média
  return acc / FILTRO;
  
}
