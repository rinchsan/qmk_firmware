// Copyright 2022 Takuya Urakawa @hsgw (dm9records.com, 5z6p.com)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _DVORAK,
    _LOWER,
};

enum custom_keycodes {
    LOWER = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DVORAK] = LAYOUT(
         KC_TAB, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y, XXXXXXX,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L, KC_SLSH,
        KC_LCTL,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I, XXXXXXX,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S, KC_MINS,
        KC_LSFT, KC_SCLN,    KC_Q,    KC_J,    KC_K,    KC_X,          KC_BSLS,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,
                 XXXXXXX, XXXXXXX, KC_LOPT, KC_LCMD,  KC_SPC,  KC_ENT,   LOWER,  KC_ESC, XXXXXXX, XXXXXXX, XXXXXXX
    ),
    [_LOWER] = LAYOUT(
         KC_GRV, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, XXXXXXX, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  KC_EQL,
        KC_BRIU,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, XXXXXXX,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_LBRC,
        KC_BRID,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,          XXXXXXX,   KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_RBRC,
                 XXXXXXX, XXXXXXX, KC_MUTE, XXXXXXX, KC_SPC,  KC_VOLD, _______, KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX
    )
};

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
