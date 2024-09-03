
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

#include QMK_KEYBOARD_H
#include "print.h"

uint8_t  saved_mods = 0;           // to pass state between process_record_user and matrix_scan_user
uint8_t  combo_on = 0;             // for combo actions to hold before triggering
bool     combo_triggered = false;  // for one-shot-combo-actions
uint32_t linger_timer = 0;         // time to hold a key before something else happens.

enum layers{
    HD_BASE,
    HD_SHIFT,
    HD_NAV,
    HD_FN,
    WIN_BASE,
    WIN_FN
};

enum my_keycode {
  CKEY_PIPE = SAFE_RANGE, // Pipe character |
  CKEY_PLUS,    // Plus character +
  CKEY_CURL_O,  // Opening Curly Braces {
  CKEY_CURL_C,  // Closing Curly Braces }
  CKEY_SQ_O,    // Opening Square Brackets [
  CKEY_SQ_C,    // Closing Square Brackets ]
  CKEY_TILDE,   // Tilde ~
  CKEY_AT,      // At @
  CKEY_HASH,    // Hash #,
  CKEY_CARET,   // Caret ^
  CKEY_LT,      // Less Than <
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [HD_BASE] = LAYOUT_tkl_f13_iso(
        KC_ESC,                 KC_F1,              KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,               KC_F7,     KC_F8,    KC_F9,        KC_F10,         KC_F11,     KC_F12,         KC_MUTE,                   KC_PSCR,  DF(WIN_BASE),  QK_AUTO_SHIFT_TOGGLE,
        LSFT(KC_EQL),           KC_1,               KC_2,     KC_3,     KC_4,     KC_5,     KC_6,                KC_7,      KC_8,     KC_9,         KC_0,           LSFT(KC_0), RALT(KC_MINUS), KC_BSPC,                   KC_INS,   KC_HOME,       KC_PGUP,
        KC_TAB,                 KC_W,               KC_F,     KC_M,     KC_P,     KC_V,     LSFT(KC_7),          KC_DOT,    KC_Q,     LSFT(KC_2),   LSFT(KC_NUHS),  KC_Y,       LSFT(KC_8),                                KC_DEL,   KC_END,        KC_PGDN,
        TG(HD_NAV),             KC_R,               KC_S,     KC_N,     KC_T,     KC_G,     KC_COMMA,            KC_A,      KC_E,     KC_I,         KC_H,           KC_J,       LSFT(KC_9),     KC_ENT,
        LM(HD_SHIFT, MOD_LSFT), RALT(KC_MINUS),     KC_X,     KC_C,     KC_L,     KC_D,     KC_B,                KC_SLASH,  KC_U,     KC_O,         KC_Z,           KC_K,                       LM(HD_SHIFT, MOD_RSFT),              KC_UP,
        KC_LCTL,                KC_LWIN,            KC_LALT,                                KC_SPC,                                                 KC_RALT,        KC_RWIN,    MO(HD_FN),      KC_RCTL,                   KC_LEFT,  KC_DOWN,       KC_RGHT),

    [HD_SHIFT] = LAYOUT_tkl_f13_iso(
        KC_ESC,                 KC_F1,              KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,               KC_F7,     KC_F8,    KC_F9,        KC_F10,         KC_F11,     KC_F12,         KC_MUTE,                   KC_PSCR,  KC_NO,         RGB_MOD,
        CKEY_TILDE,             KC_1,               CKEY_AT,  CKEY_HASH,KC_4,     KC_5,     CKEY_CARET,          KC_6,      KC_NO,    CKEY_LT,      KC_NUBS,        KC_SLASH,   CKEY_PIPE,      KC_BSPC,                   KC_INS,   KC_HOME,       KC_PGUP,
        KC_TAB,                 KC_W,               KC_F,     KC_M,     KC_P,     KC_V,     KC_RBRC,             KC_DOT,    KC_Q,     CKEY_SQ_O,    CKEY_SQ_C,      KC_Y,       CKEY_CURL_O,                               KC_DEL,   KC_END,        KC_PGDN,
        TG(HD_NAV),             KC_R,               KC_S,     KC_N,     KC_T,     KC_G,     KC_COMMA,            KC_A,      KC_E,     KC_I,         KC_H,           KC_J,       CKEY_CURL_C,    KC_ENT,
        LM(HD_SHIFT, MOD_LSFT), CKEY_PIPE,          KC_X,     KC_C,     KC_L,     KC_D,     KC_B,                CKEY_PLUS, KC_U,     KC_O,         KC_Z,           KC_K,                       LM(HD_SHIFT, MOD_RSFT),              KC_UP,
        KC_LCTL,                KC_LWIN,            KC_LALT,                                KC_SPC,                                                 KC_RALT,        KC_RWIN,    MO(HD_FN),      KC_RCTL,                   KC_LEFT,  KC_DOWN,       KC_RGHT),

