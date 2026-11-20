<div align = center>

# [EasyButton](https://github.com/ArminJo/EasyButtonAtInt01)
Lightweight Arduino library for push buttons connected between ground and INT0 and / or INT1 or any other PCINT pin.

[![Badge License: GPLv3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
 &nbsp; &nbsp; 
[![Badge Version](https://img.shields.io/github/v/release/ArminJo/EasyButtonAtInt01?color=yellow&logo=DocuSign&logoColor=white)](https://github.com/ArminJo/EasyButtonAtInt01/releases/latest)
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
[![Button Changelog](https://img.shields.io/badge/Changelog-blue?logoColor=white&logo=AzureArtifacts)](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#revision-history)

</div>

#### If you find this library useful, please give it a star.

&#x1F30E; [Google Translate](https://translate.google.com/translate?sl=en&u=https://github.com/ArminJo/EasyButtonAtInt01)

<br/>

# Contents
- [Features](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#features)
- [Supported pins](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#supported-pins)
- [Usage](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#usage)
- [Callback functions](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#callback-functions)
- [Long press detection](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#long-press-detection)
- [Double press detection](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#double-press-detection)
- [How debouncing works](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#how-debouncing-works)
- [Handling the multiple definition error](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#handling-the-multiple-definition-error)
- [Compile options / macros for this library](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#compile-options--macros-for-this-library)
- [Class members and methods](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#class-members-and-methods)
- [Examples](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#examples)
- [Migrating from version 3.x](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#migrating-from-version-3x)
- [Revision History](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#revision-history)

<br/>

# Features
- **No external pullup** and **no polling** needed.
- Completely **interrupt based**, with **non-blocking debouncing**.
  The button state is **available instantly**, without any debouncing delay.
- **Toggle button** functionality.
- Optional **callback functions** for button press and button release.
- **Long press detection** and **double press detection**.
- Support for **active high buttons**.
- **Measuring the maximum bouncing period** of your button.
- Button 1 can use any pin that supports pin change interrupts (PCINT).
- Small memory footprint.

<br/>

# Supported pins
| CPU / board | Button 0 | Button 1 at INT1 (default) | Button 1 at PCINT, if `INT1_PIN` is defined |
|-|-|-|-|
| ATmega328P (Uno, Nano, Pro Mini) | D2 (INT0) | D3 | D4 to D7 |
| ATtiny25 / 45 / 85 | PB2 (INT0) | - (there is no INT1) | PB0, PB1, PB3 (default), PB4, PB5 |
| ATtiny87 / 167 | PB6 (INT0) | PA3 | PA0 to PA7 |
| ATmega644P / 644PA / 1284P | PB2 (INT2) | not supported | not supported |

- To use a PCINT pin for button 1, define `INT1_PIN` **before** including *EasyButtonAtInt01.hpp*, e.g. `#define INT1_PIN 7`.
  See [TwoButtons example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/TwoButtons/TwoButtons.ino#L53).
- After the include, the macros `INT0_PIN` and `INT1_PIN` contain the pin numbers actually used.
- **ATtiny**: Pin numbers are the ones of [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore).
  The digispark core is no longer supported.
- **ATtiny25 / 45 / 85**: Button 1 always uses PCINT. PB5 is the RESET pin and can only be used if the RESET function is disabled by fuse.
- **ATtiny87 / 167**: `INT1_PIN` is the bit number of port A (0 to 7). `INT0_PIN` is 14, which is PB6 in the "New" pin mapping of ATTinyCore.
- **ATmega644P / 644PA / 1284P**: INT0 and INT1 are used by the second USART, so button 0 uses INT2 at PB2. Only button 0 is supported.
- **Other AVR CPUs**: The ATmega328P layout is assumed, i.e. INT0 at PD2 and INT1 at PD3.
  Boards with a different layout, like Leonardo or Mega, need a check of the pin numbers.

<br/>

# Usage
## One button
Connect the button between pin 2 and ground.

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin 2)
#include "EasyButtonAtInt01.hpp"

EasyButton Button0AtPin2; // No parameter -> button is connected to INT0 (pin 2)

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState); // ButtonToggleState is true after the first press
}
```

## Two buttons
Connect the buttons between pin 2 and ground and between pin 3 and ground.

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin 2)
#define USE_BUTTON_1 // Enable code for button at INT1 (pin 3) or at PCINT pin INT1_PIN
#include "EasyButtonAtInt01.hpp"

EasyButton Button0AtPin2(BUTTON_AT_INT0);          // Button is connected to INT0 (pin 2)
EasyButton Button1AtPin3(BUTTON_AT_INT1_OR_PCINT); // Button is connected to INT1 (pin 3)

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.print(F("Button0 pin="));
    Serial.print(INT0_PIN);
    Serial.print(F(" Button1 pin="));
    Serial.println(INT1_PIN);
}

void loop() {
    digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
    delay(100);
    digitalWrite(LED_BUILTIN, Button1AtPin3.ButtonToggleState);
    delay(200);
}
```

## Polling the state change flag
If you do not want to use a callback function, check the `ButtonStateHasJustChanged` flag in your loop.
The library only sets the flag, your program must reset it.
See [OneButton example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/OneButton/OneButton.ino#L59).

```c++
void loop() {
    if (Button0AtPin2.ButtonStateHasJustChanged) {
        Button0AtPin2.ButtonStateHasJustChanged = false; // Acknowledge the state change
        digitalWrite(LED_BUILTIN, Button0AtPin2.ButtonToggleState);
    }
}
```

<br/>

# Callback functions
- The **press callback** is called on every button press, with `ButtonToggleState` as parameter.
  **The value at the first call is true.**
- The **release callback** is called on every button release, with the additional parameter `aButtonPressDurationMillis`.
- Both callbacks run in **interrupt context**, so keep them short.
- Interrupts are enabled before a callback is called. Thus **`millis()` and `delay()` can be used in a callback**.
  A new button event during a long callback is handled, but it calls the callback again nested, which needs additional stack.

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin 2)
#include "EasyButtonAtInt01.hpp"

void handleButtonPress(bool aButtonToggleState);
void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis);
EasyButton Button0AtPin2(&handleButtonPress, &handleButtonRelease); // Button is connected to INT0 (pin 2)

void handleButtonPress(bool aButtonToggleState) {
    digitalWrite(LED_BUILTIN, aButtonToggleState); // aButtonToggleState is true at the first call
}

void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis) {
    (void) aButtonToggleState;
    Serial.print(F("Button released after "));
    Serial.print(aButtonPressDurationMillis);
    Serial.println(F(" ms"));
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {}
```

<br/>

# Long press detection
## In the release callback
This is the easiest way. Keep in mind, that you already got a press callback at the start of the long press.
See [Callback example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/Callback/Callback.ino#L111).

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin 2)
#include "EasyButtonAtInt01.hpp"

void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis);
EasyButton Button0AtPin2(nullptr, &handleButtonRelease); // No press callback, button is connected to INT0 (pin 2)

void handleButtonRelease(bool aButtonToggleState, uint16_t aButtonPressDurationMillis) {
    (void) aButtonToggleState;
    if (aButtonPressDurationMillis >= EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS) { // 400 ms
        Serial.print(F("Long press of "));
        Serial.print(aButtonPressDurationMillis);
        Serial.println(F(" ms detected"));
    }
}

void setup() {
    Serial.begin(115200);
}

void loop() {}
```

## In the loop
This allows you to give feedback as soon as the long press duration is reached, while the button is still pressed.
`checkForLongPress()` returns `EASY_BUTTON_LONG_PRESS_STILL_POSSIBLE`, `EASY_BUTTON_LONG_PRESS_ABORT` or `EASY_BUTTON_LONG_PRESS_DETECTED`.
See [TwoButtons example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/TwoButtons/TwoButtons.ino#L88).

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin 2)
#include "EasyButtonAtInt01.hpp"

bool sLongPressMessageSent = false;
void handleButtonPress(bool aButtonToggleState) {
    (void) aButtonToggleState;
    sLongPressMessageSent = false; // Enable a new message for this press
}
EasyButton Button0AtPin2(&handleButtonPress); // Button is connected to INT0 (pin 2)

void setup() {
    Serial.begin(115200);
}

void loop() {
    if (!sLongPressMessageSent && Button0AtPin2.checkForLongPress(1000) == EASY_BUTTON_LONG_PRESS_DETECTED) {
        Serial.println(F("Long press of 1000 ms just detected"));
        sLongPressMessageSent = true; // Print message only once per long press
    }
}
```

`checkForLongPressBlocking()` waits until the button is released or the long press duration is reached.

<br/>

# Double press detection
**Call `checkForDoublePress()` only in the button press callback function**, and as early as possible.
It will not work as expected if it is called anywhere else.
See [Callback example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/Callback/Callback.ino#L81)
and [TwoButtons example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/TwoButtons/TwoButtons.ino#L128).

```c++
#define USE_BUTTON_0 // Enable code for button at INT0 (pin 2)
#include "EasyButtonAtInt01.hpp"

void handleButtonPress(bool aButtonToggleState);
EasyButton Button0AtPin2(&handleButtonPress); // Button is connected to INT0 (pin 2)

void handleButtonPress(bool aButtonToggleState) {
    if (Button0AtPin2.checkForDoublePress()) { // Call it early in the callback function
        Serial.println(F("Double press (< 400 ms) detected"));
    }
    digitalWrite(LED_BUILTIN, aButtonToggleState);
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {}
```

<br/>

# How debouncing works
- The first level change after a quiet period is accepted **immediately**.
- Every further level change within `BUTTON_DEBOUNCING_MILLIS` (default **30 ms**) is ignored as bouncing,
  and **restarts** the debouncing time. So the button must be quiet for 30 ms before the next change is accepted.
- Because the debouncing time is retriggered, a small value is sufficient, even for buttons with long bouncing.
- A very short press, whose release falls into the debouncing time, is not completely detected.
  Call `updateButtonState()` in your loop to catch up with such a missed release.
- To determine the bouncing of your button, use the [DebounceTest example](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/DebounceTest/DebounceTest.ino#L31).
  It enables `ANALYZE_MAX_BOUNCING_PERIOD`, which prints the maximum time between 2 bouncing interrupts.

<br/>

# Handling the multiple definition error
If you get the error `multiple definition of __vector_1` (or `__vector_2`), another library uses the `attachInterrupt()` function.<br/>
Then insert the line `#define USE_ATTACH_INTERRUPT` in your program **before** the line `#include "EasyButtonAtInt01.hpp"`.

<br/>

# Compile options / macros for this library
To customize the library to different requirements, there are some compile options / macros available.<br/>
These macros must be defined in your program **before** the line `#include "EasyButtonAtInt01.hpp"` to take effect.<br/>
Modify them by enabling / disabling them, or change the values if applicable.

| Name | Default value | Description |
|-|-:|-|
| `USE_BUTTON_0` | disabled | Enables the code for button 0 at INT0 (pin 2 on ATmega328P, PB6 on ATtiny167, PB2 on ATtinyX5). |
| `USE_BUTTON_1` | disabled | Enables the code for button 1 at INT1 (pin 3 on ATmega328P, PA3 on ATtiny167) or at PCINT pin `INT1_PIN`. |
| `INT1_PIN` | 3 | Pin for button 1. Any value other than the INT1 pin selects a pin change interrupt. See [Supported pins](https://github.com/ArminJo/EasyButtonAtInt01?tab=readme-ov-file#supported-pins). |
| `NO_INITIALIZE_IN_CONSTRUCTOR` | disabled | Disables the initialization in all constructors without the `aIsButtonAtINT0` parameter. Use it to initialize buttons later, e.g. in `setup()`, by calling `init()`. |
| `BUTTON_IS_ACTIVE_HIGH` | disabled | Enable it if your buttons are active high. Then no internal pullup is activated. |
| `USE_ATTACH_INTERRUPT` | disabled | Forces the use of the Arduino function `attachInterrupt()`. Required if you get the error "multiple definition of `__vector_1`" (or `__vector_2`). |
| `NO_BUTTON_RELEASE_CALLBACK` | disabled | Disables the code for the release callback. This saves 2 bytes RAM and 64 bytes program memory. |
| `BUTTON_DEBOUNCING_MILLIS` | 30 | Retriggerable debouncing time. Adapt it to the characteristic of your button. |
| `ANALYZE_MAX_BOUNCING_PERIOD` | disabled | Determines the maximum time between 2 bouncing interrupts and stores it in `MaxBouncingPeriodMillis`. Every new maximum is printed. |
| `BUTTON_LED_FEEDBACK` | disabled | Activates the LED at `BUTTON_LED_FEEDBACK_PIN` as long as the button is pressed. |
| `BUTTON_LED_FEEDBACK_PIN` | `LED_BUILTIN` | The pin to use for button LED feedback. |
| `INTENTIONALLY_USE_PCI0_FOR_BUTTON1` | disabled | Suppresses the warning "You are using PCINT0 interrupt for button 1" on ATtiny87 / 167. |
| `USE_INT2_FOR_BUTTON_0` | automatically set for ATmega644P / 644PA / 1284P | Uses INT2 at PB2 for button 0, because INT0 and INT1 are used by the second USART. |
| `MEASURE_EASY_BUTTON_INTERRUPT_TIMING` | disabled | Sets `INTERRUPT_TIMING_OUTPUT_PIN` to high during the ISR, for measuring the ISR timing with an oscilloscope. |
| `INTERRUPT_TIMING_OUTPUT_PIN` | 6 | The pin to use for `MEASURE_EASY_BUTTON_INTERRUPT_TIMING`. |
| `TRACE` | disabled | Prints debug output. This changes the timing of the ISR! |

<br/>

# Class members and methods
**Note:** Many methods enable interrupts at their end, even if they were disabled before the call.

```c++
// Constructors for the 1. button, if both buttons are enabled. Otherwise for the only enabled button.
EasyButton();
EasyButton(void (*aButtonPressCallback)(bool aButtonToggleState));
EasyButton(void (*aButtonPressCallback)(bool aButtonToggleState),
        void (*aButtonReleaseCallback)(bool aButtonToggleState, uint16_t aButtonPressDurationMillis));

// Constructors with explicit button selection: BUTTON_AT_INT0 or BUTTON_AT_INT1_OR_PCINT
EasyButton(bool aIsButtonAtINT0);
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState));
EasyButton(bool aIsButtonAtINT0, void (*aButtonPressCallback)(bool aButtonToggleState),
        void (*aButtonReleaseCallback)(bool aButtonToggleState, uint16_t aButtonPressDurationMillis));

void init(bool aIsButtonAtINT0); // Called by the constructors, or by you if NO_INITIALIZE_IN_CONSTRUCTOR is defined

bool readButtonState();          // Current level of the button pin, not debounced: BUTTON_IS_ACTIVE or BUTTON_IS_INACTIVE
bool readDebouncedButtonState(); // Stored state during debouncing time, otherwise current level of the button pin
bool getStoredButtonState();     // State at the last accepted change. May be wrong after a missed release.
void updateButtonState();        // Catches up with a state change, which was ignored by the ISR. Call it in your loop.
uint16_t updateButtonPressDuration(); // Updates and returns ButtonPressDurationMillis while the button is pressed

uint8_t checkForLongPress(uint16_t aLongPressThresholdMillis = EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS);         // 400 ms
bool checkForLongPressBlocking(uint16_t aLongPressThresholdMillis = EASY_BUTTON_LONG_PRESS_DEFAULT_MILLIS);    // 400 ms
bool checkForDoublePress(uint16_t aDoublePressCheckDistanceMillis = EASY_BUTTON_DOUBLE_PRESS_DEFAULT_MILLIS);  // 400 ms
bool checkForButtonNotPressedTime(uint16_t aTimeoutMillis); // true if the last release was at least aTimeoutMillis ago

// Public members
volatile bool ButtonToggleState;               // Toggles at every press. Initial value is false.
volatile bool ButtonStateHasJustChanged;       // Set by the library at every press and release. Must be reset by your program.
volatile unsigned long ButtonLastChangeMillis; // millis() of the last accepted press or release
volatile uint16_t ButtonPressDurationMillis;   // Duration of the last press. Set at release or by updateButtonPressDuration().
volatile unsigned long ButtonReleaseMillis;    // millis() of the last release
volatile unsigned int MaxBouncingPeriodMillis; // Only if ANALYZE_MAX_BOUNCING_PERIOD is defined
```

<br/>

# Examples
| Example | Description |
|-|-|
| [OneButton](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/OneButton/OneButton.ino) | One button, polling of the `ButtonStateHasJustChanged` flag in the loop. |
| [Callback](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/Callback/Callback.ino) | Press and release callback, double press and long press detection. |
| [TwoButtons](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/TwoButtons/TwoButtons.ino) | Two buttons, long press detection in the loop, double press detection, button 1 at PCINT pin on ATtiny. |
| [DebounceTest](https://github.com/ArminJo/EasyButtonAtInt01/blob/master/examples/DebounceTest/DebounceTest.ino) | Measures the maximum bouncing period of your button. |

For the TwoButtons example on ATtiny25 / 45 / 85, the serial output of [ATtinySerialOut](https://github.com/ArminJo/ATtinySerialOut) must not use PB2,
because PB2 is the INT0 pin. Use e.g. `-DTX_PIN=PB0`.

<br/>

# Migrating from version 3.x
| 3.x | 4.0.0 |
|-|-|
| `checkForForButtonNotPressedTime()` | `checkForButtonNotPressedTime()` |
| `checkForLongPress()` returns `EASY_BUTTON_LONG_PRESS_DETECTED` after the release of a long press | Returns `EASY_BUTTON_LONG_PRESS_ABORT` after release. Use the release callback or `checkForLongPressBlocking()` instead. |
| `getButtonStateIsActive()` | `getStoredButtonState()` |
| `bool updateButtonState()` | `void updateButtonState()` |
| `BUTTON_DEBOUNCING_MILLIS` default 50 ms | Default 30 ms, the debouncing time is now retriggered |
| `INT1_PIN` 0 to 2, 8 to 13, A0 to A5 on ATmega328P | Compile error, use pins 4 to 7 for PCINT |
| Digispark core for ATtiny167 | Only ATTinyCore is supported |

<br/>

# Revision History
### Version 4.0.0
- Changed: Renamed `checkForForButtonNotPressedTime()` to `checkForButtonNotPressedTime()` and `getButtonStateIsActive()` to `getStoredButtonState()`.
- Changed: `checkForLongPress()` returns `EASY_BUTTON_LONG_PRESS_ABORT` after button release.
- Changed: `updateButtonState()` is now `void`.
- Changed: The debouncing time is now retriggerable, therefore `BUTTON_DEBOUNCING_MILLIS` is now 30 by default.
- Changed: Improved internal ISR.
- Changed: Use INT2 by default on ATmega644P / 644PA / 1284P.
- Changed: Button 1 on ATmega328P is restricted to pins 3 to 7.
- Removed: Support for the digispark core for ATtiny167. Only ATTinyCore is supported.

### Version 3.4.1
- Avoid wrong double press detection if calling `checkForDoublePress()` after release of button.
- Hack for ATmega 644.

### Version 3.4.0
- Added `NO_INITIALIZE_IN_CONSTRUCTOR` macro to enable late initializing.
- `ButtonStateIsActive` is now private, since it is not reliable after bouncing. Use `readButtonState()` or `readDebouncedButtonState()` instead.

### Version 3.3.1
- Avoid mistakenly double press detection after boot.

### Version 3.3.0
- Renamed EasyButtonAtInt01.cpp.h to EasyButtonAtInt01.hpp.

### Version 3.2.0
- Allow button 1 on pin 8 to 13 and A0 to A5 for ATmega328.

### Version 3.1.0
- 2 sets of constructors, one for only one button used and one for the second button if two buttons used.
- Map pin numbers for Digispark pro boards, for use with digispark library.

### Version 3.0.0
- Added button release handler and adapted examples.
- Revoke change for "only one true result per press for `checkForLongPressBlocking()`". It is superseded by button release handler.
- Support buttons which are active high by defining `BUTTON_IS_ACTIVE_HIGH`.
- Improved detection of maximum bouncing period used in DebounceTest.

### Version 2.1.0
- Avoid 1 ms delay for `checkForLongPressBlocking()` if button is not pressed.
- Only one true result per press for `checkForLongPressBlocking()`.

### Version 2.0.0
- Ported to ATtinyX5 and ATtiny167.
- Support also PinChangeInterrupt for button 1 on pin PA0 to PA7 for ATtiny87/167.
- Long button press detection support.
- Analyzes maximum debouncing period.
- Double button press detection support.
- Very short button press handling.
- Renamed to EasyButtonAtInt01.hpp

### Version 1.0.0
- Initial version for ATmega328.

<br/>

# CI
The library examples are compiled with GitHub Actions for the following boards:

- arduino:avr:uno
- arduino:avr:leonardo
- arduino:avr:mega
- ATTinyCore:avr:attinyx5:chip=85,clock=1internal
- ATTinyCore:avr:attinyx7micr:sketchclock=1external16
