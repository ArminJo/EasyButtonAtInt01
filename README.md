# EasyButton
### Version 2.0.0

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/EasyButtonAtInt01.svg?)](https://www.ardu-badge.com/EasyButtonAtInt01)
[![Build Status](https://travis-ci.org/ArminJo/EasyButtonAtInt01.svg?branch=master)](https://travis-ci.org/ArminJo/EasyButtonAtInt01)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FEasyButtonAtInt01)](https://github.com/brentvollebregt/hit-counter)

Arduino library for handling push buttons just connected between ground and INT0 and / or INT1 pin.<br/>

No external pullup, **no polling needed**.

The library is totally **based on interrupt** and **debouncing is implemented in a not blocking way**. 
Debouncing is merely done by ignoring a button change within the debouncing time. So **button state is instantly available** without debouncing delay!

- Each button press toggles a state variable, so **no external logic for implementing a toggle button is needed**.
- Support for **double press detection** is included. See Callback example.
- Support for **long press detection**, which needs some polling or blocking, is included. See EasyButtonExample example.

INT0 and INT1 are connected to:
- Pin 2 / 3 on most Arduinos (ATmega328*).
- PB6 / PA3 on ATtiny167. To use one of PA0 to PA7 instead of PA3 just (re)define INT1_PIN before including *EasyButtonAtInt01.h*. E.g. `#define INT1_PIN 7`. See EasyButtonExample.cpp.
- Only PB2 / INT0 at on ATtinyX5.



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
then comment out the line `#define USE_ATTACH_INTERRUPT` in *EasyButtonAtInt01.h* or 
define global symbol with `-DUSE_ATTACH_INTERRUPT` which is not yet possible in Arduino IDE:-(.<br/>
## Consider to use [Sloeber](http://eclipse.baeyens.it/stable.php?OS=Windows) as IDE<br/>
If you are using Sloeber as your IDE, you can easily define global symbols at *Properties/Arduino/CompileOptions*.<br/>

## Class methods
```
EasyButton(bool aIsButtonAtINT0); // Constructor
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState)); // Constructor
void init(); // used by constructors

bool readButtonState();
bool updateButtonState();
uint16_t updateButtonPressDuration();
uint8_t checkForLongPress(uint16_t aLongPressThresholdMillis);
bool checkForLongPressBlocking(uint16_t aLongPressThresholdMillis);
bool checkForDoublePress(uint16_t aDoublePressDelayMillis);
bool checkForForButtonNotPressedTime(uint16_t aTimeoutMillis);
```

# Revision History

### Version 2.0.0
- Ported to ATtinyX5 and ATiny167.
- Support also PinChangeInterrupt for button 1 on Pin PA0 to PA7 for ATtiniy87/167.
- Long button press detection support.
- Analyzes maximum debouncing period.
- Double button press detection support.
- Very short button press handling.

### Version 1.0.0
- initial version for ATmega328.

# Travis CI
The EasyButton library examples are tested on Travis CI for the following boards:

- Arduino Uno
- Arduino Leonardo
- Digispark
- Digispark Pro
- Generic ATTiny85

## Requests for modifications / extensions
Please write me a PM including your motivation/problem if you need a modification or an extension e.g. PCINT* support for ATtiny85.

#### If you find this library useful, please give it a star.
