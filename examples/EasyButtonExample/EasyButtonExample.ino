/*
 *  EasyButtonExample.cpp
 *
 *  Example for one or two buttons
 *  Button1 using PCINT0 - PA7 on an ATtiny167 and
 *
 *  Copyright (C) 2018  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of EasyButtonAtInt01 https://github.com/ArminJo/EasyButtonAtInt01.
 *
 *  EasyButtonAtInt01 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#include <Arduino.h>

//#define USE_ATTACH_INTERRUPT // enable it if you get the error " multiple definition of `__vector_1'" (or `__vector_2')
//#define BUTTON_DEBOUNCING_MILLIS 80 // With this you can adapt to the characteristic of your button. Default is 50.
//#define ANALYZE_MAX_BOUNCING_PERIOD //  Analyze the button actual debounce value

#define USE_BUTTON_0  // Enable code for button 0 at INT0.
#define USE_BUTTON_1  // Enable code for button 1 at INT1 or PCINT[0:7]

// definitions for ATtinies
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"

#  if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) && TX_PIN == PB2
#error "Please change TX_PIN in ATtinySerialOut.h from PB2 to e.g. PB0 for use with this example"
#  endif

#  if defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#define INT1_PIN 1 // use PB2 for button 1

#  elif defined(ARDUINO_AVR_DIGISPARKPRO)
#undef LED_BUILTIN    // In case we use another core e.g. in Eclipse
#define LED_BUILTIN 9 // On a Digispark Pro we have PB1 / D9 / PCB pin 1

#define INT1_PIN 7   // use PCINT7/PA7 instead of INT1/PA3 for button 1 on ATtiny167
//#define INTENTIONALLY_USE_PCI0_FOR_BUTTON1  // yes we know that we use the PCINT[0:7] for button 1. It is no typo.

#  else
#undef LED_BUILTIN
#define LED_BUILTIN PB1 // define pin of built in LED for your ATtiny
#  endif // ATTiny type
#endif // ATTinies

#include "EasyButtonAtInt01.cpp.h"

// The callback function for button 1
void handleButtonPress(bool aButtonToggleState);
EasyButton Button0AtPin2(true, &handleButtonPress); // true  -> button is connected to INT0
EasyButton Button1AtPin3((bool) false); // false -> button is not connected to INT0 but connected to INT1 or PCINT[0:7]. (bool) to avoid overloaded warning for digispark compiler.

#define VERSION_EXAMPLE "3.0"

long sOldDeltaMillis;

void setup() {
// initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__)
    while (!Serial)
        ; //delay for Leonardo
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nVersion " VERSION_EXAMPLE " from " __DATE__));
    Serial.println(F("Using library version " VERSION_EASY_BUTTON));
}

void loop() {

    /*
     * Button 1 - check manually.
     * But it is easier to just use a callback function as we do for button 0
     */
    Button1AtPin3.updateButtonState(); // this is ONLY required if we expect a regular button press which is shorter than BUTTON_DEBOUNCING_MILLIS!
    if (Button1AtPin3.ButtonStateHasJustChanged) {
        Button1AtPin3.ButtonStateHasJustChanged = false; // Acknowledge button state change flag

        digitalWrite(LED_BUILTIN, Button1AtPin3.ButtonToggleState);

        /*
         * Print new status
         */
        Serial.print(F("Button 1 IsActive="));
        Serial.print(Button1AtPin3.ButtonStateIsActive);
        Serial.print(F(" ToggleState="));
        Serial.print(Button1AtPin3.ButtonToggleState);
        if (!Button1AtPin3.readDebouncedButtonState()) {
            Serial.print(F(" press duration="));
            Serial.print(Button1AtPin3.ButtonPressDurationMillis);
            Serial.print(F(" ms"));
        }
        Serial.println();
    }
}

void handleButtonPress(bool aButtonToggleState) {
    /*
     * This function works reliable only if called early in press callback function
     */
    if (Button0AtPin2.checkForDoublePress()) {
        Serial.println(F("Button 0 double press (< 400 ms) detected"));
    }

    digitalWrite(LED_BUILTIN, aButtonToggleState);

    Serial.print("Button 0 ToggleState=");
    Serial.println(aButtonToggleState);
}
