#include <Metro.h>
#include <Queue.h>

const int len = 4;
Metro metros[len] = {
  Metro(1, true), Metro(10, true), Metro(100, true), Metro(1000, true)};
Queue queues[len] = {Queue(), Queue(), Queue(), Queue()};
int pins[len] = {9, 10, 12, 13};
int states[len] = {LOW, LOW, LOW, LOW};

static float msbase = 100;
static float msmax = 1750;
static float lenbase = 1;
static float lenmax = 6;
static float gapbase = 20000;
static float gapmin = 500;
static float offmin = 10;
static float offmax = 250;

void loadQueue(Queue *queue, int ms, int len, int gap)
{
  len = random(1, len);
  gap = random(gap / 2, gap);
  // push even parity group of ons and offs
  for (int i = 0; i < len; i++) {
    queue->push(random(1, ms));
    queue->push(random(offmin, offmax));
  }
  // push a final on to make us odd parity
  queue->push(random(1, ms));
  // push delay between group to make us even parity
  queue->push(gap);

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

void setup()
{
  for (int i = 0; i < len; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], states[i]);
  }
}

void loop()
{
  for (int i = 0; i < len; i++) {
    if (queues[i].isEmpty()) {
      // We've used up this sequence of firings, speed up and load another.
      msbase = min(msbase * 1.05, msmax);
      lenbase = min(lenbase * 1.05, lenmax);
      gapbase = max(gapbase * 0.97, gapmin);
      loadQueue(&queues[i], msbase, lenbase, gapbase);
      // write a diagnostic
      Serial.println("");
      Serial.println("loop");
      Serial.println(msbase);
      Serial.println(lenbase);
      Serial.println(gapbase);
    }
    if (metros[i].check() == 1) {
      // the metro has fired.  Toggle its pin and tell it to fire when
      // the pin should be toggled again.
      togglePin(i);
      metros[i].interval(queues[i].pop());
    }
  }    
  delay(1);
}
