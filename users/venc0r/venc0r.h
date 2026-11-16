// Copyright 2021 Jörg Markert (@venc0r)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include QMK_KEYBOARD_H

#include "rows.h"

enum userspace_layers{
    _ALPHA = 0,   // default
    _LOWER,
    _RAISE,
    _NUMBER,
    _ADJUST,
    _GAME,
};

enum userspace_keycodes {
    UM_TOGG = SAFE_RANGE,
};
