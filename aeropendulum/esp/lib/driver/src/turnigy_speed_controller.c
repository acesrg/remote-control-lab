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
/** \file turnigy_speed_controller.c */
#include <stdbool.h>

#include <esp/gpio.h>
#include <log.h>
#include <pwm.h>
#include <FreeRTOS.h>
#include <task.h>

#include <turnigy_speed_controller.h>

/**
 * \brief   PWM configuration structure.
 */
typedef struct pwm_config_t {
    uint16_t frequency_hz;  /**< \brief Frequency of the PWM signal in Hertz */
    uint16_t poweron_duty;  /**< \brief Duty necessary to init the propeller */
    uint8_t pin;            /**< \brief Pin where the PWM will be initialized */
} pwm_config_t;

#define DEFAULT_DRIVER_PWM_PIN              14      /**< \brief Default driver pin */
#define DEFAULT_DRIVER_PWM_FREQUENCY_HZ     100     /**< \brief Default PWM frequency */
#define DEFAULT_DRIVER_PWM_POWERON_DUTY     0x1C2A  /**< \brief Default duty that will init the driver */
#define DEFAULT_DRIVER_PWM_POWEROFF_DUTY    0x0001  /**< \brief Default duty that will stop the driver */
#define DRIVER_PWM_COUNT                    1       /**< \brief Quantity of PWMs to use */
#define DRIVER_PWM_REVERSE                  false   /**< \brief PWM Reverse option */

/**
 * \brief   Global PWM driver configuration.
 */
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

retval_t turnigy_speed_controller_deinit_sequence() {
    log_trace("Set PWM 0 duty");
    pwm_set_duty(DEFAULT_DRIVER_PWM_POWEROFF_DUTY);

    log_trace("Stop PWM");
    pwm_stop();

    return RV_OK;
}

retval_t turnigy_speed_controller_update_pwm_duty(uint16_t duty) {
    /**
     * \note    PWM workaround:
     *
     *     It seems that updating the pwm during the duty cycle causes it to go down and
     *     immediately up again. This could produce unintended behavior, such as turning
     *     the PWM off at an specific time, that the motor driver would interpret as some
     *     new command (eg. between 1.1 and 2 ms, for the turnigy drivers).
     *
     *     To suppress the possibility of unexpected commands making it to the PWM
     *     driver, the duty cycle is updated ONLY during PWM's low state.
     *
     *     This is a workaround, the ideal behavior is that the PWM duty cycle updates at
     *     the beginning of a new cycle only. This doesn't happen here.
     * */
    while (gpio_read(driver_config.pin)) {
        log_trace("waiting for duty cycle to end ...");
        vTaskDelay(0.1 / (1000 * driver_config.frequency_hz * portTICK_PERIOD_MS));
    }
    log_trace("set actuator duty: 0x%04X", duty);
    taskENTER_CRITICAL();
    pwm_set_duty(duty);
    taskEXIT_CRITICAL();

    return RV_OK;
}
