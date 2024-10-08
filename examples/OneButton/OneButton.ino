/*
 *  OneButton.cpp
 *
 *  Connect the button between pin 2 (ATtinyX7 pin 14) and ground.
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#include <Arduino.h>

//#define USE_ATTACH_INTERRUPT // enable it if you get the error " multiple definition of `__vector_1'" (or `__vector_2')

#define USE_BUTTON_0  // Enable code for 1. button at INT0 / D2

#include "EasyButtonAtInt01.hpp"

#if defined(ARDUINO_AVR_DIGISPARK)
#define LED_BUILTIN PB1
#elif defined(ARDUINO_AVR_DIGISPARKPRO)
// On a Digispark Pro we have PB1 / D9 / PCB pin 1
#define LED_BUILTIN (9)
#elif ! defined(LED_BUILTIN)
#define LED_BUILTIN PB1 // define port of built in LED for your ATtiny
#endif

EasyButton PageSwitchButton0AtPin2; // Only 1. button (USE_BUTTON_0) enabled -> button is connected to INT0

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
    /*
     * Manually check button state change
     * Using a callback function is sometimes easier, see EasyButtonExample and Callback example.
     */
    if (PageSwitchButton0AtPin2.ButtonStateHasJustChanged) {
        // reset flag in order to do call digitalWrite() only once per button press
        PageSwitchButton0AtPin2.ButtonStateHasJustChanged = false;
        digitalWrite(LED_BUILTIN, PageSwitchButton0AtPin2.ButtonToggleState);
    }
}
