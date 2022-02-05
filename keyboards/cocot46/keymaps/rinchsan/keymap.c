/* Copyright 2021 aki27
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
#include <stdio.h>

#include "paw3204.h"
#include "pointing_device.h"

bool isScrollMode;

// Defines names for use in layer keycodes and the keymap
enum layers {
    _DVORAK,
    _LOWER,
};

enum custom_keycodes {
  LOWER = SAFE_RANGE,
  MBTN1,
  MBTN2,
  MBTN3,
  SCRL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DVORAK] = LAYOUT(
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
       KC_TAB, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,      KC_Y,                                            KC_F,    KC_G,    KC_C,    KC_R,    KC_L,   KC_SLSH,
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
      KC_LCTL,    KC_A,    KC_O,    KC_E,    KC_U,      KC_I,                                            KC_D,    KC_H,    KC_T,    KC_N,    KC_S,   KC_MINS,
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
      KC_LSFT, KC_SCLN,    KC_Q,    KC_J,    KC_K,      KC_X, KC_MS_BTN1,               KC_MS_BTN2,      KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,   KC_BSLS,
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
                        XXXXXXX, KC_LOPT, KC_LCMD,    KC_SPC,    KC_PGUP,  KC_MS_BTN3,   KC_PGDOWN,    KC_ENT,   LOWER,  KC_ESC,  KC_BSPC
                                                    //`-----------------------'  `-----------------------'
    ),
    [_LOWER] = LAYOUT(
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
       KC_GRV, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR,   KC_PERC,                                         KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,    KC_EQL,
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
      KC_BRIU,    KC_1,    KC_2,    KC_3,    KC_4,      KC_5,                                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_LBRC,
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
      KC_BRID,   KC_F1,   KC_F2,   KC_F3,   KC_F4,     KC_F5, KC_MS_BTN4,               KC_MS_BTN5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,   KC_RBRC,
  //|-------------------------------------------------------|                                       |-------------------------------------------------------|
                        XXXXXXX, KC_MUTE, XXXXXXX,    KC_SPC,    KC_PGUP,  KC_MS_BTN3,   KC_PGDOWN,   KC_VOLD, _______, KC_VOLU, XXXXXXX
                                                    //`-----------------------'  `-----------------------'
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

void matrix_init_user(void) {
    init_paw3204();
}

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 3, .col = 6},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 2, .col = 6},
    .pressed = false
};

void matrix_scan_user(void) {
    static int  cnt;
    static bool paw_ready;

    report_mouse_t mouse_rep = pointing_device_get_report();

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

    if (cnt++ % 50 == 0) {
        uint8_t pid = read_pid_paw3204();
        if (pid == 0x30) {
            dprint("paw3204 OK\n");
            paw_ready = true;
        } else {
            dprintf("paw3204 NG:%d\n", pid);
            paw_ready = false;
        }
    }

    if (paw_ready) {
        uint8_t stat;
        int8_t x, y;
        int8_t r_x, r_y;

        read_paw3204(&stat, &x, &y);

        // 45-degree angle
        int8_t degree = 45;
        r_x =  + x * cos(degree) + y * sin(degree);
        r_y =  - x * sin(degree) + y * cos(degree);
        // int8_t degree = -45;
        // r_x =  - x * cos(degree) - y * sin(degree);
        // r_y =  + x * sin(degree) - y * cos(degree);
        /* normal angle
        r_x = y;
        r_y = x;
        */

        if (isScrollMode) {
            if (cnt % 5 == 0) {
                mouse_rep.v = -r_y;
                mouse_rep.h = r_x;
            }
        } else {
            mouse_rep.x = r_x;
            mouse_rep.y = r_y;
        }

        if (cnt % 10 == 0) {
            dprintf("stat:%3d x:%4d y:%4d\n", stat, mouse_rep.x, mouse_rep.y);
        }

        if (stat & 0x80) {
            pointing_device_set_report(mouse_rep);
        }
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _LOWER:
        isScrollMode = true;
        break;
    default:
        isScrollMode = false;
        break;
    }
    return state;
}
