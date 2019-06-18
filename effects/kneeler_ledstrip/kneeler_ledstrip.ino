#include <Adafruit_NeoPixel.h>

#define OUT_PIN 3
//#define NUM_LEDS 144
#define NUM_LEDS 72

const int kneelPin = 38;     // the number of the pushbutton pin
const int ledPin =  6;      // the number of the LED pin

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, OUT_PIN, NEO_GRB + NEO_KHZ800);

int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
const long debounceDelay = 50;    // the debounce time; increase if the output flickers

long lastDisplayTime = 0;  // the last time the output pin was toggled
const long ledDelayTime = 200;  // the last time the output pin was toggled

const uint8_t fade = 32;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(kneelPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

//pink blue green yellow
//uint8_t colors[4 * 3] = {255, 0, 255, 0, 0, 255, 0, 255, 0, 255, 255, 0};
uint8_t colors[4 * 3] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0};
uint8_t cindex = 0;
int16_t onval = 255;

void loop() {
  if ((millis() - lastDisplayTime) > ledDelayTime) {
    for (int i = 0; i < 4; i++) {
      uint8_t off = (((cindex + i) % 4) * 3);
      uint32_t c = strip.Color(colors[off] * onval, colors[off + 1] * onval, colors[off + 2] * onval);
      for (int j = 0; j < 9; j++) {
        uint8_t l = j + i * 9; 
        strip.setPixelColor(l, c);
        strip.setPixelColor(71 - l, c);
      }
    }
    strip.show();
    lastDisplayTime = millis();
    cindex = (cindex + 1) % 4;
    onval = onval + (buttonState ? fade : -fade);

    if (onval > 255)
      onval = 255;
    else if (onval < 0)
      onval = 0;
  }

  // read the state of the switch into a local variable:
  int reading = digitalRead(kneelPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  } else if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }
  lastButtonState = reading;
  digitalWrite(ledPin, buttonState);
}

void all(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

