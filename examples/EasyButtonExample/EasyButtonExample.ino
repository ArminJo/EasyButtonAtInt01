/*
 *  EasyButtonExample.cpp
 *
 *  Example for one or two buttons using PA7/PCINT0 for button 1 on an ATtiny167 and PB3//PCINT for button 1 on an ATtiny*5
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
//#define BUTTON_DEBOUNCING_MILLIS 80

#define USE_BUTTON_0  // Enable code for 1. button at INT0
//#define USE_BUTTON_1  // Enable code for 2. button at INT1 or PCINT[0:7]
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#include "ATtinyUtils.h" // for changeDigisparkClock()
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) && TX_PIN == PB2
#error "Please change TX_PIN in ATtinySerialOut.h from PB2 to e.g. PB0 for use with this example"
#endif
#  if defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#  elif defined(ARDUINO_AVR_DIGISPARKPRO)
#define LED_BUILTIN (9) // On a Digispark Pro we have PB1 / D9 / PCB pin 1
#define INT1_PIN 7   // use PCINT7/PA7 instead of INT1/PA3 for button 1 on ATtiny167
#define INTENTIONALLY_USE_PCI0_FOR_BUTTON1  // yes we know that we use the PCINT[0:7] for button 1. It is no typo.
#  else
#undef LED_BUILTIN
#define LED_BUILTIN PB1 // define pin of built in LED for your ATtiny
#  endif
#endif

#include "EasyButtonAtInt01.h"

EasyButton Button1AtPin3(true);  // true  -> button is connected to INT0
#ifdef USE_BUTTON_1
bool sButtonLongPressDetected = false;
// The callback function for button 2
void printButtonToggleState(bool aButtonToggleState);
EasyButton Button2AtPA7(false, &printButtonToggleState); // false -> button is not connected to INT0 but connected to INT1 or PCINT[0:7]
#endif

#define VERSION_EXAMPLE "2.0"

long sOldDeltaMillis;

void setup() {
// initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

#if defined(ARDUINO_AVR_DIGISPARK) || defined(ARDUINO_AVR_DIGISPARKPRO)
    changeDigisparkClock();
#endif

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__)
    while (!Serial)
        ; //delay for Leonardo
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nVersion " VERSION_EXAMPLE " from " __DATE__));

}

void loop() {

    /*
     * Button 1
     */
    Button1AtPin3.updateButtonState(); // this is ONLY needed if we expect a regular button press which is shorter than BUTTON_DEBOUNCING_MILLIS!
    if (Button1AtPin3.ButtonStateHasJustChanged) {
        Button1AtPin3.ButtonStateHasJustChanged = false; // Acknowledge button state change flag

        digitalWrite(LED_BUILTIN, Button1AtPin3.ButtonToggleState);

        /*
         * Print new status
         */
        Serial.print(F("Button1 IsActive="));
        Serial.print(Button1AtPin3.ButtonStateIsActive);
        Serial.print(F(" ToggleState="));
        Serial.print(Button1AtPin3.ButtonToggleState);
        if (!Button1AtPin3.readDebouncedButtonState()) {
            Serial.print(F(" press duration="));
            Serial.print(Button1AtPin3.ButtonPressDurationMillis);
            Serial.print(F(" ms"));
        }
        if (Button1AtPin3.MaxBouncingPeriodMillisHasJustChanged) {
            Button1AtPin3.MaxBouncingPeriodMillisHasJustChanged = false;
            Serial.print(F(" MaxBouncingPeriod="));
            Serial.print(Button1AtPin3.MaxBouncingPeriodMillis);
        }
        Serial.println();
    }

#ifdef USE_BUTTON_1
    /*
     * Button 2 uses a callback function
     *
     * Long press (> 500 ms) detection by polling function checkForLongPress()
     */
    if (!sButtonLongPressDetected && Button2AtPA7.checkForLongPress(500) == EASY_BUTTON_LONG_PRESS_DETECTED) {
        sButtonLongPressDetected = true;
        Serial.println(F("Button2 long press (> 500 ms) detected"));
    }
#endif
}

#ifdef USE_BUTTON_1
void printButtonToggleState(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);

    Serial.print("Button2 ToggleState=");
    Serial.print(aButtonToggleState);
    sButtonLongPressDetected = false;
    if (Button2AtPA7.MaxBouncingPeriodMillisHasJustChanged) {
        Button2AtPA7.MaxBouncingPeriodMillisHasJustChanged = false;
        Serial.print(F(" MaxBouncingPeriod="));
        Serial.print(Button2AtPA7.MaxBouncingPeriodMillis);
    }
    Serial.println();
}
#endif
