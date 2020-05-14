/*
 *  Callback.cpp
 *
 *  Example for use of callback function at button press as well as double press detection.
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

#define USE_BUTTON_0  // Enable code for 1. button at INT0 (pin2)
#include "EasyButtonAtInt01.cpp.h"

void showButtonToggleState(bool aButtonToggleState);    // The callback function
EasyButton Button0AtPin2(&showButtonToggleState);       // Only 1. button (USE_BUTTON_0) enabled -> button is connected to INT0 (pin2)

#define VERSION_EXAMPLE "2.1"

#if defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#elif defined(ARDUINO_AVR_DIGISPARKPRO)
// On a Digispark Pro we have PB1 / D9 / PCB pin 1
#define LED_BUILTIN (9)
#elif ! defined(LED_BUILTIN)
#define LED_BUILTIN PB1 // define port of built in LED for your ATtiny
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__)
    while (!Serial)
        ; //delay for Leonardo
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nVersion " VERSION_EXAMPLE " from " __DATE__));
}

void loop() {
    delay(10);
}

/*
 * The callback function is called at each button press
 * Initial value is false, so first call is with true
 */
void showButtonToggleState(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
    /*
     * Double press (< 200 ms) detection by calling checkForForDoublePress() once at button press time.
     */
    if (Button0AtPin2.checkForDoublePress(300)) {
        Serial.println(F("Double press (< 300 ms) detected"));
    }
}
