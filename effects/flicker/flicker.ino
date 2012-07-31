#include <Metro.h>
#include <Queue.h>

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

static const int len = 4;
static effect effects[] = {effect(), effect(), effect(), effect()};
static Metro metros[] = {Metro(6400), Metro(3200), Metro(1600), Metro(800)};
static Queue queues[] = {Queue(), Queue(), Queue(), Queue()};
static const int pins[] = {9, 10, 13, 14};
static int states[] = {LOW, LOW, LOW, LOW};

void loadQueue(int q)
{
  // push even parity group of ons and offs
  for (int i = 0; i < random(0, effects[q].lenbase); i++) {
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

void setup()
{
  for (int i = 0; i < len; i++) {
    pinMode(pins[i], OUTPUT);
    states[i] = LOW;
    digitalWrite(pins[i], states[i]);
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
    // rest of the effects are fake arcers
    effects[i].onbase = 100;
    effects[i].onmin = 3;
    effects[i].onmax = 1250;
    effects[i].lenbase = 1;
    effects[i].lenmax = 8;
    effects[i].gapbase = 20000;
    effects[i].gapmin = 750;
    effects[i].offmin = 10;
    effects[i].offmax = 200;
  }
}

void loop()
{
  for (int i = 0; i < len; i++) {
    if (queues[i].isEmpty()) {
      // We've used up this sequence of firings, speed up and load another.
      effects[i].onbase = min(effects[i].onbase * 1.05, effects[i].onmax);
      effects[i].lenbase = min(effects[i].lenbase * 1.05, effects[i].lenmax);
      effects[i].gapbase = max(effects[i].gapbase * 0.97, effects[i].gapmin);
      loadQueue(i);
    }
    if (metros[i].check() == 1) {
      // The metro has fired.  Toggle its pin and tell it to fire when
      // the pin should be toggled again.
      togglePin(i);
      metros[i].reset();        // XXX necessary?
      metros[i].interval(queues[i].pop());
    }
  }    
  //delay(1);
}
