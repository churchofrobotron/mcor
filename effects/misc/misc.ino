#define WAVE_TYPE "WAV"
#define KILLED_TYPE "HUMKIL"
#define MY_ADDR "1"
#define SEPARATOR ":"
#define WAVE_COMMAND 1
#define KILLED_COMMAND 2
String wavePreamble = WAVE_TYPE MY_ADDR SEPARATOR;
String killedPreamble = KILLED_TYPE MY_ADDR SEPARATOR;
#define WAVE_PIN 10
#define KILLED_PIN 9
const unsigned int baud = 9600;

// must be outside of function, http://arduino.cc/forum/index.php?topic=73177.0
static String command = "";
const int killedInterval = 3000;       // 3 secs
const int waveInterval = 5000;       // 5 secs
static unsigned long killedTime;
static unsigned long waveTime;

int parseCommand(const String &cmd)
{
  //command format: <my_type><my_addr>SEPARATOR<data>
  if (cmd.startsWith(killedPreamble)) {
    return KILLED_COMMAND;
  }
  if (cmd.startsWith(wavePreamble)) {
    return WAVE_COMMAND;
  }
  return NULL;
//   // ignore the number part of the command  
//   String num = cmd.substring(preamble.length(), preamble.length() + 4);
//   char buf[4];
//   num.toCharArray(buf, 4); //up to 4 characters
//   unsigned int mask;
//   sscanf(buf, "%d", &mask);
//   return mask;
}

String getCommand()
{
  char c;
  while (Serial.available()) {
    c = Serial.read();
    if (c=='\n' || c=='\r') {
      String out = command;
      command="";
      return out;
    } else {
      command += c;
    }
  }
  return NULL;
}

void setup()
{
  Serial.begin(baud);	// USB, communication to PC or Mac
  killedTime = 0;
  waveTime = 0;
  pinMode(WAVE_PIN, OUTPUT);
  digitalWrite(WAVE_PIN, LOW);
  pinMode(KILLED_PIN, OUTPUT);
  digitalWrite(KILLED_PIN, LOW);
}

void loop()
{
  int cmd = parseCommand(getCommand());
  if (cmd == KILLED_COMMAND) {
    digitalWrite(KILLED_PIN, HIGH);
    killedTime = millis();
    Serial.println("killed start");
  } else if (cmd == WAVE_COMMAND) {
    digitalWrite(WAVE_PIN, HIGH);
    waveTime = millis();
    Serial.println("wave start");
  }
  if (millis() - killedTime > killedInterval) {
    digitalWrite(KILLED_PIN, LOW);
    Serial.println("killed stop");
  }
  if (millis() - waveTime > waveInterval) {
    digitalWrite(WAVE_PIN, LOW);
    Serial.println("wave stop");
  }
  delay(100);
}
