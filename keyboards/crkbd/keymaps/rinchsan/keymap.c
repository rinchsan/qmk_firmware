/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

enum layers {
    _DVORAK,
    _LOWER,
};

enum custom_keycodes {
    DVORAK = SAFE_RANGE,
    LOWER,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DVORAK] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y,                         KC_F,    KC_G,    KC_C,    KC_R,   KC_L,  KC_SLSH,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                         KC_D,    KC_H,    KC_T,    KC_N,   KC_S,  KC_MINS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_SCLN,    KC_Q,    KC_J,    KC_K,    KC_X,                         KC_B,    KC_M,    KC_W,    KC_V,    KC_Z, KC_BSLS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LOPT, KC_LCMD,  KC_SPC,     KC_ENT,   LOWER,  KC_ESC
                                      //`--------------------------'  `--------------------------'
  ),
  [_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  KC_EQL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_LBRC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_RBRC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_VOLD, KC_VOLU, KC_MUTE,    KC_BRID, _______, KC_BRIU
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;
  }
  return rotation;
}

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Dvorak"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Sym/Num/F"), false);
            break;
    }
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM logo[] = {
        126,255,255,255,239,207,207,159,159,14,0,0,0,0,255,255,254,252,252,248,240,192,192,255,255,255,255,254,0,0,0,0,
        255,255,255,255,240,248,252,254,63,31,15,7,0,0,0,252,255,255,255,255,207,207,207,255,255,254,254,120,0,0,0,255,
        255,255,255,255,15,15,15,31,63,254,252,248,240,0,0,0,255,255,255,255,0,0,0,0,0,255,255,255,255,0,0,0,
        0,255,255,255,254,252,248,240,224,192,206,255,255,255,255,0,0,0,0,255,255,255,255,224,240,252,254,127,31,15,7,1,
        112,248,249,243,243,247,255,255,127,63,12,0,0,0,255,255,255,255,1,3,7,15,31,63,127,255,255,127,0,0,0,0,
        255,255,255,255,7,15,63,127,254,252,248,224,192,0,0,127,255,255,255,127,3,15,63,255,255,249,241,192,0,0,0,127,
        255,255,255,255,240,240,248,248,126,127,63,31,15,0,0,0,15,63,127,255,252,248,240,248,252,255,127,63,31,0,0,0,
        0,255,255,255,255,1,3,3,15,31,63,127,255,255,255,0,0,0,0,255,255,255,255,7,15,31,127,254,252,248,240,192,
    };
    oled_write_raw_P(logo, sizeof(logo));
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
    } else {
        oled_render_logo();
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
#endif // OLED_ENABLE
