/*
 *  DebounceTest.cpp
 *
 *  Test button with reduced debounce time.
 *  If one bounce is not suppressed the internal LED stays active
 *  and the toggle output at pin 12 does not seem to toggle (it toggles twice, but you may not see it).
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

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#if ! defined(LED_BUILTIN) && defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#endif
#endif

//#define USE_ATTACH_INTERRUPT
//#define MEASURE_TIMING

#define BUTTON_DEBOUNCING_MILLIS 2
#define LED_FEEDBACK_FOR_DEBOUNCE_TEST
#define BUTTON_TEST_FEEDBACK_LED_PIN LED_BUILTIN

#define USE_BUTTON_0  // Enable code for Button at PCINT0
#include "EasyButtonAtInt01.h"

EasyButton Button0AtPin2(true); // true  -> Button is connected to PCINT0

#define VERSION_EXAMPLE "1.0"

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

    pinMode(12, OUTPUT);
}

void loop() {
    digitalWrite(12, Button0AtPin2.ButtonToggleState);
}
