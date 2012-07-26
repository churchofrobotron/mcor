#include <Metro.h>
#include <Queue.h>


struct effect {
  float onbase;
  float onmax;
  float lenbase;
  float lenmax;
  float gapbase;
  float gapmin;
  float offmin;
  float offmax;
};

const int len = 4;
effect *effects;
Metro *metros;
Queue *queues;
int pins[len] = {9, 10, 12, 13};
int states[len];

static float msbase = 100;
static float msmax = 1500;
static float lenbase = 1;
static float lenmax = 6;
static float gapbase = 20000;
static float gapmin = 750;
static float offmin = 10;
static float offmax = 250;

void loadQueue(Queue *queue, int ms, int len, int gap)
{
  len = random(0, len);
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
  free(metros);
  free(queues);
  free(effects);
  effects = (effect *) malloc(sizeof(effect) * len);
  metros = (Metro *) malloc(sizeof(Metro) * len);
  queues = (Queue *) malloc(sizeof(Queue) * len);
  for (int i = 0; i < len; i++) {
    metros[i] = Metro(pow(10, i+2), true);
    queues[i] = Queue();
    pinMode(pins[i], OUTPUT);
    states[i] = LOW;
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
