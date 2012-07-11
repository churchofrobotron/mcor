#include <Metro.h>
#include <Queue.h>

const int len = 4;
Metro metros[len] = {
  Metro(10, true), Metro(10, true), Metro(10, true), Metro(10, true)};
Queue queues[len] = {Queue(), Queue(), Queue(), Queue()};
int leds[len] = {12, 10, 9, 8};
int states[len] = {LOW, LOW, LOW, LOW};

float msbase = 100;
float lenbase = 1;
float gapbase = 10000;

void loadQueue(Queue *queue, int ms, int len, int gap)
{
  len = random(1, min(len, 6));
  ms = min(ms, 3000);
  gap = max(100, random(gap / 2, gap));
  // push delay between group
  queue->push(0);
  queue->push(gap);
  // push even-parity group of flicker ons and offs
  for (int i = 0; i < len; i++) {
    queue->push(random(10, min(ms, 2000)));
    queue->push(random(10, 250));
  }
}

void toggleLed(int led)
{
  if (states[led] == HIGH)  { 
    states[led] = LOW;
  } 
  else {
    states[led] = HIGH;
  }
  digitalWrite(leds[led], states[led]);
}

void setup()
{
  for (int i = 0; i < len; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], states[i]);
  }
}

void loop()
{
  for (int i = 0; i < len; i++) {
    if (queues[i].isEmpty()) {
      loadQueue(&queues[i], msbase, lenbase, gapbase);
      msbase = msbase * 1.1;
      gapbase = gapbase * 0.95;
      lenbase = lenbase * 1.1;
    }
    if (metros[i].check() == 1) {
      toggleLed(i);
      metros[i].interval(queues[i].pop());
    }
  }    
  delay(1);
}
