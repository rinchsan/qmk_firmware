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

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0x1727
#define PRODUCT_ID   0x0004
#define DEVICE_VER   0x0001
#define MANUFACTURER aki27
#define PRODUCT      trackball_module

/* key matrix size */
#define MATRIX_ROWS 1
#define MATRIX_COLS 5

// wiring of each half
#define DIRECT_PINS { { B6, B3, B2, NO_PIN, NO_PIN } }
#define UNUSED_PINS { F4, F5, D4, C6, D7, E6, B4, B5}

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#undef LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#undef LOCKING_RESYNC_ENABLE

/* Limit the maximum layer number to 8 */
#define LAYER_STATE_8BIT

/* Rotary encoder */
#define ENCODERS_PAD_A { D2 }
#define ENCODERS_PAD_B { D3 }
#define ENCODER_RESOLUTION 4

#define TAPPING_FORCE_HOLD
#define TAPPING_TERM 200

#define ADNS5050_SCLK_PIN         F7
#define ADNS5050_SDIO_PIN         F6
#define ADNS5050_CS_PIN           B1

#define POINTING_DEVICE_ROTATION_180
#define OLED_FONT_H "keyboards/trackball_module/lib/glcdfont.c"
