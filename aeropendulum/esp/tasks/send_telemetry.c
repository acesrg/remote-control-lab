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
#include <send_telemetry.h>
#include <json_parser.h>
#include <encoder.h>


extern uint8_t URI_TASK;

extern SemaphoreHandle_t xMutex_actuator_data;
extern SemaphoreHandle_t xMutex_sensor_data;

extern PwmConfigType pwm_config;

SimpleJSONType actuator_db[1] = {{"duty", DEFAULT_PROPELLER_PWM_POWERON_DUTY}};

SimpleJSONType sensor_db[2] = {{"angle", 0},
                               {"error", 0}};

void send_telemetry_task(void *pvParameter) {
    log_trace("task started");
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;

    log_trace("encoder init");
    quadrature_encoder_init(ENCODER_PIN_A, ENCODER_PIN_B);

    uint16_t last_actuator_duty = 0;

    while (1) {
        if (xMutex_actuator_data != NULL) {
            /* See if we can obtain the actuator_db mutex */
            if (xSemaphoreTake(xMutex_actuator_data, (TickType_t) 100) == pdTRUE) {
                uint16_t actuator_duty_value = actuator_db[0].value;
                xSemaphoreGive(xMutex_actuator_data);

                /*
                 * PWM workarround:
                 *
                 * It seems that updating the pwm during the duty cycle causes it to go down and
                 * inmediatly up again. This could produce unintended behavior, such as turning
                 * the PWM off at an specific time, that the motor driver would interpret as some
                 * new command (eg. between 1.1 and 2 ms, for the turnigy drivers).
                 *
                 * To mitigate this behaviors, pwm duty is updated only whe it has actually
                 * changed.
                 *
                 * Then, to supress the possibility of unexpected commands making it to the PWM
                 * driver, the duty cycle is updated ONLY during PWM's low state.
                 *
                 * This is a workarround, the ideal behavior is that the pwm duty cycle updates at
                 * the beginning of a new cycle only. This doesn't happen here.
                 * */
                if (actuator_duty_value != last_actuator_duty) {
                    while (gpio_read(pwm_config.pin)) {
                        log_trace("waiting for duty cycle to end ...");
                        vTaskDelay(0.1 / (1000 * pwm_config.frequency_hz * portTICK_PERIOD_MS));
                    }
                    log_trace("set actuator duty: 0x%04X", actuator_duty_value);
                    taskENTER_CRITICAL();
                    pwm_set_duty(actuator_duty_value);
                    taskEXIT_CRITICAL();
                }
                last_actuator_duty = actuator_duty_value;
            }
        }

        if (xMutex_sensor_data != NULL) {
            /* See if we can obtain the actuator_db mutex */
            if (xSemaphoreTake(xMutex_sensor_data, (TickType_t) 100) == pdTRUE) {
                sensor_db[0].value = get_encoder_value();
                log_trace("sensor value = %d", sensor_db[0].value);
                xSemaphoreGive(xMutex_sensor_data);
            }
        }

        vTaskDelay(TELEMETRY_RATE_ms / portTICK_PERIOD_MS);

        if (pcb == NULL || pcb->state != ESTABLISHED) {
            // when task stops mark as undefined
            URI_TASK = URI_UNDEF;
            log_trace("disconected, delete task");
            vTaskDelete(NULL);
        }
    }
}
