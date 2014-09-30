/* Church of Robotron

   Main Altar controller
   Includes Son of Flapper, remote AC control
   
*/

#include <Servo.h>

Servo leftServo;
Servo rightServo;

// Serial command strings
String cmd1on = "1on";
String cmd1off = "1off";
String cmd2on = "2on";
String cmd2off = "2off";
String cmd3on = "3on";
String cmd3off = "3off";
String cmd4on = "4on";
String cmd4off = "4off";
String cmd5on = "5on";
String cmd5off = "5off";
String cmdFlap = "HumanKilled";
String cmdTweak = "tweak";
String cmdIdle = "idle";
String cmdEnd = "GameOver";

// AC outlet pins
int ac1on = 11;
int ac1off = 12;
int ac2on = 13;
int ac2off = 14;
int ac3on = 15;
int ac3off = 16;
int ac4on = 17;
int ac4off = 18;
int ac5on = 19;
int ac5off = 20;
int remoteDelay = 200;

// Servo pins
int leftServoPin = 9;
int rightServoPin = 10;

// serial variables
char inChar = -1;
const int serialBufferLen = 24;
char serialBuffer[serialBufferLen];
int index = 0;

// servo angles for tweeaking
int idleAngleLeft = 0;
int flapAngleLeft = 90;

int idleAngleRight = 0;
int flapAngleRight = 90;

void setup() {
  
  leftServo.attach(leftServoPin);
  rightServo.attach(rightServoPin);
  leftServo.write(idleAngleLeft);
  rightServo.write(idleAngleRight);
  
  char serialBuffer[12];
  Serial.begin(9600);
  
  for(int x = 11; x <= 20; x++) {
     // set up AC outlet pins
     pinMode(x,OUTPUT);
     //make sure all AC adapter pins are not triggering)
     digitalWrite(x, LOW);
  }  
}

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

void flap() {
  leftServo.write(flapAngleLeft);
  rightServo.write(flapAngleRight);
  delay(500);
  leftServo.write(idleAngleLeft);
  rightServo.write(idleAngleRight);
}

void serialCmd(String myCmd) {
  if (myCmd == cmdFlap) flap();
  if (myCmd == cmdEnd) {
//    digitalWrite(ac1on, HIGH);
//    digitalWrite(ac5off, HIGH);
//    delay(remoteDelay);
//    digitalWrite(ac1on, LOW);
//    digitalWrite(ac5off, LOW);    
    digitalWrite(ac1on, HIGH);
    delay(remoteDelay);
    digitalWrite(ac1on, LOW);
    delay(remoteDelay);
    digitalWrite(ac5off, HIGH);
    delay(remoteDelay);
    digitalWrite(ac5off, LOW);
  }  
  if (myCmd == cmd1on) {
    digitalWrite(ac1on, HIGH);
    delay(remoteDelay);
    digitalWrite(ac1on, LOW);
  } else if (myCmd == cmd1off) {
    digitalWrite(ac1off, HIGH);
    delay(remoteDelay);
    digitalWrite(ac1off, LOW);
  } else if (myCmd == cmd2on) {
    digitalWrite(ac2on, HIGH);
    delay(remoteDelay);
    digitalWrite(ac2on, LOW);
  } else if (myCmd == cmd2off) {
    digitalWrite(ac2off, HIGH);
    delay(remoteDelay);
    digitalWrite(ac2off, LOW);
  } else if (myCmd == cmd3on) {
    digitalWrite(ac3on, HIGH);
    delay(remoteDelay);
    digitalWrite(ac3on, LOW);
  } else if (myCmd == cmd3off) {
    digitalWrite(ac3off, HIGH);
    delay(remoteDelay);
    digitalWrite(ac3off, LOW);
  } else if (myCmd == cmd4on) {
    digitalWrite(ac4on, HIGH);
    delay(remoteDelay);
    digitalWrite(ac4on, LOW);
  } else if (myCmd == cmd4off) {
    digitalWrite(ac4off, HIGH);
    delay(remoteDelay);
    digitalWrite(ac4off, LOW);
  } else if (myCmd == cmd5on) {
    digitalWrite(ac5on, HIGH);
    delay(remoteDelay);
    digitalWrite(ac5on, LOW);
  } else if (myCmd == cmd5off) {
    digitalWrite(ac5off, HIGH);
    delay(remoteDelay);
    digitalWrite(ac5off, LOW);
  } else if (myCmd == cmdTweak) {
    leftServo.write(flapAngleLeft);
    rightServo.write(flapAngleRight);
  } else if (myCmd == cmdIdle) {
    leftServo.write(idleAngleLeft);
    rightServo.write(idleAngleRight);
  }
}
