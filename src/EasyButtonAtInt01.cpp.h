/*
 * EasyButtonAtInt01.cpp.h
 *
 * This file can be directly configured and included in one source.
 * Include EasyButtonAtInt01.h file if you need the declarations in a second source file.
 *
 *  Arduino library for handling push buttons connected between ground and INT0 and / or INT1 pin.
 *  INT0 and INT1 are connected to Pin 2 / 3 on most Arduinos (ATmega328), to PB6 / PA3 on ATtiny167 and on ATtinyX5 we have only INT0 at PB2.
 *  The library is totally based on interrupt.
 *  Debouncing is implemented in a not blocking way! It is merely done by ignoring a button change within the debouncing time.
 *  So button state is instantly available without debouncing delay!
 *
 *  Usage:
 *  #define USE_BUTTON_0
 *  #include "EasyButtonAtInt01.hpp"
 *  EasyButton Button0AtPin2(true);
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 */

#if defined(__AVR__)
#warning  ------For version >= 3.3.0 you must change: #include "EasyButtonAtInt01.cpp.h" to: #include "EasyButtonAtInt01.hpp"------
#include "EasyButtonAtInt01.hpp"
#endif // defined(__AVR__)
