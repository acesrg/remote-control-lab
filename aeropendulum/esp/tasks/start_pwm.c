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
#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <http_server.h>
#include <classic_controller.h>
#include <json_parser.h>
#include <pwm.h>


typedef struct PwmConfigType {
    uint16_t frequency_hz;
    uint16_t poweron_duty;
    uint16_t pin;
} PwmConfigType;

extern uint8_t URI_TASK;

PwmConfigType pwm_config = {
                            DEFAULT_PROPELLER_PWM_FREQUENCY_HZ,
                            DEFAULT_PROPELLER_PWM_POWERON_DUTY,
                            PROPELLER_PWM_PIN
                            };

void start_pwm_task(void *pvParameter) {
    uint8_t pins[1];
    log_trace("task started");
    log_trace("pwm_init(1, [%d])", pwm_config.pin);
    pins[0] = pwm_config.pin;
    pwm_init(1, pins, false);

    log_trace("pwm_set_freq %d", pwm_config.frequency_hz);
    pwm_set_freq(pwm_config.frequency_hz);

    log_trace("pwm set duty");
    pwm_set_duty(pwm_config.poweron_duty);

    log_trace("pwm_start");
    pwm_start();

    URI_TASK = URI_UNDEF;
    log_trace("disconected, delete task");
    vTaskDelete(NULL);
}
