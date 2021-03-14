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
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <log.h>

#include <json_parser.h>
#include <propeller_utils.h>

#include <update_actuators.h>


extern SemaphoreHandle_t xMutex_actuator_data;
uint16_t ACTUATORS_UPDATE_PERIOD_ms = DEFAULT_ACTUATORS_UPDATE_PERIOD_ms;

SimpleJSONType actuator_db[1] = {{"duty", 0}};

void update_actuators_task(void *pvParameter) {
    log_trace("task started");

    uint16_t last_actuator_duty = 0;

    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
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
                 * */
                if (actuator_duty_value != last_actuator_duty) {
                    set_propeller_duty(actuator_duty_value);
                }
                last_actuator_duty = actuator_duty_value;
            }
        }
        vTaskDelayUntil(&xLastWakeTime, ACTUATORS_UPDATE_PERIOD_ms / portTICK_PERIOD_MS);
    }
}
