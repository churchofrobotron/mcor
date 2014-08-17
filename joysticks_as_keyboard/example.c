/* Church of Robotron Joystick Firmware, Praise the Mutant! */

/* Keyboard example with debug channel, for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard_debug.h"
#include "print.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define NUM_BUTTONS 9
#define INPUT_B 0
#define INPUT_D 1

#define OUTPUT_E 2

// Which port does this button read from
const uint8_t keyPort[NUM_BUTTONS] = { INPUT_B, INPUT_D, INPUT_D, INPUT_D, INPUT_D, INPUT_D, INPUT_D, INPUT_D, INPUT_D };
// Which bit from the input port should we check
const uint8_t keyMask[NUM_BUTTONS] = { 1 << 7, 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7 };
// What key should we send from that button
const uint8_t key[NUM_BUTTONS] = { KEY_D, KEY_F, KEY_E, KEY_S, KEY_K, KEY_L, KEY_I, KEY_J, KEY_1 };
// What index into the keyboard state array should we use.  Selected these indexes to minimize conflicts.  For example, up/down share an index because they should never be down at the same time.  Keyboard state array is defined in usb_keyboard_debug, it allows for up to six keys to be down at a time.
const uint8_t keyIndex[NUM_BUTTONS] = { 0, 1, 0, 1, 2, 3, 2, 3, 4 };
// This is used to debounce the button presses and releases, a button must be in a state for 8 loops in order to
// transition to the new state.
uint8_t debounce[NUM_BUTTONS] = {0};

//output pins for left/right fans
typedef struct port_pin
{
  uint8_t port;
  uint8_t pin;
} port_pin;

const port_pin left_out={OUTPUT_E, 1<<0};
const port_pin right_out={OUTPUT_E, 1<<1};

int main(void)
{
  uint8_t values[3];
	uint8_t i;
  uint8_t temp;

	// set for 16 MHz clock
	CPU_PRESCALE(0);

	// Configure all port B and port D pins as inputs with pullup resistors
	// See the "Using I/O Pins" page for details.
	// http://www.pjrc.com/teensy/pins.html
  //DDRD
	DDRD = 0x00;
	DDRB = 0x00;
	PORTB = 0xFF;
	PORTD = 0xFF;
  DDRE=0x00;
  PORTE=0xFF;

  //set left/right pins to output low
  if (left_out.port==OUTPUT_E)
  {
    DDRE|=left_out.pin;
    PORTE&=~left_out.pin;
  }
  else
  {//wtf
  }

  if (right_out.port==OUTPUT_E)
  {
    DDRE|=right_out.pin;
    PORTE&=~right_out.pin;
  }
  else
  {//wtf
  }

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

   // Initialize the keyboard state to nothing pressed.
   keyboard_modifier_keys = 0;
   for (i=0; i < 5; i++)
      keyboard_keys[i] = 0;

 	while (1) {
		// read all port B and port D pins
      values[0] = PINB;
      values[1] = PIND;
      values[2] = PORTE;

      for (i=0; i<NUM_BUTTONS; i++)
      {
         // Shift the debounce value for this button to the left one bit
         debounce[i] <<= 1;
         if (values[keyPort[i]] & keyMask[i])
         {
            // Because we're using a pullup resistor, the value of the pin will be high
            // if the button doesn't tie the pin to ground.  So set the bottom bit to 1
            debounce[i] |= 1;
         } else {
            // Because we're using a pullup resistor, the value of the pin will be low
            // if the button is pressed and ties the pin to ground.  So set the bottom bit to 0
            debounce[i] &= 0xFE;
         }
         // If the key has been pressed down for eight loop iterations (8 bits in a byte), send the keydown.
         if (debounce[i] == 0x00)
         {
            // Set key down
            keyboard_keys[keyIndex[i]] = key[i];
            //turn on IO pin for left/right down
            if (key[i]==KEY_S) {//left
              values[left_out.port]|=left_out.pin;
            }
            if (key[i]==KEY_F) {//right
              values[right_out.port]|=right_out.pin;
            }

         } else {
            // If the key has been released for eight loop iterations, send keyup (if this key is currently marked as down)
           if (debounce[i] == 0xFF)
           {
             // Set key up
             if (keyboard_keys[keyIndex[i]] == key[i])
               keyboard_keys[keyIndex[i]] = 0;
             //turn off IO pin for left/right down
             if (key[i]==KEY_S) {//left
               values[left_out.port]&=~left_out.pin;
             }
             if (key[i]==KEY_F) {//right
               values[right_out.port]&=~right_out.pin;
             }
           }
         }
      }
      //set output ports to updated values
      PORTE=values[2];
      // Send keyboard_keys and keyboard_modifier_keys to host pc
      usb_keyboard_send();
	}
}
