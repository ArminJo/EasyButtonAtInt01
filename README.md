# EasyButton
Arduino library for handling push buttons at INT0 and / or INT1 pin (pin 2 / 3).<br/>
The library is totally based on interrupt.<br/>
Debouncing is implemented in a not blocking way! It is merely done by ignoring a button change within the debouncing time.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/ArminJo/EasyButtonAtInt01.svg?branch=master)](https://travis-ci.org/ArminJo/EasyButtonAtInt01)


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
