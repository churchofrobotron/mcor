/* USB to Serial - Teensy becomes a USB to Serial converter
   http://dorkbotpdx.org/blog/paul/teensy_as_benito_at_57600_baud

   You must select Serial from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#define MY_TYPE "START"
#define MY_ADDR "1"
#define SEPARATOR ":"
String preamble=MY_TYPE MY_ADDR SEPARATOR;
// String command;

const int len = 4;
const unsigned int pins[len] = {9, 10, 11, 12};
int states[len] = {LOW, LOW, LOW, LOW};

const unsigned int baud=9600;

void togglePin(int pin)
{
  if (states[pin] == HIGH)  { 
    states[pin] = LOW;
  } 
  else {
    states[pin] = HIGH;
  }
  digitalWrite(pins[pin], states[pin]);
}

void setup()
{
  Serial.begin(baud);	// USB, communication to PC or Mac

  for (int i = 0; i < len; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], states[i]);
  }
}

// return the data part of the command
int parseCommand(const String &cmd)
{
  //command format: <my_type><my_addr>SEPARATOR<data>
  if (!cmd.startsWith(preamble)) {
    return NULL;
  }
  
  String num = cmd.substring(preamble.length(), preamble.length() + 4);
  
  char buf[4];
  num.toCharArray(buf, 4); //up to 4 characters
  unsigned int mask;
  sscanf(buf, "%d", &mask);
  return mask;
}

String getCommand()
{
  String command = "";
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

void loop()
{
  int cmd = parseCommand(getCommand());
  if (cmd != NULL) {
    Serial.println("got command");
    Serial.println(cmd);
    togglePin(cmd);
  }
  delay(1000);
}