    [HD_NAV] = LAYOUT_tkl_f13_iso(
        KC_ESC,                 KC_F1,              KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,               KC_F7,     KC_F8,    KC_F9,        KC_F10,         KC_F11,     KC_F12,         KC_MUTE,                   KC_PSCR,  KC_NO,         RGB_MOD,
        LSFT(KC_EQL),           KC_NO,              KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,               KC_NO,     KC_NO,    KC_NO,        KC_NO,          KC_NO,      KC_NO,          KC_BSPC,                   KC_INS,   KC_HOME,       KC_PGUP,
        KC_TAB,                 KC_NO,              KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,               KC_BSPC,   KC_DEL,   KC_HOME,      KC_END,         KC_NO,      KC_NO,                                     KC_DEL,   KC_END,        KC_PGDN,
        TG(HD_NAV),             KC_LCTL,            KC_LSFT,  KC_LALT,  KC_NO,    KC_G,     KC_NO,               KC_LEFT,   KC_DOWN,  KC_UP,        KC_RIGHT,       KC_NO,      KC_NO,          KC_ENT,
        KC_LSFT,                KC_NO,              KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,               KC_NO,     KC_NO,    KC_NO,        KC_NO,          KC_NO,                      KC_RSFT,                             KC_UP,
        KC_LCTL,                KC_LWIN,            KC_LALT,                                KC_SPC,                                                 KC_RALT,        KC_RWIN,    MO(HD_FN),      KC_RCTL,                   KC_LEFT,  KC_DOWN,       KC_RGHT),

    [HD_FN] = LAYOUT_tkl_f13_iso(
        _______,                KC_BRID,            KC_BRIU,  _______,  _______,  RGB_VAD,  RGB_VAI,             KC_MPRV,   KC_MPLY,  KC_MNXT,      KC_MUTE,        KC_VOLD,    KC_VOLU,        RGB_TOG,                   _______,  _______,       RGB_TOG,
        _______,                _______,            _______,  _______,  _______,  _______,  _______,             _______,   _______,  _______,      _______,        _______,    _______,        _______,                   _______,  _______,       _______,
        RGB_TOG,                RGB_MOD,            RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,             _______,   _______,  _______,      _______,        _______,    _______,                                   _______,  _______,       _______,
        _______,                RGB_RMOD,           RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,             _______,   _______,  _______,      _______,        _______,    _______,        _______,
        _______,                _______,            _______,  _______,  _______,  _______,  _______,             NK_TOGG,   _______,  _______,      _______,        _______,                    _______,                             _______,
        _______,                _______,            _______,                                _______,                                                _______,        _______,    _______,        _______,                   _______,  _______,       _______),

    [WIN_BASE] = LAYOUT_tkl_f13_iso(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,     KC_MUTE,  KC_PSCR,  DF(HD_BASE),    QK_AUTO_SHIFT_TOGGLE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,  KC_INS,   KC_HOME,        KC_PGUP,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,              KC_DEL,   KC_END,         KC_PGDN,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,    KC_ENT,
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,        KC_RGHT),

    [WIN_FN] = LAYOUT_tkl_f13_iso(
        _______,  KC_BRID,  KC_BRIU,  _______,  _______,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,    RGB_TOG,  _______,  _______,        RGB_TOG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,        _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,        _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,    _______,  _______,  _______,        _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [HD_BASE]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [HD_SHIFT] = {ENCODER_CCW_CW(KC_VOLD, RGB_VAI) },
    [HD_NAV]   = {ENCODER_CCW_CW(KC_VOLD, RGB_VAI) },
    [HD_FN]    = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI) }
};
#endif


