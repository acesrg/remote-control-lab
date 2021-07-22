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

/* Global variable: between 0 and 1 */
uint8_t SYSTEM_LOG_LEVEL = LOG_INFO;

/**
 * \brief Program entrypoint.
 */
void user_init(void) {
    uart_set_baud(0, 115200);
    log_set_level(SYSTEM_LOG_LEVEL);
    log_info("SDK version:%s ", sdk_system_get_sdk_version());

    /* turn off LED */
    gpio_enable(ONBOARD_LED_PIN, GPIO_OUTPUT);
    gpio_write(ONBOARD_LED_PIN, true);

    /* initialize tasks */
    xTaskCreate(&adc_read, "adc read", 256, NULL, 2, NULL);
    xTaskCreate(&uart_publisher, "uart publisher", 256, NULL, 2, NULL);
    xTaskCreate(&PWM_writer, "PWM_writer", 256, NULL, 2, NULL);
}
