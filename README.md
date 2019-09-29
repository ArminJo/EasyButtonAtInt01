# EasyButton
Arduino library for handling push buttons connected between ground and INT0 and / or INT1 pin (pin 2 / 3).<br/>
The library is totally based on interrupt and debouncing is implemented in a not blocking way. It is merely done by ignoring a button change within the debouncing time.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/EasyButtonAtInt01.svg?)](https://www.ardu-badge.com/EasyButtonAtInt01)
[![Build Status](https://travis-ci.org/ArminJo/EasyButtonAtInt01.svg?branch=master)](https://travis-ci.org/ArminJo/EasyButtonAtInt01)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FEasyButtonAtInt01)](https://github.com/brentvollebregt/hit-counter)

## Usage
To use a single button, it needs only:

```
#define USE_BUTTON_0
#include "EasyButtonAtInt01.h"
EasyButton Button0AtPin2(true);

void setup() {}
void loop() {
...
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
...
}
```

## Callback usage
Even the use of a callback function is simple.

```
#define USE_BUTTON_0
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
then comment out the #define USE_ATTACH_INTERRUPT al line 61 in EasyButtonAtInt01.h.

# Travis CI
The EasyButton library examples are built on Travis CI for the following boards:

- Arduino Uno
- Arduino Leonardo
- Adafruit Trinket (using ATTiny85)

#### If you find this library useful, please give it a star.
