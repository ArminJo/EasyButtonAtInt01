/*
 *  TwoButtons.ino
 *
 *  Example for two buttons
 *  Button 0 is using INT0.
 *  Button 1 is using INT1 or INT1_PIN with PinChangeInterrupt, if INT1_PIN is defined.
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
//#define BUTTON_DEBOUNCING_MILLIS 50 // With this you can adapt to the characteristic of your button. Default is 30.
//#define ANALYZE_MAX_BOUNCING_PERIOD // Analyze the button actual debounce value
//#define NO_BUTTON_RELEASE_CALLBACK  // Disables the code for release callback. This saves 2 bytes RAM and 64 bytes program memory.
#define BUTTON_LED_FEEDBACK         // This activates LED_BUILTIN as long as button is pressed.
//#define BUTTON_LED_FEEDBACK_PIN     // The pin to use for button LED feedback.
#define USE_BUTTON_0  // Enable code for button 0 at INT0.
#define USE_BUTTON_1  // Enable code for button 1 at INT1 or INT1_PIN

// definitions for ATtinies
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.hpp" // Available as Arduino library "ATtinySerialOut"
/*
 * Pin number scheme are from AttinyCore
 */
#  if (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)) && TX_PIN == PIN_PB2
#error Please change TX_PIN in ATtinySerialOut.h from PIN_PB2 to e.g. PIN_PB0 for use with this example
#  endif

#if !defined(LED_BUILTIN)
#define LED_BUILTIN     PIN_PB1 // define port of built in LED for your ATtiny board
#endif

#  if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#define INT1_PIN        7 // use PCINT7/PA7 instead of INT1/PA3 for button 1 on ATtiny167
#  endif // ATTiny type
#endif // ATTinies

#include "EasyButtonAtInt01.hpp"

void handleButtonPressForButton0(bool aButtonToggleState);    // The callback function for button 0
EasyButton Button0AtPin2(&handleButtonPressForButton0);       // Only callback parameter -> button is connected to INT0
EasyButton Button1AtPin3(BUTTON_AT_INT1_OR_PCINT);  // Button is connected to INT1 or INT1_PIN if INT1_PIN is defined.
#define LONG_PRESS_BUTTON_DURATION_MILLIS   1000
bool sLongPressMessageSent = false;

void setup() {
// initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/ \
    || defined(SERIALUSB_PID)  || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nUsing library version " VERSION_EASY_BUTTON " from " __DATE__));
    Serial.print(F("Button0 pin="));
    Serial.print(INT0_PIN);
    Serial.print(F(" Button1 pin="));
    Serial.println(INT1_PIN);
}

void loop() {

    /*
     * Check for long press of button 0
     */
    if (!sLongPressMessageSent
            && Button0AtPin2.checkForLongPress(LONG_PRESS_BUTTON_DURATION_MILLIS) == EASY_BUTTON_LONG_PRESS_DETECTED) {
        Serial.print(F("Long press of  "));
        auto tButtonPressDurationMillis = millis() - Button0AtPin2.ButtonLastChangeMillis;
        Serial.print(tButtonPressDurationMillis);
        Serial.print(F(" = "));
        Serial.print(millis());
        Serial.print(F(" - "));
        Serial.print(Button0AtPin2.ButtonLastChangeMillis);
        Serial.println(F(" ms just detected"));
        sLongPressMessageSent = true; // Print message only once per long press
    }

    /*
     * Button 1 - check manually here as demonstration, but it would be easier to just use a callback function like we do for button 0
     */
    if (Button1AtPin3.ButtonStateHasJustChanged) {
        Button1AtPin3.ButtonStateHasJustChanged = false; // Acknowledge button state change flag

        /*
         * Print new status
         */
        Serial.print(F("Button 1 IsActive="));
        Serial.print(Button1AtPin3.getStoredButtonState());
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

void handleButtonPressForButton0(bool aButtonToggleState) {
    sLongPressMessageSent = false; // reset flag

    /*
     * checkForDoublePress() works reliable only, if called early in press callback function
     */
    if (Button0AtPin2.checkForDoublePress()) {
        Serial.println(F("Button 0 double press (< 400 ms) detected"));
    }

    Serial.print(F("Button 0 ToggleState="));
    Serial.println(aButtonToggleState);
}
