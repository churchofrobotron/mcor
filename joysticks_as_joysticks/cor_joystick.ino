/* Complete USB Joystick Example
   Teensy becomes a USB joystick with 16 or 32 buttons and 6 axis input

   You must select Joystick from the "Tools > USB Type" menu

   Pushbuttons should be connected between the digital pins and ground.
   Potentiometers should be connected to analog inputs 0 to 5.

   This example code is in the public domain.
*/

// Configure the number of buttons.  Be careful not
// to use a pin for both a digital button and analog
// axis.  The pullup resistor will interfere with
// the analog voltage.
const int numButtons = 16;  // 16 for Teensy, 32 for Teensy++
unsigned long lastTime;

const int right_up = 6;

void setup() {
  // you can print to the serial monitor while the joystick is active!
  Serial.begin(9600);
  // configure the joystick to manual send mode.  This gives precise
  // control over when the computer receives updates, but it does
  // require you to manually call Joystick.send_now().
  Joystick.useManualSend(true);
  for (int i=0; i<numButtons; i++) {
    pinMode(i, INPUT_PULLUP);
    Joystick.button(1, 0);    
  }
  Joystick.X(512);
  Joystick.Y(512);
  lastTime = millis();
  Serial.println("Begin CoR Joystick");
}

byte allButtons[numButtons];
byte prevButtons[numButtons];

byte send_button[numButtons] = { 
  0, 0, 0, 0, 
  3, 2, 1, 10, 
  4, 0, 0, 0, 
  0, 0, 0, 0 };

void loop() {  
  // Right hand side + start button
  
  // read digital pins and use them for the buttons
  for (int i=4; i<9; i++) {
    if (digitalRead(i)) {
      // when a pin reads high, the button is not pressed
      // the pullup resistor creates the "on" signal
      allButtons[i] = 0;
    } else {
      // when a pin reads low, the button is connecting to ground.
      allButtons[i] = 1;
    }
    if (send_button[i] != 0)
    {
      Joystick.button(send_button[i], allButtons[i]);
    }
  }
  
  const int left_pin = 2;
  const int right_pin = 1;
  const int up_pin = 3;
  const int down_pin = 0;
  
  int xValue = (!digitalRead(left_pin)) ? 0 : ((!digitalRead(right_pin)) ? 1023 : 512);
  int yValue = (!digitalRead(up_pin)) ? 0 : ((!digitalRead(down_pin)) ? 1023 : 512);
  
  Joystick.X(xValue);
  Joystick.Y(yValue);

  if ((yValue == 0) && (allButtons[right_up]))
  {
    if (millis() - lastTime > 5000)
    {
      Serial.println("Sending start");
      Joystick.button(10, 1);
      if (millis() - lastTime > 5010)
        lastTime = millis();
    }    
  } else {
    lastTime = millis();
  }
      
  // Because setup configured the Joystick manual send,
  // the computer does not see any of the changes yet.
  // This send_now() transmits everything all at once.
  Joystick.send_now();
  
  // check to see if any button changed since last time
  boolean anyChange = false;
  for (int i=0; i<numButtons; i++) {
    if (allButtons[i] != prevButtons[i]) anyChange = true;
    prevButtons[i] = allButtons[i];
  }
  
  // if any button changed, print them to the serial monitor
  if (anyChange) {
    Serial.print("Buttons: ");
    for (int i=0; i<numButtons; i++) {
      if ((i != 0) && ((i & 3) == 0))
        Serial.print(" ");
      Serial.print(allButtons[i], DEC);
    }
    Serial.println();
  }
  
  // a brief delay, so this runs "only" 200 times per second
  delay(5);
}

