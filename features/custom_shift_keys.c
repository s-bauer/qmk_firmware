// Copyright 2021-2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file custom_shift_keys.c
 * @brief Custom Shift Keys implementation
 *
 * For full documentation, see
 * <https://getreuer.info/posts/keyboards/custom-shift-keys>
 */

#include "custom_shift_keys.h"

#if !defined(IS_QK_MOD_TAP)
// Attempt to detect out-of-date QMK installation, which would fail with
// implicit-function-declaration errors in the code below.
#error "custom_shift_keys: QMK version is too old to build. Please update QMK."
#else

bool process_custom_shift_keys(uint16_t keycode, keyrecord_t *record) {
  static uint16_t registered_keycode = KC_NO;

  // If a custom shift key is registered, then this event is either releasing
  // it or manipulating another key at the same time. Either way, we release
  // the currently registered key.
  if (registered_keycode != KC_NO) {
    unregister_code16(registered_keycode);
    registered_keycode = KC_NO;
  }

  bool isCapslock = host_keyboard_led_state().caps_lock;

  const uint8_t saved_mods = get_mods();
#ifndef NO_ACTION_ONESHOT
  const uint8_t mods = saved_mods /*| get_weak_mods()*/ | get_oneshot_mods();
#else
  const uint8_t mods = saved_mods /*| get_weak_mods()*/;
#endif  // NO_ACTION_ONESHOT

  for (int i = 0; i < NUM_CUSTOM_SHIFT_KEYS; ++i) {
    if (keycode == custom_shift_keys[i].keycode) {
      // // Continue default handling if this is a tap-hold key being held.
      // if ((IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && record->tap.count == 0) {
      //   return true;
      // }

      bool isShiftPressed = (mods & MOD_MASK_SHIFT) != 0
#if CUSTOM_SHIFT_KEYS_NEGMODS != 0
          // Nothing in CUSTOM_SHIFT_KEYS_NEGMODS is held.
          && (mods & (CUSTOM_SHIFT_KEYS_NEGMODS)) == 0
#endif
;

      if(isCapslock) {
        if(isShiftPressed) {
          if (record->event.pressed) { 
            #ifdef CONSOLE_ENABLE
                uprintf("CAPS, SHIFT, Press\n");
            #endif 
            // capslock and shift should cancel each other and we should get the normal case
            if (IS_QK_MODS(custom_shift_keys[i].real_keycode) && (QK_MODS_GET_MODS(custom_shift_keys[i].real_keycode) & MOD_LSFT) != 0) {
              // if the key that should be printed out includs a shift modifier, 
              // we need to cancel the current shift and press the key without the shift modifier

              del_weak_mods(MOD_MASK_SHIFT);
  #ifndef NO_ACTION_ONESHOT
              del_oneshot_mods(MOD_MASK_SHIFT);
  #endif
              unregister_mods(MOD_MASK_SHIFT);

              register_code16(custom_shift_keys[i].real_keycode & (~QK_LSFT));
              set_mods(saved_mods);
            }
          } else {
            #ifdef CONSOLE_ENABLE
                uprintf("CAPS, SHIFT, Unpress\n");
            #endif 
            unregister_code16(custom_shift_keys[i].real_keycode & (~QK_LSFT));
          }
        } else {
          if (record->event.pressed) { 
            #ifdef CONSOLE_ENABLE
                uprintf("CAPS, NO SHIFT, Press\n");
            #endif 
            // capslock is on, but no shift is pressed. The shifted key should be registered
            if (IS_QK_MODS(custom_shift_keys[i].shifted_keycode) && (QK_MODS_GET_MODS(custom_shift_keys[i].shifted_keycode) & MOD_LSFT) != 0) {
              // The shifted key code contains the shift modifier. As caps lock is already active, we need to register the key without the shift modifier
              register_code16(custom_shift_keys[i].shifted_keycode & (~QK_LSFT));
            }
          } else {
            #ifdef CONSOLE_ENABLE
                uprintf("CAPS, NO SHIFT, Unpress\n");
            #endif 
            unregister_code16(custom_shift_keys[i].shifted_keycode & (~QK_LSFT));
          }
        }
      } else {
        // No CapsLock
        if(isShiftPressed) {
          if (record->event.pressed) {     
            #ifdef CONSOLE_ENABLE
                uprintf("NO CAPS, SHIFT, Press\n");
            #endif 

            // no caps lock but shift pressed. Register the shifted key without the modifier
            register_code16(custom_shift_keys[i].shifted_keycode & (~QK_LSFT));
          } else {
            #ifdef CONSOLE_ENABLE
                uprintf("NO CAPS, SHIFT, Unpress\n");
            #endif 

            unregister_code16(custom_shift_keys[i].shifted_keycode & (~QK_LSFT));
          }
        } else {
          // no shift 
          if (record->event.pressed) { 
            #ifdef CONSOLE_ENABLE
                uprintf("NO CAPS, NO SHIFT, Press\n");
            #endif 

            // no caps lock and no shift, register the normal key
            register_code16(custom_shift_keys[i].real_keycode);
          } else {
            #ifdef CONSOLE_ENABLE
                uprintf("NO CAPS, NO SHIFT, Unpress\n");
            #endif

            unregister_code16(custom_shift_keys[i].real_keycode);
          }
        }
      }

      return false;
    }
  }

  return true;  // Continue with default handling.
}

#endif  // version check