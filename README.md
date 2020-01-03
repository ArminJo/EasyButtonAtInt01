# EasyButton
Arduino library for handling push buttons connected between ground and INT0 and / or INT1 pin.<br/>
INT0 and INT1 are connected to Pin 2 / 3 on most Arduinos (ATmega328), to PB6 / PA3 on ATtiny167 and on ATtinyX5 we have only INT0 at PB2.<br/>

The library is totally **based on interrupt** and **debouncing is implemented in a not blocking way**. 
It is merely done by ignoring a button change within the debouncing time. So **button state is instantly available** without debouncing delay!

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/EasyButtonAtInt01.svg?)](https://www.ardu-badge.com/EasyButtonAtInt01)
[![Build Status](https://travis-ci.org/ArminJo/EasyButtonAtInt01.svg?branch=master)](https://travis-ci.org/ArminJo/EasyButtonAtInt01)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FEasyButtonAtInt01)](https://github.com/brentvollebregt/hit-counter)

## Usage
To use a single button, it needs only:

```
#define USE_BUTTON_0 // Enable code for Button at INT0
#include "EasyButtonAtInt01.h"
EasyButton Button0AtPin2(true); // true -> Button is connected to INT0

void setup() {}
void loop() {
...
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
...
}
```
To use 2 buttons, it needs only:
```
#define USE_BUTTON_0 // Enable code for Button at INT0
#define USE_BUTTON_1 // Enable code for Button at INT1
#include "EasyButtonAtInt01.h"
EasyButton Button0AtPin2(true);  // true  -> Button is connected to INT0
EasyButton Button0AtPin3(false); // false -> Button is not connected to INT0 => connected to INT1

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
#define USE_BUTTON_0 // Enable code for Button at INT0
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
then comment out the line 55 `#define USE_ATTACH_INTERRUPT` in *EasyButtonAtInt01.h* or 
define global symbol with `-DUSE_ATTACH_INTERRUPT` which is not yet possible in Arduino IDE:-(.<br/>

# Revision History
### Version 1.1.0
- Ported to ATtinyX5 and ATiny167.
- Support PinChangeInterrupt for ATtinies.
- Long press detection support.

### Version 1.1.0
- initial version for ATmega328.

# Travis CI
The EasyButton library examples are tested on Travis CI for the following boards:

- Arduino Uno
- Arduino Leonardo
- Digispark
- Digispark Pro
- Generic ATTiny85

#### If you find this library useful, please give it a star.
