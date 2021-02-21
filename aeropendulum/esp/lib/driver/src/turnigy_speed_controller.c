/*
 * Copyright 2021 Marco Miretti.
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
#include <stdbool.h>

#include <log.h>
#include <pwm.h>

#include <turnigy_speed_controller.h>

typedef struct pwm_config_t {
    uint16_t frequency_hz;
    uint16_t poweron_duty;
    uint8_t pin;
} pwm_config_t;

#define DEFAULT_DRIVER_PWM_PIN   14
#define DEFAULT_DRIVER_PWM_FREQUENCY_HZ  100
#define DEFAULT_DRIVER_PWM_POWERON_DUTY  0x1C2A
#define DRIVER_PWM_COUNT    1
#define DRIVER_PWM_REVERSE  false

pwm_config_t driver_config = {
    .pin = DEFAULT_DRIVER_PWM_PIN,
    .frequency_hz = DEFAULT_DRIVER_PWM_FREQUENCY_HZ,
    .poweron_duty = DEFAULT_DRIVER_PWM_POWERON_DUTY,
};

retval_t turnigy_speed_controller_init_sequence() {
    log_trace("Set PWM in pin %d", driver_config.pin);
    pwm_init(DRIVER_PWM_COUNT, &driver_config.pin, DRIVER_PWM_REVERSE);

    log_trace("Set PWM frequency to %d Hz", driver_config.frequency_hz);
    pwm_set_freq(driver_config.frequency_hz);

    log_trace("Set PWM default duty");
    pwm_set_duty(driver_config.poweron_duty);

    log_trace("Start PWM");
    pwm_start();

    return RV_OK;
}
