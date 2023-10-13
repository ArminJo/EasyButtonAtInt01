<div align = center>

# [EasyButton](https://github.com/ArminJo/EasyButtonAtInt01)
Lightweight Arduino library for handling push buttons just connected between ground and INT0 and / or INT1 or any other PCINT pin.

[![Badge License: GPLv3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
 &nbsp; &nbsp; 
[![Badge Version](https://img.shields.io/github/v/release/ArminJo/EasyButtonAtInt01?include_prereleases&color=yellow&logo=DocuSign&logoColor=white)](https://github.com/ArminJo/EasyButtonAtInt01/releases/latest)
 &nbsp; &nbsp; 
[![Badge Commits since latest](https://img.shields.io/github/commits-since/ArminJo/EasyButtonAtInt01/latest?color=yellow)](https://github.com/ArminJo/EasyButtonAtInt01/commits/master)
 &nbsp; &nbsp; 
[![Badge Build Status](https://github.com/ArminJo/EasyButtonAtInt01/workflows/LibraryBuild/badge.svg)](https://github.com/ArminJo/EasyButtonAtInt01/actions)
 &nbsp; &nbsp; 
![Badge Hit Counter](https://visitor-badge.laobi.icu/badge?page_id=ArminJo_EasyButtonAtInt01)
<br/>
<br/>
[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/badges/StandWithUkraine.svg)](https://stand-with-ukraine.pp.ua)

Available as [Arduino library "EasyButtonAtInt01"](https://www.arduinolibraries.info/libraries/easy-button-at-int01).

[![Button Install](https://img.shields.io/badge/Install-brightgreen?logoColor=white&logo=GitBook)](https://www.ardu-badge.com/EasyButtonAtInt01)
 &nbsp; &nbsp; 
[![Button Changelog](https://img.shields.io/badge/Changelog-blue?logoColor=white&logo=AzureArtifacts)](https://github.com/ArminJo/EasyButtonAtInt01#revision-history)

</div>

#### If you find this library useful, please give it a star.

&#x1F30E; [Google Translate](https://translate.google.com/translate?sl=en&u=https://github.com/ArminJo/EasyButtonAtInt01)

<br/>

# Features
- No external pullup, **no polling needed**.
- The library is totally **based on interrupt** and **debouncing is implemented in a not blocking way**.
Debouncing is merely done by ignoring a button change within the debouncing time (default 50 ms).
So **button state is instantly available** without debouncing delay!
- Implements **toggle button** functionality.
- Support for **double press detection** is included. See [TwoButtons](examples/TwoButtons/TwoButtons.ino#L111) and [Callback example](examples/Callback/Callback.ino#L77).
- Support for **long press detection**, is included. See [Callback example](examples/Callback/Callback.ino#L97).
- Support for **active high buttons**.
- Small memory footprint.
- Support to **measure maximum bouncing period of a button**. See [DebounceTest example](examples/DebounceTest/DebounceTest.ino#L62).
- Button 1 can used at any pin supporting pin change interrupt.
- The exact pin numbers of the buttons are available by the macros INT0_PIN and INT1_PIN, which are set after the include.

## Table of available pins for the 2 buttons
| CPU | Button 0 | Button 1 using INT1 | Button 1 using PCINT, if INT1_PIN is defined != 3 |
|-|-|-|-|
| ATmega328* | D2 | D3 | Pin 0 to 2, 4 to 13, A0 to A5 |
| ATtiny5x | PB2 | | PB0 - PB5 |
| ATtiny167 | PB6 | PA3 | PA0 to PA2, PA4 to PA7 |

To use the PCINT buttons instead of the default one, just define INT1_PIN **before** including *EasyButtonAtInt01.hpp*.<br/>
E.g. `#define INT1_PIN 7`. See [TwoButtons.cpp](examples/TwoButtons/TwoButtons.ino#L52).

## Usage
To use a single button, it needs only:

```c++
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

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#define USE_BUTTON_1 // Enable code for button at INT1 (pin3) or PCINT[0:7]
#include "EasyButtonAtInt01.hpp"
EasyButton Button0AtPin2();  // no parameter -> Button is connected to INT0 (pin2)
EasyButton Button1AtPin3(BUTTON_AT_INT1_OR_PCINT); // Button is connected to INT1 (pin3)

void setup() {
    Serial.print(F("Button0 pin="));
    Serial.print(INT0_PIN);
    Serial.print(F(" Button1 pin="));
    Serial.println(INT1_PIN);
}
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
In this library, interrupts are enabled before the callback function is called. This allows the timer interrupt for millis() to work and therefore **delay() and millis() can be used in a callback function**.

```c++
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
The easiest way is to use the button release handler. Keep in mind, that you will get a press callback at the start of the long press.

```c++
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
```

Or check in loop, this enables to react with feedback as soon as long press duration is reached.

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin2)
#include "EasyButtonAtInt01.hpp"

bool sLongPressMessageSent = false;
void handleButtonPress(bool aButtonToggleState, uint16_t aButtonPressDurationMillis) {sLongPressMessageSent = false};
EasyButton Button0AtPin2(&handleButtonPress); // Button is connected to INT0 (pin2)

void setup() {}
void loop() {
if (!sLongPressMessageSent && Button0AtPin2.checkForLongPress(1000) == EASY_BUTTON_LONG_PRESS_DETECTED) {
        Serial.println(F("Long press of 1000 ms just detected"));
        sLongPressMessageSent = true; // Print message only once per long press
}

```


    if (sLCDDisplayPage == POWER_METER_PAGE_INFO && PageButtonAtPin3.checkForLongPress(1000) == EASY_BUTTON_LONG_PRESS_DETECTED) {


## Double press detection
**Call checkForDoublePress() only from button press callback function**. It will not work as expected, if called outside this callback function.

```c++
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

<br/>

# Handling the `multiple definition` error
If you get the error `multiple definition of __vector_1` (or `__vector_2`) because another library uses the attachInterrupt() function,
then insert the line `#define USE_ATTACH_INTERRUPT` in your program **before** the line `#include "EasyButtonAtInt01.hpp"`.

<br/>


# Compile options / macros for this library
To customize the library to different requirements, there are some compile options / macros available.<br/>
These macros must be defined in your program **before** the line `#include "EasyButtonAtInt01.hpp"` to take effect.<br/>
Modify them by enabling / disabling them, or change the values if applicable.

| Name | Default value | Description |
|-|-:|-|
| `USE_BUTTON_0` | disabled | Enables code for button at INT0 (pin2 on 328P, PB6 on ATtiny167, PB2 on ATtinyX5). The macro INT0_PIN is set after the include. |
| `USE_BUTTON_1` | disabled | Enables code for button at INT1 (pin3 on 328P, PA3 on ATtiny167, PCINT0 / PCx for ATtinyX5). The macro INT1_PIN is set after the include. |
| `INT1_PIN` | % | It overrides the usage of pin at the processors INT1 pin. Thus, it is the pin number of the pin for button 1 to use with Pin Change Interrupts. |
| `NO_INITIALIZE_IN_CONSTRUCTOR` | disabled | Disables the auto initializing in all constructors without the "aIsButtonAtINT0" parameter. Must be used if buttons must be initialized manually (e.g. later in setup). |
| `BUTTON_IS_ACTIVE_HIGH` | disabled | Enable this if your buttons are active high. |
| `USE_ATTACH_INTERRUPT` | disabled | This forces use of the arduino function attachInterrupt(). It is required if you get the error "multiple definition of \`__vector_1'" (or \`__vector_2'), because another library uses the attachInterrupt() function. |
| `NO_BUTTON_RELEASE_CALLBACK` | disabled | Disables the code for release callback. This saves 2 bytes RAM and 64 bytes program memory. |
| `BUTTON_DEBOUNCING_MILLIS` | 50 | With this you can adapt to the characteristic of your button. |
| `ANALYZE_MAX_BOUNCING_PERIOD` | disabled | Analyze the buttons actual debounce value. |
| `BUTTON_LED_FEEDBACK` | disabled | This activates LED_BUILTIN as long as button is pressed. |
| `BUTTON_LED_FEEDBACK_PIN` | disabled | The pin to use for button LED feedback. |
| `INTENTIONALLY_USE_PCI0_FOR_BUTTON1` | disabled | Activate it to suppress the warning: "Using PCINT0 interrupt for button 1". |

The exact pin numbers of the buttons used internally are available by the macros INT0_PIN and INT1_PIN, which are set after the include.

## Class methods
```c++
EasyButton(); // Constructor for button at INT0
EasyButton(void (*aButtonPressCallback)(bool aButtonToggleState)); // Constructor for button at INT0

EasyButton(bool aIsButtonAtINT0); // Constructor
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState));
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState), void (*aButtonReleaseCallback)(bool aButtonToggleState, uint16_t aButtonPressDurationMillis));
void init(); // used by constructors

#define EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS 400
#define EASY_BUTTON_DOUBLE_PRESS_DEFAULT_MILLIS 400

bool readButtonState();
bool readDebouncedButtonState();
bool updateButtonState();
uint16_t updateButtonPressDuration();
uint8_t checkForLongPress(uint16_t aLongPressThresholdMillis = EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS);
bool checkForLongPressBlocking(uint16_t aLongPressThresholdMillis = EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS);
bool checkForDoublePress(uint16_t aDoublePressDelayMillis = EASY_BUTTON_DOUBLE_PRESS_DEFAULT_MILLIS);
bool checkForForButtonNotPressedTime(uint16_t aTimeoutMillis);
```

# Revision History
###  Version 3.4.1 - work in progress
- Avoid wrong double press detection if calling checkForDoublePress() after release of button.

###  Version 3.4.0
- Added `NO_INITIALIZE_IN_CONSTRUCTOR` macro to enable late initializing.
- `ButtonStateIsActive` is now private, since it is not reliable after bouncing. Use `readButtonState()` or `readDebouncedButtonState()` instead.

###  Version 3.3.1
- Avoid mistakenly double press detection after boot.

###  Version 3.3.0
- Renamed EasyButtonAtInt01.cpp.h to EasyButtonAtInt01.hpp.

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
