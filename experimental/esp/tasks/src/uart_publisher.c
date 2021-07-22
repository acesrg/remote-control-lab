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
#include <log.h>

/* configuration includes */
#include <pinout_configuration.h>

/*Asignación*/
#define PWM_DUTY_OFF                        0x0000  /**< \brief Minimum signal value */
#define PWM_DUTY_MAX                        0xFFFF  /**< \brief Maximum signal value */

/* Global variable: between 0 and 1 */
static float pwm_duty;
static float pwm_duty_percentage;
static float adc_voltage;

/**
 * \brief   uart_publisher.
 */
void uart_publisher(void *pvParameters) {
    for (;;) {
        pwm_duty_percentage = pwm_duty*(100.0/(PWM_DUTY_MAX-1.0));
        printf("{\"pwm\": %f, \"adc\": %f}\n", pwm_duty_percentage, adc_voltage);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
