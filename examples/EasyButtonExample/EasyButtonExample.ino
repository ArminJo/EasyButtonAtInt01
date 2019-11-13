/*
 *  EasyButtonExample.cpp
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
//#define MEASURE_TIMING

//#define BUTTON_DEBOUNCING_MILLIS 80
//#define LED_FEEDBACK_FOR_DEBOUNCE_TEST

#define USE_BUTTON_0
//#define USE_BUTTON_1
#include "EasyButtonAtInt01.h"

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#include "ATtinySerialOut.h"
#endif

EasyButton Button0AtPin2(true);
#ifdef USE_BUTTON_1
EasyButton Button0AtPin3(false);
#endif

#define VERSION_EXAMPLE "1.0"

bool sOldButton0State; // negative logic: true means button pin is LOW
bool sOldButton0StateHasChanged;
bool sOldButton0ToggleState; // toggle on press, not on release

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
    delay(10); // delay starts output with the next print and not in the middle of the prints, since the interrupt happened during the delay
    printIfChangedAndUpdate(&sOldButton0StateHasChanged, Button0AtPin2.ButtonStateHasJustChanged, "StateHasChanged=");
    printIfChangedAndUpdate(&sOldButton0State, Button0AtPin2.ButtonStateIsActive, "State=");
    printIfChangedAndUpdate(&sOldButton0ToggleState, Button0AtPin2.ButtonToggleState, "ToggleState=");
    // Acknowledge change flag
    if (Button0AtPin2.ButtonStateHasJustChanged) {
        Button0AtPin2.ButtonStateHasJustChanged = false;
        sOldButton0StateHasChanged = false;
    }
#ifndef LED_FEEDBACK_FOR_DEBOUNCE_TEST
    digitalWrite(LED_BUILTIN,Button0AtPin2.ButtonToggleState);
#endif
}
