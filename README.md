# [EasyButton](https://github.com/ArminJo/EasyButtonAtInt01)
Available as Arduino library "EasyButtonAtInt01"

### [Version 3.2.0](https://github.com/ArminJo/EasyButtonAtInt01/releases)

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Installation instructions](https://www.ardu-badge.com/badge/EasyButtonAtInt01.svg?)](https://www.ardu-badge.com/EasyButtonAtInt01)
[![Commits since latest](https://img.shields.io/github/commits-since/ArminJo/EasyButtonAtInt01/latest)](https://github.com/ArminJo/EasyButtonAtInt01/commits/master)
[![Build Status](https://github.com/ArminJo/EasyButtonAtInt01/workflows/LibraryBuild/badge.svg)](https://github.com/ArminJo/EasyButtonAtInt01/actions)
[![Hit Counter](https://hitcounter.pythonanywhere.com/count/tag.svg?url=https%3A%2F%2Fgithub.com%2FArminJo%2FEasyButtonAtInt01)](https://github.com/brentvollebregt/hit-counter)

Lightweight Arduino library for handling push buttons just connected between ground and INT0 and / or INT1 pin.<br/>
- No external pullup, **no polling needed**.
- The library is totally **based on interrupt** and **debouncing is implemented in a not blocking way**.
Debouncing is merely done by ignoring a button change within the debouncing time (default 50 ms).
So **button state is instantly available** without debouncing delay!
- Implements **toggle button** functionality.
- Support for **double press detection** is included. See [EasyButtonExample](examples/EasyButtonExample/EasyButtonExample.ino#L112) and [Callback example](examples/Callback/Callback.ino#L78).
- Support for **long press detection**, is included. See [Callback example](examples/Callback/Callback.ino#L98).
- Support for **active high buttons**.
- Small memory footprint.
- Support to **measure maximum bouncing period of a button**. See [DebounceTest example](examples/DebounceTest/DebounceTest.ino#L64).

## Table of available pins for the 2 buttons
| CPU | Button 0 | Button 1 using INT1 | Button 1 using PCINT, if INT1_PIN is defined !=3 |
|-|-|-|-|
| ATmega328* | D2 | D3 | Pin 0 to 2, 4 to 13, A0 to A5 |
| ATtiny5x | PB2 | | PB0 - PB5 |
| ATtiny167 | PB6 | PA3 | PA0 to PA2, PA4 to PA7 |

To use the PCINT buttons instead of the default one, just define INT1_PIN **before** including *EasyButtonAtInt01.hpp*.<br/>
E.g. `#define INT1_PIN 7`. See [EasyButtonExample.cpp](examples/EasyButtonExample/EasyButtonExample.ino#L52).

## Usage
To use a single button, it needs only:

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.hpp"
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
#define USE_BUTTON_1 // Enable code for button at INT1 (pin3) or PCINT[0:7]
#include "EasyButtonAtInt01.hpp"
EasyButton Button0AtPin2();  // no parameter -> Button is connected to INT0 (pin2)
EasyButton Button1AtPin3(BUTTON_AT_INT1_OR_PCINT); // Button is connected to INT1 (pin3)

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

## Usage of callback functions
The button press callback function is is called on every button press with ButtonToggleState as parameter.<br/>
**The value at the first call (after first press) is true**.<br/>
The button release callback function is called on every button release with the additional parameter `ButtonPressDurationMillis`.<br/>
Both callback functions run in an interrupt service context, which means they should be as short/fast as possible.
In this library, interrupts are enabled before the callback function is called. This allows the timer interrupt for millis() to work and therfore **delay() and millis() can be used in a callback function**.

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.hpp"

// Initial value is false, so first call is with true
void handleButtonPress(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState);
}
EasyButton Button0AtPin2(&handleButtonPress); // Button is connected to INT0 (pin2)

void setup() {}
void loop() {}
```

## Long press detection
The easiest way is to check it in the button release handler. Do not forget, that you will get a press callback (if enabled) at the start of the long press.

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.hpp"

void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis);
EasyButton Button0AtPin2(NULL, &handleButtonRelease); // Button is connected to INT0 (pin2)

handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis) {
    if (aButtonPressDurationMillis >= EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS) { // 400 ms
        Serial.print(F("Long press "));
        Serial.print(aButtonPressDurationMillis);
        Serial.println(F(" ms detected"));
    }
}

void setup() {}
void loop() {}
}
```

## Double press detection
Call checkForDoublePress() only from button press callback function. It will not work as expected called outside this callback function.

```
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.hpp"

void handleButtonPress(bool aButtonToggleState);
EasyButton Button0AtPin2(&printButtonToggleState);

// Initial value is false, so first call is with true
void handleButtonPress(bool aButtonToggleState) {
     // This function works reliable only if called early in callback function
    if (Button0AtPin2.checkForDoublePress()) {
        Serial.println(F("Button 0 double press (< 400 ms) detected"));
    }
    digitalWrite(LED_BUILTIN, aButtonToggleState);
}

void setup() {}
void loop() {}
```

## Handling the `multiple definition` error
If you get the error `multiple definition of __vector_1` (or `__vector_2`) because another library uses the attachInterrupt() function,
then activate the line `#define USE_ATTACH_INTERRUPT` in *EasyButtonAtInt01.h* or
define global symbol with `-DUSE_ATTACH_INTERRUPT` which is not yet possible in Arduino IDE:-(.<br/>

## Consider using [Sloeber](http://eclipse.baeyens.it/stable.php?OS=Windows) as IDE<br/>
If you are using Sloeber as your IDE, you can easily define global symbols at *Properties > Arduino > CompileOptions*.<br/>

## Class methods
```
EasyButton(); // Constructor for button at INT0
EasyButton(void (*aButtonPressCallback)(bool aButtonToggleState)); // Constructor for button at INT0

EasyButton(bool aIsButtonAtINT0); // Constructor
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState));
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState), void (*aButtonReleaseCallback)(bool aButtonToggleState, uint16_t aButtonPressDurationMillis));
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
###  Version 3.2.0
- Allow button1 on pin 8 to 13 and A0 to A5 for ATmega328.

###  Version 3.1.0
- 2 sets of constructors, one for only one button used and one for the second button if two buttons used.
- Map pin numbers for Digispark pro boards, for use with with digispark library.

###  Version 3.0.0
- Added button release handler and adapted examples.
- Revoke change for "only one true result per press for checkForLongPressBlocking()". It is superseded by button release handler.
- Support buttons which are active high by defining `BUTTON_IS_ACTIVE_HIGH`.
- Improved detection of maximum bouncing period used in DebounceTest.

###  Version 2.1.0
- Avoid 1 ms delay for `checkForLongPressBlocking()` if button is not pressed.
- Only one true result per press for `checkForLongPressBlocking()`.

### Version 2.0.0
- Ported to ATtinyX5 and ATiny167.
- Support also PinChangeInterrupt for button 1 on Pin PA0 to PA7 for ATtiny87/167.
- Long button press detection support.
- Analyzes maximum debouncing period.
- Double button press detection support.
- Very short button press handling.
- Renamed to EasyButtonAtInt01.hpp

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
