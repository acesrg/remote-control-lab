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

/* third party local libs */
#include <log.h>

/* configuration includes */
#include <pinout_configuration.h>

/* Variable Global: esta variable es global para
 comunucarse entre threads */
static uint16_t valor_adc;

/* Variable Global: comprendida entre 0 y 1 */
static float pwm_duty;

uint8_t SYSTEM_LOG_LEVEL = LOG_INFO;


/**
 * \brief   adc_read.
 */
void adc_read(void *pvParameters) {
    for (;;) {
        valor_adc = sdk_system_adc_read();
        log_debug("Valor: %d", valor_adc);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * \brief   uart_publisher.
 */
void uart_publisher(void *pvParameters) {
    for (;;) {
        printf("{pwm: %f, adc: %d}", pwm_duty, valor_adc);
        log_debug("finished printing");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


/**
 * \brief Program's entrypoint.
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
}
