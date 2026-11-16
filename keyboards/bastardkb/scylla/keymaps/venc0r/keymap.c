// Copyright 2021 Jörg Markert (@venc0r)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "venc0r.h"

#define LAYOUT_wrapper(...) LAYOUT_split_4x6_5(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_ALPHA] = LAYOUT_wrapper(
        _______________Y_COLEMAK_L0________________, _______________Y_COLEMAK_R0________________,
        _______________Y_COLEMAK_L1________________, _______________Y_COLEMAK_R1________________,
        _______________Y_COLEMAK_L2________________, _______________Y_COLEMAK_R2________________,
        _______________Y_COLEMAK_L3________________, _______________Y_COLEMAK_R3________________,
                                    __Y_THUMB_L0___, __Y_THUMB_R0___,
                                    __Y_THUMB_L1___, __Y_THUMB_R1___
    ),

    [_LOWER] = LAYOUT_wrapper(
        _______________Y_SYMBOL_L0_________________, _______________Y_SYMBOL_R0_________________,
        _______________Y_SYMBOL_L1_________________, _______________Y_SYMBOL_R1_________________,
        _______________Y_SYMBOL_L2_________________, _______________Y_SYMBOL_R2_________________,
        _______________Y_SYMBOL_L3_________________, _______________Y_SYMBOL_R3_________________,
                                    __Y_THUMB_N0___, __Y_THUMB_N0___,
                                    __Y_THUMB_N1___, __Y_THUMB_N1___
    ),

    [_RAISE] = LAYOUT_wrapper(
        _______________Y_NAV_L0____________________, _______________Y_NAV_R0____________________,
        _______________Y_NAV_L1____________________, _______________Y_NAV_R1____________________,
        _______________Y_NAV_L2____________________, _______________Y_NAV_R2____________________,
        _______________Y_NAV_L3____________________, _______________Y_NAV_R3____________________,
                                    __Y_THUMB_N0___, __Y_THUMB_N0___,
                                    __Y_THUMB_N1___, __Y_THUMB_N1___
    ),

    [_ADJUST] = LAYOUT_wrapper(
        _______________Y_ADJUST_L0_________________, _______________Y_ADJUST_R0_________________,
        _______________Y_ADJUST_L1_________________, _______________Y_ADJUST_R1_________________,
        _______________Y_ADJUST_L2_________________, _______________Y_ADJUST_R2_________________,
        _______________Y_ADJUST_L3_________________, _______________Y_ADJUST_R3_________________,
                                    __Y_THUMB_N0___, __Y_THUMB_N0___,
                                    __Y_THUMB_N1___, __Y_THUMB_N1___
    ),

    [_GAME] = LAYOUT_wrapper(
        _______________Y_GAME_L0___________________, _______________Y_GAME_R0___________________,
        _______________Y_GAME_L1___________________, _______________Y_GAME_R1___________________,
        _______________Y_GAME_L2___________________, _______________Y_GAME_R2___________________,
        _______________Y_GAME_L3___________________, _______________Y_GAME_R3___________________,
                                    __Y_G_THUMB_L0_, __Y_G_THUMB_R0_,
                                    __Y_G_THUMB_L1_, __Y_G_THUMB_R1_
    ),
};


enum unicode_names { AE_LOWER, AE_UPPER, OE_LOWER, OE_UPPER, UE_LOWER, UE_UPPER, SZ };

const uint32_t PROGMEM unicode_map[] = {
    [AE_LOWER] = 0x00E4, [AE_UPPER] = 0x00C4,
    [OE_LOWER] = 0x00F6, [OE_UPPER] = 0x00D6,
    [UE_LOWER] = 0x00FC, [UE_UPPER] = 0x00DC,
    [SZ]       = 0x00DF,
};

enum combo_names { BOOT_COMBO, AE_COMBO, OE_COMBO, UE_COMBO, SZ_COMBO };

const uint16_t PROGMEM boot_combo[] = {KC_Z, KC_Q, COMBO_END};
const uint16_t PROGMEM ae_combo[]   = {KC_Q, C_HOME_A, COMBO_END};
const uint16_t PROGMEM oe_combo[]   = {KC_SCLN, C_HOME_O, COMBO_END};
const uint16_t PROGMEM ue_combo[]   = {KC_U, C_HOME_E, COMBO_END};
const uint16_t PROGMEM sz_combo[]   = {KC_F, C_HOME_S, COMBO_END};

combo_t key_combos[] = {
    [BOOT_COMBO] = COMBO(boot_combo, QK_BOOT),
    [AE_COMBO]   = COMBO(ae_combo, UP(AE_LOWER, AE_UPPER)),
    [OE_COMBO]   = COMBO(oe_combo, UP(OE_LOWER, OE_UPPER)),
    [UE_COMBO]   = COMBO(ue_combo, UP(UE_LOWER, UE_UPPER)),
    [SZ_COMBO]   = COMBO(sz_combo, UM(SZ)),
};

static bool     umlaut_combos_enabled = false;
static uint16_t last_key_press_time;
static uint16_t first_key_after_idle_time;

// Umlaut combos only fire on macOS (or after UM_TOGG) and only when their
// first key starts a new burst of typing, like ZMK's require-prior-idle-ms.
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (combo_index == BOOT_COMBO) {
        return true;
    }
    return umlaut_combos_enabled && timer_elapsed(first_key_after_idle_time) <= COMBO_TERM;
}

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (timer_elapsed(last_key_press_time) >= UMLAUT_PRIOR_IDLE_MS) {
            first_key_after_idle_time = timer_read();
        }
        last_key_press_time = timer_read();
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == UM_TOGG && record->event.pressed) {
        umlaut_combos_enabled = !umlaut_combos_enabled;
        return false;
    }
    return true;
}

bool process_detected_host_os_user(os_variant_t detected_os) {
    umlaut_combos_enabled = detected_os == OS_MACOS || detected_os == OS_IOS;
    return true;
}
