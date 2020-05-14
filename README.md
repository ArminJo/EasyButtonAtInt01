# [EasyButton](https://github.com/ArminJo/EasyButtonAtInt01)
Available as Arduino library "EasyButtonAtInt01"

### [Version 2.1.0](https://github.com/ArminJo/EasyButtonAtInt01/releases)

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/EasyButtonAtInt01.svg?)](https://www.ardu-badge.com/EasyButtonAtInt01)
[![Commits since latest](https://img.shields.io/github/commits-since/ArminJo/EasyButtonAtInt01/latest)](https://github.com/ArminJo/EasyButtonAtInt01/commits/master)
[![Build Status](https://github.com/ArminJo/EasyButtonAtInt01/workflows/LibraryBuild/badge.svg)](https://github.com/ArminJo/EasyButtonAtInt01/actions)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FEasyButtonAtInt01)](https://github.com/brentvollebregt/hit-counter)

Arduino library for handling push buttons just connected between ground and INT0 and / or INT1 pin.<br/>
- No external pullup, **no polling needed**.
- The library is totally **based on interrupt** and **debouncing is implemented in a not blocking way**. 
Debouncing is merely done by ignoring a button change within the debouncing time (default 50 ms).
So **button state is instantly available** without debouncing delay!
- Each button press toggles a state variable, so **no external logic for implementing a toggle button is needed**.
- Support for **double press detection** is included. See EasyButtonExample and Callback example.
- Support for **long press detection**, is included. See EasyButtonExample example.
- Support to **measure maximum bouncing period of a button**. See EasyButtonExample example.

## Table of available pins for the 2 buttons
| CPU | Button 0 | Button 1 using INT1 | Button 1 using PCINT, if INT1_PIN is defined !=3 |
|-|-|-|-|
| ATmega328* | D2 | D3 | Pin 0 to 2, 4 to 7 |
| ATtiny5x | PB2 | | PB0 - PB5 |
| ATtiny167 | PB6 | PA3 | PA0 to PA2, PA4 to PA7 |

To use the PCINT buttons instead of the default one, just define INT1_PIN **before** including *EasyButtonAtInt01.cpp.h*.<br/>
E.g. `#define INT1_PIN 7`. See [EasyButtonExample.cpp](examples/EasyButtonExample/EasyButtonExample.ino#L46).

## Usage
To use a single button, it needs only:

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.cpp.h"
EasyButton Button0AtPin2;

void setup() {}
void loop() {
...
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState); // The value at the first call after first press is true
...
}
```
To use 2 buttons, it needs only:

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#define USE_BUTTON_1 // Enable code for button at INT1 (pin3)
#include "EasyButtonAtInt01.cpp.h"
EasyButton Button0AtPin2(true);  // true  -> Button is connected to INT0 (pin2)
EasyButton Button1AtPin3(false); // false -> Button is not connected to INT0 => connected to INT1 (pin3)

void setup() {}
void loop() {
...
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
    delay(100);
    digitalWrite(LED_BUILTIN, Button1AtPin3.ButtonToggleState);
    delay(200);
...
}
```

## Usage of callback function
The callback function is is called on every button press with ButtonToggleState as parameter.<br/>
**The value at the first call (after first press) is true**.<br/>
The callback function runs in an interrupt service context, which means it should be as short as possible. 
But before callback function is called, interrupts are enabled.
This allows the timer interrupt for millis() to work and therfore **delay() and millis() can be used in the callback function**.

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.cpp.h"

// initial value is false, so first call is with true
void printButtonToggleState(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
}
EasyButton Button0AtPin2(&printButtonToggleState);

void setup() {}
void loop() {}
```

## Long press detection
Check it cyclical in your loop. Do not forget, that you will get a callback (if enabled) at the start of the long press.
The blocking call only blocks if button is pressed, otherwise it returns immediately.

```
void loop() {
...
    // default long press period is 400 ms
    if (Button1AtPin3.checkForLongPressBlocking()) {
        doSomething();
    }
...
}
```

## Double press detection
Call checkForDoublePress() only from callback function. It will not work as expected called outside the callback function.

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.cpp.h"

void printButtonToggleState(bool aButtonToggleState);
EasyButton Button0AtPin2(&printButtonToggleState);

// initial value is false, so first call is with true
void printButtonToggleState(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
     // This function works reliable only if called in callback function
    if (Button0AtPin2.checkForDoublePress()) {
        Serial.println(F("Button 0 double press (< 400 ms) detected"));
    }
}

void setup() {}
void loop() {}
```

## Handling the `multiple definition` error
If you get the error `multiple definition of __vector_1` (or `__vector_2`) because another library uses the attachInterrupt() function,
then comment out the line `#define USE_ATTACH_INTERRUPT` in *EasyButtonAtInt01.h* or 
define global symbol with `-DUSE_ATTACH_INTERRUPT` which is not yet possible in Arduino IDE:-(.<br/>

## Consider to use [Sloeber](http://eclipse.baeyens.it/stable.php?OS=Windows) as IDE<br/>
If you are using Sloeber as your IDE, you can easily define global symbols at *Properties/Arduino/CompileOptions*.<br/>

## Class methods
```
EasyButton(bool aIsButtonAtINT0); // Constructor
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState)); // Constructor
void init(); // used by constructors

#define EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS 400
#define EASY_BUTTON_DOUBLE_PRESS_DEFAULT_MILLIS 400

bool readButtonState();
bool updateButtonState();
uint16_t updateButtonPressDuration();
uint8_t checkForLongPress(uint16_t aLongPressThresholdMillis = EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS);
bool checkForLongPressBlocking(uint16_t aLongPressThresholdMillis = EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS);
bool checkForDoublePress(uint16_t aDoublePressDelayMillis = EASY_BUTTON_DOUBLE_PRESS_DEFAULT_MILLIS);
bool checkForForButtonNotPressedTime(uint16_t aTimeoutMillis);
```

# Revision History
###  Version 2.1.0
- Avoid 1 ms delay for `checkForLongPressBlocking()` if button is not pressed.
- Only one true result per press for `checkForLongPressBlocking()`.

### Version 2.0.0
- Ported to ATtinyX5 and ATiny167.
- Support also PinChangeInterrupt for button 1 on Pin PA0 to PA7 for ATtiniy87/167.
- Long button press detection support.
- Analyzes maximum debouncing period.
- Double button press detection support.
- Very short button press handling.
- Renamed to EasyButtonAtInt01.cpp.h

### Version 1.0.0
- initial version for ATmega328.

# CI
The library examples are tested with GitHub Actions for the following boards:

- arduino:avr:uno
- arduino:avr:leonardo
- arduino:avr:mega          
- digistump:avr:digispark-tiny1
- digistump:avr:digispark-pro
- ATTinyCore:avr:attinyx5:chip=85,clock=1internal

## Requests for modifications / extensions
Please write me a PM including your motivation/problem if you need a modification or an extension e.g. PCINT* support for ATtiny85.

#### If you find this library useful, please give it a star.
