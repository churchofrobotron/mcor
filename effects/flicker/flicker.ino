#include <Metro.h>
#include <Queue.h>

// communication stuff

#define START_TYPE "START"
#define END_TYPE "END"
#define MY_ADDR "1"
#define SEPARATOR ":"
#define START_COMMAND 1
#define END_COMMAND 2
String startPreamble = START_TYPE MY_ADDR SEPARATOR;
String endPreamble = END_TYPE MY_ADDR SEPARATOR;
const unsigned int baud = 9600;
// must be outside of function, http://arduino.cc/forum/index.php?topic=73177.0
static String command = "";
static boolean on = false;

// effect stuff

struct effect {
  float onbase;
  float onmin;
  float onmax;
  float lenbase;
  float lenmax;
  float gapbase;
  float gapmin;
  float offmin;
  float offmax;
};

// 2 jacob's ladders, 5 arcers
static const int len = 7;
static effect effects[] = {
  effect(), effect(), effect(), effect(), effect(), effect(), effect()};
// first 2 effects are jacob's ladders, rest of the effects are arcers
static Metro metros[] = {
  Metro(12800), Metro(6400),
  Metro(3200), Metro(1600), Metro(800), Metro(400), Metro(200)};
// must hold lenmax * 2 + 2
static Queue queues[] = {
  Queue(50), Queue(50), Queue(50), Queue(50), Queue(50), Queue(50), Queue(50)};
// pins 9-10 are jacob's ladders, pins 12-16 are arcers
static const int pins[] = {9, 10, 12, 13, 14, 15, 16};
static int states[] = {LOW, LOW, LOW, LOW, LOW, LOW};

// return the data part of the command
int parseCommand(const String &cmd)
{
  //command format: <my_type><my_addr>SEPARATOR<data>
  if (cmd.startsWith(startPreamble)) {
    return START_COMMAND;
  }
  if (cmd.startsWith(endPreamble)) {
    return END_COMMAND;
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

void loadQueue(int q)
{
  // push even parity group of ons and offs
  for (int i = 0;
       i < random((effects[q].lenbase / 2), effects[q].lenbase);
       i++) {
    queues[q].push(random(effects[q].onmin, effects[q].onbase));
    queues[q].push(random(effects[q].offmin, effects[q].offmax));
  }
  // push a final on to make us odd parity
  queues[q].push(random(effects[q].onmin, effects[q].onbase));
  // push final off to make us even parity; delay between next group
  queues[q].push(random(effects[q].gapbase / 2, effects[q].gapbase));
}

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

void setup_effects()
{
  for (int i = 0; i < len; i++) {
    states[i] = LOW;
    digitalWrite(pins[i], states[i]);
  }
  long metro_interval = 128000;
  for (int i = 0; i < len; i++) {
    metros[i].interval(metro_interval);
    metro_interval = metro_interval / 2;
  }
  for (int i = 0; i < 2; i++) {
    // first 2 effects are jacob's ladders
    effects[i].onbase = 4000;
    effects[i].onmin = 2000;
    effects[i].onmax = 4000;
    effects[i].lenbase = 0;
    effects[i].lenmax = 0;
    effects[i].gapbase = 20000;
    effects[i].gapmin = 750;
    effects[i].offmin = 0;      // should not be used
    effects[i].offmax = 0;      // should not be used
  }
  for (int i = 2; i < len; i++) {
    // rest of the effects are arcers
    effects[i].onbase = 100;
    effects[i].onmin = 3;
    effects[i].onmax = 1250;
    effects[i].lenbase = 1;
    effects[i].lenmax = 10;
    effects[i].gapbase = 10000;
    effects[i].gapmin = 750;
    effects[i].offmin = 10;
    effects[i].offmax = 200;
  }
}

void setup()
{
  Serial.begin(baud);	// USB, communication to PC or Mac
  for (int i = 0; i < len; i++) {
    pinMode(pins[i], OUTPUT);
  }
  setup_effects();
}

void diag(int i)
{
  Serial.println("diag");
  Serial.println(i);
  Serial.println(effects[i].onbase);
  //Serial.println(effects[i].onmin);
  Serial.println(effects[i].onmax);
  Serial.println(effects[i].lenbase);
  Serial.println(effects[i].lenmax);
  Serial.println(effects[i].gapbase);
  Serial.println(effects[i].gapmin);
  //Serial.println(effects[i].offmin);
  //Serial.println(effects[i].offmax);
  Serial.println();
}

float randomFloat(float min, float max)
{
  return (float)random((long)(min * 100), (long)(max * 100)) / 100.0;
}

void effect_loop()
{
  for (int i = 0; i < len; i++) {
    if (queues[i].isEmpty()) {
      // We've used up this sequence of firings, speed up and load another.
      effects[i].onbase = min(
          randomFloat(effects[i].onbase * 1.05, effects[i].onbase * 1.1),
          effects[i].onmax);
      effects[i].lenbase = min(
          randomFloat(effects[i].lenbase * 1.05, effects[i].lenbase * 1.1),
          effects[i].lenmax);
      effects[i].gapbase = max(
          randomFloat(effects[i].gapbase * 0.9, effects[i].gapbase * 0.95),
          effects[i].gapmin);
      loadQueue(i);
      diag(i);
    }
    if (metros[i].check() == 1) {
      // The metro has fired.  Toggle its pin and tell it to fire when
      // the pin should be toggled again.
      togglePin(i);
      metros[i].reset();        // XXX necessary?
      metros[i].interval(queues[i].pop());
    }
  }    
}

void loop()
{
  int cmd = parseCommand(getCommand());
  if (cmd == START_COMMAND) {
    setup_effects();
    on = true;
    Serial.println("on");
  } else if (cmd == END_COMMAND) {
    setup_effects();
    on = false;
    Serial.println("off");
  }
  if (on) {
    effect_loop();
  }
  delay(1);
}
