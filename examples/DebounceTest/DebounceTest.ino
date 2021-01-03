/*
 *  DebounceTest.cpp
 *
 *  Test button with reduced debounce time.
 *  If one bounce is not suppressed, the internal LED sometimes toggles because of bouncing.
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
//#define MEASURE_INTERRUPT_TIMING

#define ANALYZE_MAX_BOUNCING_PERIOD
#define BUTTON_DEBOUNCING_MILLIS 2

#define USE_BUTTON_0  // Enable code for 1. button at INT0
#include "EasyButtonAtInt01.cpp.h"

EasyButton Button0AtPin2; // Only 1. button (USE_BUTTON_0) enabled -> button is connected to INT0

#if defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#elif defined(ARDUINO_AVR_DIGISPARKPRO)
// On a Digispark Pro we have PB1 / D9 / PCB pin 1
#define LED_BUILTIN (9)
#elif ! defined(LED_BUILTIN)
#define LED_BUILTIN PB1 // define port of built in LED for your ATtiny
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
void setup() {
// initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)
    delay(2000); // To be able to connect Serial monitor after reset and before first printout
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nUsing library version " VERSION_EASY_BUTTON " from " __DATE__));

    Serial.println(F("Button debouncing time is reduced to " STR(BUTTON_DEBOUNCING_MILLIS) " ms"));
    Serial.println(F("Please press the button and watch for \"Bouncing, MBP=...\" output at the Serial Monitor"));
}

void loop() {
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);

    if (Button0AtPin2.ButtonStateHasJustChanged) {
        Button0AtPin2.ButtonStateHasJustChanged = false; // Acknowledge button state change flag
        /*
         * Print new status
         */
        Serial.print(F("Button1 IsActive="));
        Serial.print(Button0AtPin2.ButtonStateIsActive);
        Serial.print(F(" ToggleState="));
        Serial.println(Button0AtPin2.ButtonToggleState);
    }
    delay(10);
}
