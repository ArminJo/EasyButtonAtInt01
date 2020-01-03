/*
 *  EasyButtonExample.cpp
 *
 *  Example for one or two buttons using PA7/PCINT0 for button 1 on an ATtiny167
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

//#define USE_ATTACH_INTERRUPT
//#define MEASURE_INTERRUPT_TIMING

//#define BUTTON_DEBOUNCING_MILLIS 80
//#define LED_FEEDBACK_FOR_DEBOUNCE_TEST

#define USE_BUTTON_0  // Enable code for button at INT0
//#define USE_BUTTON_1  // Enable code for button at INT1
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#  if defined(ARDUINO_AVR_DIGISPARK)
#undef LED_BUILTIN
#define LED_BUILTIN PB1
#  elif defined(ARDUINO_AVR_DIGISPARKPRO)
// On a Digispark Pro we have PB1 / D9 / PCB pin 1
#undef LED_BUILTIN
#define LED_BUILTIN (9)
#define INT1_PIN 7   // PA7 use another PCINT0 pin for button 1 on ATtiny167
#define INTENTIONALLY_USE_PCINT0_FOR_BUTTON1  // yes we know that we use the PCINT0 for button 1. It is no mistake.
#  endif
#endif

#include "EasyButtonAtInt01.h"

EasyButton Button0AtPin3(true);  // true  -> button is connected to INT0
#ifdef USE_BUTTON_1
// The callback function for button1
void printButtonToggleState(bool aButtonToggleState) {
    Serial.print("Button1 ToggleState=");
    Serial.println(aButtonToggleState);
}
EasyButton Button0AtPA7(false, &printButtonToggleState); // false -> button is not connected to INT0 but connected to INT1
#endif

#define VERSION_EXAMPLE "1.0"

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

}

void printIfChangedAndUpdate(bool * aOldValue, bool aNewValue, const char * aString) {
    if (*aOldValue != aNewValue) {
        *aOldValue = aNewValue;
        Serial.print(aString);
        Serial.println(aNewValue);
    }
}

void loop() {
    static bool sOldButton0State; // negative logic: true means button pin is LOW
    static bool sOldButton0StateHasChanged;
    static bool sOldButton0ToggleState; // toggle on press, not on release

    delay(10); // delay starts output with the next print and not in the middle of the prints, since the interrupt happened during the delay
    printIfChangedAndUpdate(&sOldButton0StateHasChanged, Button0AtPin3.ButtonStateHasJustChanged, "StateHasChanged=");
    printIfChangedAndUpdate(&sOldButton0State, Button0AtPin3.ButtonStateIsActive, "State=");
    printIfChangedAndUpdate(&sOldButton0ToggleState, Button0AtPin3.ButtonToggleState, "ToggleState=");
    // Acknowledge button state change flag
    if (Button0AtPin3.ButtonStateHasJustChanged) {
        Button0AtPin3.ButtonStateHasJustChanged = false;
        sOldButton0StateHasChanged = false;
    }
#ifndef LED_FEEDBACK_FOR_DEBOUNCE_TEST
    digitalWrite(LED_BUILTIN, Button0AtPin3.ButtonToggleState);
#endif
}
