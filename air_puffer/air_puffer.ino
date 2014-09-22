
#define ON_TIME 50
#define OFF_TIME 1500

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

String command = ""

void loop(){
  
  char c;
  //check for new serial characters
  while (Serial.available()) {
    c=Serial.read();
    if (c=='\n' || c=='\r') {
      if (cmd.startsWith("WaveNum")){  //it's our guy
      
      }
      command="";
    }
    else {
      
    }
  }
  //pin_cycle(0);
  //pin_cycle(1);
}
