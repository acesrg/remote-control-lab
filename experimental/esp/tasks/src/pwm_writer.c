/*
 * Copyright 2021 ACES.
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
/** \file main.c */
/* standard */
#include <string.h>

/* third party libs */
#include <FreeRTOS.h>
#include <task.h>
#include <espressif/esp_common.h>
#include <espressif/user_interface.h>
#include <esp/uart.h>
#include <pwm.h>

/* third party local libs */
#include <pwm_writer.h>

/* third party local libs */
#include <log.h>

/* configuration includes */
#include <pinout_configuration.h>


/* Variable Global: comprendida entre 0 y 1 */
static float pwm_duty;

/* Asignación de parametros para PWM */
#define DEFAULT_DRIVER_PWM_PIN              14      /**< \brief Default driver pin */
#define DEFAULT_DRIVER_PWM_FREQUENCY_HZ     100     /**< \brief Default PWM frequency */
#define DRIVER_PWM_COUNT                    1       /**< \brief Quantity of PWMs to use */
#define DRIVER_PWM_REVERSE                  false   /**< \brief PWM Reverse option */
#define PWM_DUTY_OFF                        0x0000  /**< \brief Minimum signal value */
#define PWM_DUTY_MAX                        0xFFFF  /**< \brief Maximum signal value */

/**
 * \brief   PWM configuration structure.
 */
typedef struct pwm_config_t {
    uint16_t frequency_hz;  /**< \brief Frequency of the PWM signal in Hertz */
    uint8_t pin;            /**< \brief Pin where the PWM will be initialized */
} pwm_config_t;


/**
 * \brief   Global PWM driver configuration.
 */
pwm_config_t pwm_config = {
    .pin = DEFAULT_DRIVER_PWM_PIN,
    .frequency_hz = DEFAULT_DRIVER_PWM_FREQUENCY_HZ,
};

/**
 * \brief   PWM_writer
 */
void PWM_writer(void *pvParameters) {
    
    log_trace("Set PWM in pin %d", pwm_config.pin);
    pwm_init(DRIVER_PWM_COUNT, &pwm_config.pin, DRIVER_PWM_REVERSE);

    log_trace("Set PWM frequency to %d Hz", pwm_config.frequency_hz);
    pwm_set_freq(pwm_config.frequency_hz);

    log_trace("Set PWM default duty");
    pwm_set_duty(PWM_DUTY_OFF);

    log_trace("Start PWM");
    pwm_start();
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    for (;;) {
        log_trace("Set PWM default duty");
        pwm_set_duty((uint16_t) (PWM_DUTY_MAX/2));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        log_trace("Set PWM default duty again");
        pwm_set_duty(PWM_DUTY_OFF);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}