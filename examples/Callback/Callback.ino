/*
 *  Callback.ino
 *
 *  Example for use of callback function at button press as well as double press detection.
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

#define USE_BUTTON_0  // Enable code for 1. button at INT0 (pin2)
#include "EasyButtonAtInt01.hpp"

void handleButtonPress(bool aButtonToggleState);    // The button press callback function
void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis);
EasyButton Button0AtPin2(&handleButtonPress, &handleButtonRelease); // Button is connected to INT0 (pin2)

#if !defined(LED_BUILTIN) && (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__))
#define LED_BUILTIN     PIN_PB1 // define port of built in LED for your ATtiny board
#endif

#define BLINK_SHORT_MILLIS 500
#define BLINK_LONG_MILLIS 1000

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/ \
    || defined(SERIALUSB_PID)  || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nUsing library version " VERSION_EASY_BUTTON " from " __DATE__));
    Serial.print(F("Button pin="));
    Serial.println(INT0_PIN);
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

/**
 * The callback function is called at each button press
 * @param aButtonToggleState Initial value is false, so first call is with true
 */
void handleButtonPress(bool aButtonToggleState) {
    Serial.println(F("Button pressed"));
    digitalWrite(LED_BUILTIN, aButtonToggleState);

    /*
     * Double press (< 300 ms) detection by calling checkForDoublePress() once at button press time.
     */
    if (Button0AtPin2.checkForDoublePress(300)) {
        Serial.print(F("Double press "));
        Serial.print(Button0AtPin2.ButtonLastChangeMillis - Button0AtPin2.ButtonReleaseMillis);
        Serial.println(F(" ms detected -> blink 2 times short"));

        /*
         * Let the led blink long.
         * This blocks the ISR!
         * But because we have interrupts enabled
         * even new button presses during this period are detected and printed.
         * Only stack usage grows!
         */
            blinkLEDBlocking(LED_BUILTIN, 2, BLINK_SHORT_MILLIS);
        Button0AtPin2.ButtonToggleState = false; // set toggle state according to LED level
    }

}

/*
 * Print if press was longer than 400 ms
 */
void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
    Serial.print(F("Button released, duration="));
    Serial.print(aButtonPressDurationMillis);
    Serial.println(F(" ms"));

    /*
     * Simple long press (> 400 ms) detection
     */
    if (aButtonPressDurationMillis >= EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS) {
        Serial.println(F("Long press -> blink 2 times long"));
        /*
         * Let the led blink long.
         * This blocks the ISR!
         * But because we have interrupts enabled
         * even new button presses during this period are detected and printed.
         * Only stack usage grows!
         */
        blinkLEDBlocking(LED_BUILTIN, 2, BLINK_LONG_MILLIS); // 1000 ms
        Button0AtPin2.ButtonToggleState = false; // set toggle state according to LED level
    }
}
