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
#ifndef EXAMPLES_PROJECT_INCLUDE_HTTP_SERVER_H_
#define EXAMPLES_PROJECT_INCLUDE_HTTP_SERVER_H_

#include <log.h>


/**
 * Pinout for the ESP8266 D1 MINI
 * https://user-images.githubusercontent.com/26353057/87876217-f3333a00-c9ac-11ea-9a11-893db497dc48.png
 */
#define LED_PIN             2

#define ENCODER_PIN_A       12
#define ENCODER_PIN_B       13

#define PROPELLER_PWM_PIN   14

/**
 * Propeller default configs
 * */
#define DEFAULT_PROPELLER_PWM_FREQUENCY_HZ  100
#define DEFAULT_PROPELLER_PWM_POWERON_DUTY  0x1C2A

enum URI_TASK {
    URI_UNDEF,
    URI_CLASSIC,
    URI_START_PWM,
    URI_PING,
    URI_PARSE_TEST,
};

typedef struct PwmConfigType {
    uint16_t frequency_hz;
    uint16_t poweron_duty;
    uint16_t pin;
} PwmConfigType;

void start_pwm_task(void *pvParameter);

void classic_controller_task(void *pvParameter);

void ping_task(void *pvParameter);

void test_task(void *pvParameter);

#endif /* EXAMPLES_PROJECT_INCLUDE_HTTP_SERVER_H_ */
