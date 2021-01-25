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
#ifndef EXAMPLES_PROJECT_CALLBACKS_INC_TELEMETRY_CALLBACK_H_
#define EXAMPLES_PROJECT_CALLBACKS_INC_TELEMETRY_CALLBACK_H_

#include <espressif/esp_common.h>
#include <esp8266.h>
#include <string.h>
#include <stdio.h>

typedef enum CallbackRvType {
    CALLBACK_OK,
    CALLBACK_PARSE_ERROR,
} CallbackRvType;

CallbackRvType telemetry_callback_handler(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode);

#endif /* EXAMPLES_PROJECT_CALLBACKS_INC_TELEMETRY_CALLBACK_H_ */
