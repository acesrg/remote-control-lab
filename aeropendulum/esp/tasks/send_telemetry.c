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

static SimpleJSONType sensor_db[2] = {{"angle", 0},
                                      {"error", 0}};

void send_telemetry_task(void *pvParameter) {
    log_trace("task started");
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;

    // TODO(marcotti): outer endpoint
    log_trace("encoder init");
    quadrature_encoder_init(ENCODER_PIN_A, ENCODER_PIN_B);

    // Initialise the xLastWakeTime variable with the current time.
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
        sensor_db[0].value = get_encoder_value();
        log_trace("sensor value = %d", sensor_db[0].value);

        char composed_json[JSON_SENSOR_MAX_LEN];
        size_t database_size = sizeof(sensor_db)/sizeof(*sensor_db);

        ParseRvType compose_rv = json_simple_compose(composed_json, sensor_db, database_size);
        if (compose_rv != PARSE_OK) {
            log_error("compose error");
        }

        LOCK_TCPIP_CORE();
        websocket_write(pcb, (uint8_t *) composed_json, strlen(composed_json), WS_TEXT_MODE);

        // send inmediadtly everything written to TCP socket
        tcp_output(pcb);
        UNLOCK_TCPIP_CORE();

        vTaskDelayUntil(&xLastWakeTime, TELEMETRY_RATE_ms / portTICK_PERIOD_MS);
        if (pcb == NULL || pcb->state != ESTABLISHED) {
            // when task stops mark as undefined
            URI_TASK = URI_UNDEF;
            log_trace("disconected, delete task");
            vTaskDelete(NULL);
        }
    }
}
