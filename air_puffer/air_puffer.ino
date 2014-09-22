
#define ON_TIME 50
#define OFF_TIME 1500
#define PUFF_DELAY 1000

const unsigned int BAUD = 9600;

void setup(){
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  Serial.begin(BAUD);	// USB, communication to PC or Mac
}

void pin_cycle(byte pin){
  digitalWrite(pin, HIGH);
  delay(ON_TIME);
  digitalWrite(pin, LOW);
  delay(OFF_TIME);
}

String command = "";
unsigned long start1 = 0;
unsigned long start2 = 0;

void loop(){
  
  char c;
  while (Serial.available()) {
    c=Serial.read();
    if (c=='\n' || c=='\r') {
      if (command.startsWith("WaveNum1")){
        start1 = millis();
      }
      else if(command.startsWith("WaveNum2")){
        start2 = millis();
      }     
      command="";
    }
    else {
      command += c;      
    }
  }
  
  if(start1 && ((millis() - start1) > PUFF_DELAY)){
    pin_cycle(0);
    start1 = 0;
  }
  if(start2 && ((millis() - start2) > PUFF_DELAY)){
    pin_cycle(1);
    start2 = 0;
  }
}
