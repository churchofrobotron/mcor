void setup();

//#include <TimerOne.h>
#include <CapacitiveSensor.h>
#include "CapacitiveTouchSwitch.h"

#define USE_SERIAL

long threshold=35;

const uint8_t numSensors=8;

#ifdef TEENSY3
CapacitiveTouchSwitch sns0(0,threshold);
CapacitiveTouchSwitch sns1(1,threshold);
CapacitiveTouchSwitch sns2(23,threshold);
CapacitiveTouchSwitch sns3(22,threshold);
CapacitiveTouchSwitch sns4(19,threshold);
CapacitiveTouchSwitch sns5(18,threshold);
CapacitiveTouchSwitch sns6(17,threshold);
CapacitiveTouchSwitch sns7(16,threshold);
#else
const uint8_t samples=30;
CapacitiveTouchSwitch sns0(1,0,samples,threshold);
CapacitiveTouchSwitch sns1(1,2,samples,threshold);
CapacitiveTouchSwitch sns2(4,3,samples,threshold);
CapacitiveTouchSwitch sns3(4,5,samples,threshold);
CapacitiveTouchSwitch sns4(7,6,samples,threshold);
CapacitiveTouchSwitch sns5(7,8,samples,threshold);
CapacitiveTouchSwitch sns6(13,12,samples,threshold);
CapacitiveTouchSwitch sns7(13,14,samples,threshold);
#endif

struct KeyboardTouch {
  CapacitiveTouchSwitch *key;
  uint16_t keys[2];
};

struct KeyboardTouch keys[numSensors]= {
  {&sns6,  {KEY_E,     0}}, //up
  {&sns7,  {KEY_E, KEY_S}}, //up, left
  {&sns0,  {    0, KEY_S}}, //left
  {&sns1,  {KEY_D, KEY_S}}, //left, down
  {&sns2,  {KEY_D,     0}}, //down
  {&sns3,  {KEY_D, KEY_F}}, //down, right
  {&sns4, {    0, KEY_F}}, //right
  {&sns5, {KEY_E, KEY_F}} //right, up
};

int letters[4]={KEY_E, KEY_S, KEY_D, KEY_F}; //up, left, down, right

//touched key order
int touchKeys[numSensors];
int lastKeyIdx=0;

CapacitiveTouchSwitch *sensors[numSensors]={&sns0, &sns1, &sns2, &sns3, &sns4, &sns5, &sns6, &sns7};
#ifndef TEENSY3
const uint8_t led_pins[numSensors]={21,20,19,18,17,21,20,19};
#endif

int last_on() {
  int rVal=-1;
  int least=0x7FFF;
  for (int i=0; i<numSensors; i++) {
    int elapsed=lastKeyIdx-touchKeys[i];
#ifdef USE_SERIAL
    Serial.print(i);
    Serial.print(":");
    Serial.print(elapsed);
    Serial.print(", ");
#endif
    if (elapsed<least && keys[i].key->State()) {
      least=elapsed;
      rVal=i;
    }
  }
  return rVal;
}

void setKeysLeft(struct KeyboardTouch *key) {
  Keyboard.set_key1(key->keys[0]);
  Keyboard.set_key2(key->keys[1]);
  Keyboard.send_now();
}

void setKeysRight(struct KeyboardTouch *key) {
  Keyboard.set_key3(key->keys[0]);
  Keyboard.set_key4(key->keys[1]);
  Keyboard.send_now();
}

void setKeys(struct KeyboardTouch *leftKey, struct KeyboardTouch *rightKey, struct KeyboardTouch *startKey) {
  Keyboard.set_key1(0);
  Keyboard.set_key2(0);
  Keyboard.set_key3(0);
  Keyboard.set_key4(0);
  if (leftKey) {
    Keyboard.set_key1(leftKey->keys[0]);
    Keyboard.set_key2(leftKey->keys[1]);
  }
  if (rightKey) {
    Keyboard.set_key3(rightKey->keys[0]);
    Keyboard.set_key4(rightKey->keys[1]);
  }
  Keyboard.set_key5(startKey && startKey->key->State() ? startKey->keys[0] : 0);
  Keyboard.send_now();
}

//insert point above
void setup () {
  //Start serial. Baud rate is ignored with USB.
#ifdef USE_SERIAL
  Serial.begin(9600);
  Serial.println("1 8 Way");
#endif
  //set led pins as output low
  for (int i=0; i<numSensors; i++) {
#ifndef TEENSY3
    pinMode(led_pins[i],OUTPUT);
    digitalWrite(led_pins[i],LOW);
#endif
    //configure sensor Hysteresis and timeout
    CapacitiveTouchSwitch *key=keys[i].key;
    key->SetHysteresis(threshold/2); 
    key->SetTimeoutMs(60000); //5 seconds, for now
    key->SetBaselineAlways(false); //don't adjust baseline while on 
    touchKeys[i]=0;
  }
}

void print_stats () {
  for (int i=0; i<numSensors; i++) {
    Serial.print(keys[i].key->Diff());
    Serial.print(" ");
  }
  Serial.println();
}

void loop () {
  for (int i=0; i<numSensors; i++) {
    CapacitiveTouchSwitch *key=keys[i].key;
    key->Update();
    //look for a change in switch state
    if (key->Changed()) {
      if (key->State()) {
#ifndef TEENSY3
        //if changed and now on, send a MIDI note on event
        digitalWrite(led_pins[i],HIGH);
#endif
#ifdef USE_SERIAL
        Serial.print(i);
        Serial.print(" on ");
#endif
        touchKeys[i]=lastKeyIdx;
        lastKeyIdx++;
        setKeys(keys+i,0,0);
#ifdef USE_SERIAL
        Serial.println();
#endif
      } else {
#ifdef USE_SERIAL
        Serial.print(i);
        Serial.print(" off, last on ");
#endif
        int last_key=last_on();
#ifdef USE_SERIAL
        Serial.print(last_key);
        Serial.print(" ");
#endif
        if (last_key>=0) {
          setKeys(keys+last_key,0,0);
        } else {
          setKeys(0,0,0);
        }
#ifndef TEENSY3
        digitalWrite(led_pins[i],LOW);
#endif
#ifdef USE_SERIAL
        Serial.println();
#endif
      }
    }
  }
  //print_stats();
  //delay(100);
}

