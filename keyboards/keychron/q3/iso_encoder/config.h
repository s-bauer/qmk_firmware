/* Copyright 2022 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/* key matrix pins */
#define MATRIX_ROW_PINS { B5, B4, B3, A15, A14, A13 }
#define MATRIX_COL_PINS { C14, C15, A0, A1, A2, A3, A4, A5, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN, NO_PIN }

#define SNLED27351_CURRENT_TUNE \
    { 0x9D, 0x9D, 0x44, 0x9D, 0x9D, 0x44, 0x9D, 0x9D, 0x44, 0x9D, 0x9D, 0x44 }

/* Enable caps-lock LED */
#define CAPS_LOCK_LED_INDEX 49

#define DYNAMIC_KEYMAP_LAYER_COUNT 8


#ifdef COMBO_COUNT
    #undef COMBO_COUNT
#endif

#ifdef TAPPING_TERM
    #undef TAPPING_TERM
#endif

#ifdef COMBO_HOLD
    #undef COMBO_HOLD
#endif

#ifdef COMBO_TERM
    #undef COMBO_TERM
#endif

#ifdef AUTO_SHIFT_TIMEOUT
    #undef AUTO_SHIFT_TIMEOUT
#endif

#define COMBO_ALLOW_ACTION_KEYS
#define COMBO_STRICT_TIMER
#define EXTRA_SHORT_COMBOS
#define TAPPING_TERM 150
#define COMBO_HOLD (TAPPING_TERM)
#define COMBO_TERM (TAPPING_TERM/4) // time to get all combo keys down

// AUTO SHIFT
#define AUTO_SHIFT_TIMEOUT (TAPPING_TERM)
#define NO_AUTO_SHIFT_SPECIAL
#define NO_AUTO_SHIFT_TAB
#define NO_AUTO_SHIFT_SYMBOLS
#define NO_AUTO_SHIFT_NUMERIC
