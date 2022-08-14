/*
Copyright 2022 aki27

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "quantum.h"

enum layers {
    _BASE,
    _LOWER,
};

enum custom_keycodes {
    LOWER = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_TAB, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y,                                          KC_F,    KC_G,    KC_C,    KC_R,   KC_L,  KC_SLSH,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LCTL,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                                          KC_D,    KC_H,    KC_T,    KC_N,   KC_S,  KC_MINS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_LSFT, KC_SCLN,    KC_Q,    KC_J,    KC_K,    KC_X,                                          KC_B,    KC_M,    KC_W,    KC_V,    KC_Z, KC_BSLS,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        XXXXXXX, KC_LOPT, KC_LCMD,  KC_SPC,   KC_MS_BTN1,             KC_MS_BTN2,  KC_ENT,   LOWER,  KC_ESC,  XXXXXXX,
                                                               KC_PGDOWN, KC_MS_BTN3,    KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_LOWER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_GRV, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  KC_EQL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_BRIU,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_LBRC,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_BRID,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                         KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_RBRC,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        XXXXXXX, KC_MUTE, XXXXXXX,  KC_SPC,   KC_MS_BTN1,             KC_MS_BTN2, KC_VOLD, _______, KC_VOLU,  XXXXXXX,
                                                               KC_PGDOWN, KC_MS_BTN3,    KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 2},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 5},
    .pressed = false
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    }

    return true;
}

void matrix_scan_user(void) {
    if (IS_PRESSED(encoder1_ccw)) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (IS_PRESSED(encoder1_cw)) {
        encoder1_cw.pressed = false;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    }
}

static bool lower_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
            } else {
                layer_off(_LOWER);
                if (lower_pressed) {
                    register_code(KC_RCMD);
                    unregister_code(KC_RCMD);
                }
            }
            lower_pressed = record->event.pressed;
            return false;
        default:
            if (record->event.pressed) {
                lower_pressed = false;
            }
            return true;
    }
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    render_logo();
    oled_write_layer_state();
    return false;
}
#endif
