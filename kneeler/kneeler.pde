#include <FlexiTimer2.h>

/* USB to Serial - Teensy becomes a USB to Serial converter
   http://dorkbotpdx.org/blog/paul/teensy_as_benito_at_57600_baud

   You must select Serial from the "Tools > USB Type" menu

   This example code is in the public domain.
*/


const int input_pin = 0;  //teensy 2.0

#define INPUT_DEBOUNCE_MS 200
#define MY_TYPE "KNEELER"
#define MY_ADDR "6"
#define SEPARATOR ":"
String preamble=MY_TYPE MY_ADDR SEPARATOR;
String command;

//#define DEBUG
const unsigned int baud=9600;
unsigned long debounce_timeout = 0;
int last_state = 0;

void setup() {
  pinMode(input_pin, INPUT);
  digitalWrite(input_pin, HIGH);
  Serial.begin(baud);	// USB, communication to PC or Mac
}

void loop() {
  unsigned long time = millis();

  int state = !digitalRead(input_pin);
  if (state != last_state) {
    debounce_timeout = time + INPUT_DEBOUNCE_MS;
    last_state = state;
  } else if (debounce_timeout && debounce_timeout >= time) {
    debounce_timeout = 0;
    if (state)
      Serial.println("1\n");
    else
      Serial.println("0\n");
  }
}

