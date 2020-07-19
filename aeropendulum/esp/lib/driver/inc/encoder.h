/*
 * Copyright 2020 Marco Miretti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#ifndef EXAMPLES_PROJECT_LIB_DRIVER_INC_ENCODER_H_
#define EXAMPLES_PROJECT_LIB_DRIVER_INC_ENCODER_H_

#include <espressif/esp_common.h>
#include <esp8266.h>
#include <log.h>

typedef struct EncoderObjectType {
    uint8_t pin_a;
    uint8_t pin_b;
    size_t  last_state;
    uint16_t value;
} EncoderObjectType;

typedef enum EncoderEventType {
    NO_MEASURE,
    PIN_A_RISE,
    PIN_B_RISE,
    PIN_A_FALL,
    PIN_B_FALL
} EncoderEventType;

#define INCREMENT (uint8_t) (PIN_B_RISE - PIN_A_RISE)
#define INCREMENT_ (uint8_t) (PIN_A_RISE - PIN_B_FALL)

#define DECREMENT (uint8_t) (PIN_A_RISE - PIN_B_RISE)
#define DECREMENT_ (uint8_t) (PIN_B_FALL - PIN_A_RISE)

void quadrature_encoder_init(uint8_t pin_a, uint8_t pin_b);

uint16_t get_encoder_value();
void set_encoder_value(uint16_t value);

#endif /* EXAMPLES_PROJECT_LIB_DRIVER_INC_ENCODER_H_ */
