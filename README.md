# EasyButton
Arduino library for handling push buttons connected between ground and INT0 and / or INT1 pin (pin 2 / 3).<br/>
The library is totally based on interrupt and debouncing is implemented in a not blocking way. 
It is merely done by ignoring a button change within the debouncing time.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/EasyButtonAtInt01.svg?)](https://www.ardu-badge.com/EasyButtonAtInt01)
[![Build Status](https://travis-ci.org/ArminJo/EasyButtonAtInt01.svg?branch=master)](https://travis-ci.org/ArminJo/EasyButtonAtInt01)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FEasyButtonAtInt01)](https://github.com/brentvollebregt/hit-counter)

## Usage
To use a single button, it needs only:

```
#define USE_BUTTON_0 // Enable code for Button at PCINT0
#include "EasyButtonAtInt01.h"
EasyButton Button0AtPin2(true); // true -> Button is connected to PCINT0

void setup() {}
void loop() {
...
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
...
}
```
To use 2 buttons, it needs only:
```
#define USE_BUTTON_0 // Enable code for Button at PCINT0
#define USE_BUTTON_1 // Enable code for Button at PCINT1
#include "EasyButtonAtInt01.h"
EasyButton Button0AtPin2(true);  // true  -> Button is connected to PCINT0
EasyButton Button0AtPin3(false); // false -> Button is not connected to PCINT0 => connected to PCINT1

void setup() {}
void loop() {
...
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
    delay(50);
    digitalWrite(LED_BUILTIN, Button0AtPin3.ButtonToggleState);
    delay(50);
...
}
```

## Usage of callback function
The callback function is is called on every button press with ButtonToggleState as parameter.<br/>
Before callback function is called, interrupts are enabled! This allows the timer interrupt for millis() to work!

```
#define USE_BUTTON_0 // Enable code for Button at PCINT0
#include "EasyButtonAtInt01.h"

void printButtonToggleState(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
}
EasyButton Button0AtPin2(true, &printButtonToggleState);

void setup() {}
void loop() {}
```

## Handling multiple definition error
If you get the error "multiple definition of `__vector_1'" (or `__vector_2') because another library uses the attachInterrupt() function,
then comment out the #define USE_ATTACH_INTERRUPT at line 55 in EasyButtonAtInt01.h.

# Travis CI
The EasyButton library examples are built on Travis CI for the following boards:

- Arduino Uno
- Arduino Leonardo
- Adafruit Trinket (using ATTiny85)

#### If you find this library useful, please give it a star.
