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
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <httpd/httpd.h>
#include <http_server.h>
#include <callback_classic.h>
#include <classic_controller.h>
#include <json_parser.h>


extern SimpleJSONType actuator_db[1];
extern SimpleJSONType sensor_db[2];

extern SemaphoreHandle_t xMutex_actuator_data;
extern SemaphoreHandle_t xMutex_sensor_data;

CallbackRvType classic_callback_handler(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode) {
    /*
     * first thing to do when entering the callback for the classic
     * controller is to write the sensor data to the websocket. This
     * way we avoid adding the parsing time to the websocket latency.
     * */
    char composed_json[JSON_SENSOR_MAX_LEN];
    size_t database_size = sizeof(sensor_db)/sizeof(*sensor_db);

    if (xMutex_actuator_data != NULL) {
        /* See if we can obtain the actuator_db mutex */
        if (xSemaphoreTake(xMutex_actuator_data, (TickType_t) 100) == pdTRUE) {
            ParseRvType compose_rv = json_simple_compose(composed_json, sensor_db, database_size);

            xSemaphoreGive(xMutex_actuator_data);
            if (compose_rv != PARSE_OK) {
                return CALLBACK_PARSE_ERROR;
            }
        }
    }
    websocket_write(pcb, (uint8_t *) composed_json, strlen(composed_json), WS_TEXT_MODE);


    /*
     * then, once the response was written to the websocket start
     * processing the input data.
     *
     * since the classic controller database is being used, and the
     * json shold have the form of {"duty": %f}. We pass directly
     * the structure (initialized with said form).
     *
     * after this call, actuator_db.value should be updated and 
     * ready to use.
     * */
    if (xMutex_sensor_data != NULL) {
        /* See if we can obtain the actuator_db mutex */
        if (xSemaphoreTake(xMutex_sensor_data, (TickType_t) 100) == pdTRUE) {
            ParseRvType parse_rv = quick_get_value((const char *) data, actuator_db);

            xSemaphoreGive(xMutex_sensor_data);

            if (parse_rv != PARSE_OK) {
                return CALLBACK_PARSE_ERROR;
            }
        }
    }

    /* if we get to this poin everything went ok! */
    return CALLBACK_OK;
}