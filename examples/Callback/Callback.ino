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
#include "EasyButtonAtInt01.hpp"

void handleButtonPress(bool aButtonToggleState);    // The button press callback function
void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis);
EasyButton Button0AtPin2(&handleButtonPress, &handleButtonRelease); // Button is connected to INT0 (pin2)

#if defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#elif defined(ARDUINO_AVR_DIGISPARKPRO)
// On a Digispark Pro we have PB1 / D9 / PCB pin 1
#define LED_BUILTIN (9)
#elif ! defined(LED_BUILTIN)
#define LED_BUILTIN PB1 // define port of built in LED for your ATtiny
#endif
#define BLINK_SHORT_MILLIS 200
#define BLINK_LONG_MILLIS 600

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) || defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nUsing library version " VERSION_EASY_BUTTON " from " __DATE__));
}

void loop() {
    delay(10);
}

void blinkLEDBlocking(uint8_t aLedPin, uint8_t aBlinkCount, uint16_t aDelay) {
    for (int i = 0; i < aBlinkCount; ++i) {
        digitalWrite(aLedPin, HIGH);
        delay(aDelay);
        digitalWrite(aLedPin, LOW);
        delay(aDelay);
    }
}

/*
 * The callback function is called at each button press
 * Initial value is false, so first call is with true
 */
void handleButtonPress(bool aButtonToggleState) {
    /*
     * Double press (< 200 ms) detection by calling checkForForDoublePress() once at button press time.
     */
    if (Button0AtPin2.checkForDoublePress(300)) {
        Serial.print(F("Double press "));
        Serial.print(Button0AtPin2.ButtonLastChangeMillis - Button0AtPin2.ButtonReleaseMillis);
        Serial.println(F(" ms detected"));

        // let the led blink twice short
        blinkLEDBlocking(2, LED_BUILTIN, BLINK_SHORT_MILLIS);
        Button0AtPin2.ButtonToggleState = false;
    }
    Serial.println(F("Button pressed"));
    digitalWrite(LED_BUILTIN, aButtonToggleState);
}

void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
    Serial.println(F("Button released"));

    /*
     * Simple long press (> 400 ms) detection
     */
    if (aButtonPressDurationMillis >= EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS) {
        Serial.print(F("Long press "));
        Serial.print(aButtonPressDurationMillis);
        Serial.println(F(" ms detected"));

        // let the led blink long
        blinkLEDBlocking(2, LED_BUILTIN, BLINK_LONG_MILLIS);
        Button0AtPin2.ButtonToggleState = false;
    }
}
