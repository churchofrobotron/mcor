void setup() {
  char serialBuffer[12];
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  pinMode(0, OUTPUT);
}

void fog() {
  digitalWrite(11, HIGH);
  digitalWrite(0, HIGH);
  delay(5 * 1000);
  digitalWrite(11, LOW);
  digitalWrite(0, LOW);    
}

void serialCmd(String cmd) {
  if (cmd == "Fog") {
    fog();
  }
}

char inChar = -1;
const int serialBufferLen = 24;
char serialBuffer[serialBufferLen];
int index = 0;

void loop() {
  while (Serial.available() > 0)
  {
    inChar = Serial.read();
    Serial.print(inChar);
    if (inChar != '\n') {
      serialBuffer[index] = inChar;
      index++;
      if (index >= serialBufferLen)
        index = 0;
    } else if (inChar == '\n') {
      index++;
      if (index >= serialBufferLen)
        index = 0;
      serialBuffer[index] = '\0';
      serialCmd(serialBuffer);
      for (int x = 0; x < serialBufferLen; x++) serialBuffer[x] = '\0';
      index = 0;
      inChar = -1;
    }
  }
}
