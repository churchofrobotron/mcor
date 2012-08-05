#include <MIDI.h>
#define INPUT_DEBOUNCE_MS 50

//teensy 2.0
const int input_pin = 0;
const int led_pin = 11;

unsigned long debounce_timeout = 0;
int last_state = 0;

void setup() {
  pinMode(input_pin, INPUT);
  digitalWrite(input_pin, HIGH);

  pinMode(led_pin, OUTPUT);
}

void loop() {
  unsigned long time = millis();

  int state = !digitalRead(input_pin);
  if (state != last_state) {
    debounce_timeout = time + INPUT_DEBOUNCE_MS;
    last_state = state;
  } else if (debounce_timeout && debounce_timeout <= time) {
    debounce_timeout = 0;
    usbMIDI.sendControlChange(1, (state ? 127 : 0), 1);
    digitalWrite(led_pin, state);
  }
}

