/* Church of Robotron

   Main Altar controller
   Includes Son of Flapper, remote AC control

*/

#include <Servo.h>

Servo leftServo;
Servo rightServo;

const int remoteDelay = 200;

void remotePulse(int pin) {
  digitalWrite(pin, HIGH);
  delay(remoteDelay);
  digitalWrite(pin, LOW);
}

// based on laser_driver.ino.. just added different pin for off and increased strobe time.
class laser
{
public:
  laser(unsigned int on_pin_idx, unsigned int off_pin_idx):
  _onpin(on_pin_idx),
  _offpin(off_pin_idx),
  _state(false),
  _pulse_mode(false),
  _off_time(0)
  {
  }

  ~laser() {
    _off();
  }

  void init() {
    pinMode(_onpin, OUTPUT);
    pinMode(_offpin, OUTPUT);
    _off();
  }

  void tick () {
    unsigned long time=millis();
    if (_state && ((_pulse_mode && _off_time<=time)))
    {
      _off();
    }
  }

  void pulse()
  {
    if (!_state) {
      _on();
      _pulse_mode=true;
      _off_time=millis()+_pulse_time;
    } else {
      _off();
    }
  }

  void steady()
  {
    _on();
    _pulse_mode=false;
  }

  void off()
  {
    _off();
  }

private:
  unsigned int _onpin;
  unsigned int _offpin;
  bool _state;
  bool _pulse_mode;
  unsigned long _off_time;
  static const unsigned int _pulse_time=1000;

  void _off() {
    remotePulse(_offpin);
    _state=false;
  }

  void _on() {
    remotePulse(_onpin);
    _state=true;
  }
};

laser Strobe(11, 12);
laser DeathFlash(13, 14);
laser ExtraPlayer(15, 16);
const int NUM_LASERS = 3;
laser* lasers[NUM_LASERS] = {&Strobe, &DeathFlash, &ExtraPlayer};

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
String cmdPlayerDeath = "PlayerDeath";
String cmdExtraPlayer = "XP";

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

  for (int i = 0; i < NUM_LASERS; i++)
    lasers[i]->init();
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
  for (int i = 0; i < NUM_LASERS; i++)
    lasers[i]->tick();
}

void flap() {
  leftServo.write(flapAngleLeft);
  rightServo.write(flapAngleRight);
  delay(500);
  leftServo.write(idleAngleLeft);
  rightServo.write(idleAngleRight);
}

void genericCmd(String myCmd) {
  if (myCmd == cmd1on) {
    remotePulse(ac1on);
  } else if (myCmd == cmd1off) {
    remotePulse(ac1off);
  } else if (myCmd == cmd2on) {
    remotePulse(ac2on);
  } else if (myCmd == cmd2off) {
    remotePulse(ac2off);
  } else if (myCmd == cmd3on) {
    remotePulse(ac3on);
  } else if (myCmd == cmd3off) {
    remotePulse(ac3off);
  } else if (myCmd == cmd4on) {
    remotePulse(ac4on);
  } else if (myCmd == cmd4off) {
    remotePulse(ac4off);
  } else if (myCmd == cmd5on) {
    remotePulse(ac5on);
  } else if (myCmd == cmd5off) {
    remotePulse(ac5off);
  }
}

void serialCmd(String myCmd) {
  if (myCmd == cmdFlap) {
    flap();
  } else if (myCmd == cmdPlayerDeath) {
    Strobe.pulse();
  } else if (myCmd == cmdEnd) {
    DeathFlash.pulse();
  } else if (myCmd == cmdExtraPlayer) {
    ExtraPlayer.pulse();
  } else if (myCmd == cmdTweak) {
    leftServo.write(flapAngleLeft);
    rightServo.write(flapAngleRight);
  } else if (myCmd == cmdIdle) {
    leftServo.write(idleAngleLeft);
    rightServo.write(idleAngleRight);
  }
}
