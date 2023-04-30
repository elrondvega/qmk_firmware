/*
QMK Firmware Massdrop CTRL M-AS Keymap
Copyright (C) 2020 matthewrobo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
//RGB_MATRIX_EFFECT(SOLID_REACTIVE_WIDE2)
//RGB_MATRIX_EFFECT(SOLID_REACTIVE_MULTIWIDE2)
//RGB_MATRIX_EFFECT(SOLID_REACTIVE_NEXUS2)
//RGB_MATRIX_EFFECT(SOLID_REACTIVE_MULTINEXUS2)
RGB_MATRIX_EFFECT(CONFIG_REACTIVE_MULTISPLASH)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

//static HSV SOLID_REACTIVE_WIDE_math2(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
//    uint16_t effect = tick - dist > 255 || dist > 32 ? 255 : tick - dist;
//    hsv.v           = qadd8(hsv.v, 255 - effect);
//    return hsv;
//}

//static HSV SOLID_REACTIVE_NEXUS_math2(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
//    uint16_t effect = tick - dist > 255 || ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) ? 255 : tick - dist;
//    hsv.v           = qadd8(hsv.v, 255 - effect);
//    hsv.h           = rgb_matrix_config.hsv.h + dy / 4;
//    return hsv;
//}

typedef HSV (*reactive_splash_custom_f)(uint8_t i, HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick);

bool effect_runner_reactive_splash_custom(uint8_t start, effect_params_t* params, reactive_splash_custom_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;
        hsv.v   = 32;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
            hsv           = effect_func(i, hsv, dx, dy, dist, tick);
        }
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#define _______ {130, 255, 64}
#define PURPLE {185, 255, 64}
#define PINK   {240, 255, 64}
#define TEAL   {130, 255, 64}

static HSV CONFIG_REACTIVE_SPLASH_CUSTOM_math(uint8_t i, HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {

const uint8_t PROGMEM ledmagicmap[][RGB_MATRIX_LED_COUNT][3] = {
    [0] = {
        PINK   ,          PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PURPLE , PINK   ,
        PURPLE , PURPLE , _______, _______, PURPLE , _______, _______, PURPLE , _______, _______, PURPLE , _______, _______, PURPLE , PURPLE , PURPLE , PURPLE ,
        PURPLE , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, PURPLE , PURPLE , PURPLE , PURPLE ,
        PURPLE , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          PINK   ,
        PURPLE ,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          PURPLE ,          PINK   ,
        PURPLE , PURPLE , PURPLE ,                            PINK   ,                            PURPLE , PURPLE , PURPLE , PURPLE , PINK   , PINK   , PINK
    },
};

	if (i > RGB_MATRIX_LED_COUNT) i = RGB_MATRIX_LED_COUNT;
        HSV bob = {
            .h = pgm_read_byte(&ledmagicmap[0][i][0]),
            .s = pgm_read_byte(&ledmagicmap[0][i][1]),
            .v = pgm_read_byte(&ledmagicmap[0][i][2]),
        };

	uint16_t effect = tick - dist;
        if (effect > 255) effect = 255;
        bob.v = qadd8(hsv.v, 255 - effect);

	return bob;
}

bool CONFIG_REACTIVE_MULTISPLASH(effect_params_t* params) { return effect_runner_reactive_splash_custom(0, params, &CONFIG_REACTIVE_SPLASH_CUSTOM_math); }

//bool SOLID_REACTIVE_WIDE2(effect_params_t* params) { return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SOLID_REACTIVE_WIDE_math2); }
//bool SOLID_REACTIVE_MULTIWIDE2(effect_params_t* params) { return effect_runner_reactive_splash(0, params, &SOLID_REACTIVE_WIDE_math2); }
//bool SOLID_REACTIVE_NEXUS2(effect_params_t* params) { return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SOLID_REACTIVE_NEXUS_math2); }
//bool SOLID_REACTIVE_MULTINEXUS2(effect_params_t* params) { return effect_runner_reactive_splash(0, params, &SOLID_REACTIVE_NEXUS_math2); }

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