enum combos {
    MC_ESC,
    MC_COPY,
    MC_CUT,
    MC_PASTE,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM combo_cut[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_copy[] = {KC_X, KC_L, COMBO_END};
const uint16_t PROGMEM combo_paste[] = {KC_X, KC_D, COMBO_END};

combo_t key_combos[] = {
  [MC_COPY] = COMBO_ACTION(combo_copy),
  [MC_CUT] = COMBO_ACTION(combo_cut),
  [MC_PASTE] = COMBO_ACTION(combo_paste),
};


void matrix_scan_user_process_combo(void) {  // called from matrix_scan_user if combo_on
    // For held combos only!

    if (!combo_triggered) {
        if ((timer_elapsed(linger_timer) > COMBO_HOLD) && combo_on) {
            saved_mods = get_mods();
            clear_mods();
            switch(combo_on) {

                // case MC_COPYCUT:
                //     tap_code16(LCTL(KC_X));
                //     break;
            }

            set_mods(saved_mods);
            combo_triggered = true;
        }
    }
}

void process_combo_event(uint16_t combo_index, bool pressed) {
    // ONLY non-held combos go here. 
    // For held combos, see matrix_scan_user

    uprintf("Combo: start\n");


    if (pressed) {
        // key down

        switch(combo_index) {
            case MC_COPY:
            case MC_CUT:
            case MC_PASTE:
                uprintf("Combo: pressed\n");
                combo_on = combo_index;
                break;
        }


        if(combo_on) 
            linger_timer = timer_read();
    } else {
        // key up

        if (combo_on && !combo_triggered) { // combo NOT triggered in matrix_scan_user_process_combo
            // not held (for held, see matrix_scan_user_process_combo)

            switch(combo_index) {
                case MC_COPY:
                    uprintf("Combo: MC_COPY released\n");
                    tap_code16(LCTL(KC_C));
                    break;
                
                case MC_CUT:
                    uprintf("Combo: MC_CUT released\n");
                    tap_code16(LCTL(KC_X));
                    break;

                case MC_PASTE:
                    uprintf("Combo: MC_PASTE released\n");
                    tap_code16(LCTL(KC_V));
                    break;
            }
        }

        combo_on = false;
        combo_triggered = false;
    }
}


void matrix_scan_user(void) {
    // Is a combo_action being held for delayed action/linger combos)?
    if (combo_on) {
        matrix_scan_user_process_combo();
    }
}

bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 4 : 0));
        return false;
    }

    return true;
}

uint8_t weak_mod_state;
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif 

  // Your macros ...
  mod_state = get_mods();
  weak_mod_state = get_weak_mods();
  switch (keycode) {

    case CKEY_PIPE:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_NUBS));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_NUBS));
        }
        return false;

    case CKEY_PLUS:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(KC_RBRC);
            set_mods(mod_state);
        } else {
            unregister_code16(KC_RBRC);
        }
        return false;

    case CKEY_CURL_C:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_0));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_0));
        }
        return false;

    case CKEY_CURL_O:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_7));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_7));
        }
        return false;

    case CKEY_SQ_O:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_8));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_8));
        }
        return false;

    case CKEY_SQ_C:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_9));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_9));
        }
        return false;

    case CKEY_TILDE:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_RBRC));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_RBRC));
        }
        return false;

    case CKEY_AT:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(RALT(KC_Q));
            set_mods(mod_state);
        } else {
            unregister_code16(RALT(KC_Q));
        }
        return false;

    case CKEY_HASH:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(KC_NUHS);
            set_mods(mod_state);
        } else {
            unregister_code16(KC_NUHS);
        }
        return false;

    case CKEY_CARET:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(KC_GRAVE);
            set_mods(mod_state);
        } else {
            unregister_code16(KC_GRAVE);
        }
        return false;

    case CKEY_LT:
        if(record->event.pressed) {
            del_mods(MOD_MASK_SHIFT);
            register_code16(KC_NUBS);
            set_mods(mod_state);
        } else {
            unregister_code16(KC_NUBS);
        }
        return false;
  }

  return true;
}

