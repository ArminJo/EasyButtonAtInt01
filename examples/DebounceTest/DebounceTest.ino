/*
 *  DebounceTest.ino
 *
 *  Test button with reduced debounce time.
 *  If one bounce is not suppressed, the internal LED sometimes toggles because of bouncing.
 *
 *  Copyright (C) 2018-2026  Armin Joachimsmeyer
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.
 *
 */

#include <Arduino.h>

//#define USE_ATTACH_INTERRUPT // enable it if you get the error " multiple definition of `__vector_1'" (or `__vector_2')
//#define MEASURE_EASY_BUTTON_INTERRUPT_TIMING

#define ANALYZE_MAX_BOUNCING_PERIOD
//#define BUTTON_DEBOUNCING_MILLIS 10 // test if button works reliable with low value

#define USE_BUTTON_0  // Enable code for 1. button at INT0
#include "EasyButtonAtInt01.hpp"

EasyButton Button0AtPin2; // Only 1. button (USE_BUTTON_0) enabled -> button is connected to INT0

#if !defined(LED_BUILTIN) && (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__))
#define LED_BUILTIN     PIN_PB1 // define port of built in LED for your ATtiny board
#endif

// Helper macro for getting a macro definition as string
#if !defined(STR_HELPER) && !defined(STR)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif

void setup() {
// initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

#if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/ \
    || defined(SERIALUSB_PID)  || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_attiny3217)
    // Wait until Serial Monitor is attached.
    // Required for boards using USB code for Serial like Leonardo.
    // Is void for USB Serial implementations using external chips e.g. a CH340.
    while (!Serial)
        ;
    // !!! Program will not proceed if no Serial Monitor is attached !!!
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nUsing library version " VERSION_EASY_BUTTON " from " __DATE__));
    Serial.print(F("Button pin="));
    Serial.println(INT0_PIN);
    Serial.println(F("Button retriggerable debouncing time is " STR(BUTTON_DEBOUNCING_MILLIS) " ms"));
#if defined(ANALYZE_MAX_BOUNCING_PERIOD)
    Serial.println(F("Please press the button and watch for \"MaxBouncingPeriod=...\" output at the Serial Monitor"));
#endif
}

void loop() {
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);

    if (Button0AtPin2.ButtonStateHasJustChanged) {
        Button0AtPin2.ButtonStateHasJustChanged = false; // Acknowledge button state change flag
        /*
         * Print new status
         */
        Serial.print(F("Button0 IsActive="));
        Serial.print(Button0AtPin2.getStoredButtonState());
        Serial.print(F(" ToggleState="));
        Serial.println(Button0AtPin2.ButtonToggleState);
    }
    delay(10);
}
